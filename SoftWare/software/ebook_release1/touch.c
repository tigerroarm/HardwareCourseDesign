/*
 * touch.c
 *
 *  Created on: 2019��5��9��
 *      Author: lbl
 */
#include "touch.h"
#include <math.h>
#include <unistd.h>
#include "alt_types.h"
#include "touch_resp.h"

unsigned intnirq_isr_context;// ����ȫ�ֱ����Դ���isr_contextָ��

volatile int edge_capture;


_m_tp_dev tp_dev=
{
	TP_Init,
	TP_Scan,
	{0},
	{0},
	0,
	0,
	0,
	0,
	0,
	0,
};

//Ĭ��Ϊtouchtype=0������.
unsigned char CMD_RDX=0XD0;
unsigned char CMD_RDY=0X90;

//��ʱ����
void delay_us(int x){
	int i;
	int end = x*100;
	for(i = 0; i < end; i++);
}

/*
 * RT_MISO	msg[0]			RT_MOSI	ctrl[1]
 * RT_PEN	pen_intr[0]		RT_BUSY	msg[1]
 * RT_CS	ctrl[0]			RT_CLK	ctrl[2]
 */

//SPIд����
//������ICд��1byte����
//MOSI�����豸��������豸����
//num:Ҫд�������
void TP_Write_Byte(unsigned char num)
{
	unsigned char count=0;
	int clk;
	int temp = IORD(RT_MOSI_BASE, 0);
	usleep(1);

	for(count=0;count<8;count++)
	{
		if(num&0x80)temp = (temp&0xfffffffd)|0x00000002;//ctr[1] = 1
		else temp = temp&0xfffffffd;//ctr[1] = 0

		IOWR(RT_MOSI_BASE, 0, temp);
		usleep(1);

		num<<=1;

		clk = IORD(RT_CLK_BASE, 0);

		clk = clk&0xfffffffb;//ctr[2] = 0
		IOWR(RT_CLK_BASE, 0, clk);
		usleep(1);

		clk = clk|0x00000004;//ctr[2] = 1
		IOWR(RT_CLK_BASE, 0, clk);		//��������Ч
		usleep(1);
	}
}

//MISO�����豸���룬���豸���
unsigned short int TP_Read_AD(unsigned char CMD)
{
	unsigned char count=0;
	unsigned short int Num=0;
	int clk;
	int DOUT;
	/*
	TCLK=0;		//������ʱ��
	TDIN=0; 	//����������
	TCS=0; 		//ѡ�д�����IC
	*/
	int temp = IORD(RT_MOSI_BASE, 0); // ctr[2:0] = 000
	temp = temp&0xfffffff8;
	usleep(1);
	IOWR(RT_MOSI_BASE, 0, temp);
	usleep(1);

	TP_Write_Byte(CMD);//����������
	usleep(6);//ADS7846��ת��ʱ���Ϊ6us

	//TCLK=0;
	clk = IORD(RT_CLK_BASE, 0); usleep(1);
	clk = clk&0xfffffffb;
	IOWR(RT_CLK_BASE, 0, clk); usleep(1);//ctr[2] = 0


	//TCLK=1;		//��1��ʱ�ӣ����BUSY
	clk = IORD(RT_CLK_BASE, 0); usleep(1);
	clk = clk|0x00000004;
	IOWR(RT_CLK_BASE, 0, clk); usleep(1);//ctr[2] = 1


	//TCLK=0;
	clk = IORD(RT_CLK_BASE, 0); usleep(1);
	clk = clk&0xfffffffb;
	IOWR(RT_CLK_BASE, 0, clk);usleep(1);//ctr[2] = 0


	for(count=0;count<16;count++)//����16λ����,ֻ�и�12λ��Ч
	{
		Num<<=1;

		//TCLK=0;	//�½�����Ч
		clk = IORD(RT_CLK_BASE, 0); usleep(1);
		clk = clk&0xfffffffb;
		IOWR(RT_CLK_BASE, 0, clk);usleep(1);//ctr[2] = 0

		//TCLK=1;
		clk = IORD(RT_CLK_BASE, 0); usleep(1);
		clk = clk|0x00000004;
		IOWR(RT_CLK_BASE, 0, clk);usleep(1);//ctr[2] = 1

		DOUT = IORD(RT_MISO_BASE,0); usleep(1);
		DOUT = DOUT&0x00000001;
		if(DOUT)Num++;
	}

	Num>>=4;   	//ֻ�и�12λ��Ч.

	//TCS=1;		//�ͷ�Ƭѡ
	temp = IORD(RT_CS_BASE,0); usleep(1);
	temp = temp|0x00000001;
	IOWR(RT_CS_BASE, 0, temp); usleep(1);

	return(Num);
}

//��ȡһ������ֵ(x����y)
//������ȡREAD_TIMES������,����Щ������������,
//Ȼ��ȥ����ͺ����LOST_VAL����,ȡƽ��ֵ
//xy:ָ�CMD_RDX/CMD_RDY��
//����ֵ:����������
#define READ_TIMES 5 	//��ȡ����
#define LOST_VAL 1	  	//����ֵ
unsigned short int TP_Read_XOY(unsigned char xy){
	unsigned short int i, j;
	unsigned short int buf[READ_TIMES];
	unsigned short int sum=0;
	unsigned short int temp;
	for(i=0;i<READ_TIMES;i++)buf[i]=TP_Read_AD(xy);
	for(i=0;i<READ_TIMES-1; i++)//����
	{
		for(j=i+1;j<READ_TIMES;j++)
		{
			if(buf[i]>buf[j])//��������
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}
	sum=0;
	for(i=LOST_VAL;i<READ_TIMES-LOST_VAL;i++)sum+=buf[i];
	temp=sum/(READ_TIMES-2*LOST_VAL);
	return temp;

}

//��ȡx,y����
//��Сֵ��������100.
//x,y:��ȡ��������ֵ
//����ֵ:0,ʧ��;1,�ɹ���
unsigned char TP_Read_XY(unsigned short int *x,unsigned short int *y)
{
	unsigned short int xtemp,ytemp;
	xtemp=TP_Read_XOY(CMD_RDX);
	ytemp=TP_Read_XOY(CMD_RDY);
	//if(xtemp<100||ytemp<100)return 0;//����ʧ��
	*x=xtemp;
	*y=ytemp;
	return 1;//�����ɹ�
}

//����2�ζ�ȡ������IC,�������ε�ƫ��ܳ���
//ERR_RANGE,��������,����Ϊ������ȷ,�����������.
//�ú����ܴ�����׼ȷ��
//x,y:��ȡ��������ֵ
//����ֵ:0,ʧ��;1,�ɹ���
#define ERR_RANGE 50 //��Χ
unsigned char TP_Read_XY2(unsigned short int *x,unsigned short int *y)
{
	unsigned short int x1,y1;
	unsigned short int x2,y2;
	unsigned char flag;
    flag=TP_Read_XY(&x1,&y1);
    if(flag==0)return(0);
    flag=TP_Read_XY(&x2,&y2);
    if(flag==0)return(0);
    if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))//ǰ�����β�����+-50��
    &&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
    {
        *x=(x1+x2)/2;
        *y=(y1+y2)/2;
        return 1;
    }else return 0;
}


//////////////////////////////////////////////////////////////////////////////////
//��������ɨ��
//tp:0,��Ļ����;1,��������(У׼�����ⳡ����)
//����ֵ:��ǰ����״̬.
//0,�����޴���;1,�����д���
unsigned char TP_Scan(unsigned char tp){
	int PEN = IORD(RT_PEN_BASE, 0);
	PEN = PEN&0x00000001;
	if(PEN==0)//�а�������
	{
		if(tp)TP_Read_XY2(&tp_dev.x[0],&tp_dev.y[0]);//��ȡ��������
		else if(TP_Read_XY2(&tp_dev.x[0],&tp_dev.y[0]))//��ȡ��Ļ����
		{
	 		tp_dev.x[0]=tp_dev.xfac*tp_dev.x[0]+tp_dev.xoff;//�����ת��Ϊ��Ļ����
			tp_dev.y[0]=tp_dev.yfac*tp_dev.y[0]+tp_dev.yoff;
	 	}
		if((tp_dev.sta&TP_PRES_DOWN)==0)//֮ǰû�б�����
		{
			tp_dev.sta=TP_PRES_DOWN|TP_CATH_PRES;//��������
			tp_dev.x[4]=tp_dev.x[0];//��¼��һ�ΰ���ʱ������
			tp_dev.y[4]=tp_dev.y[0];
		}
	}else
	{
		if(tp_dev.sta&TP_PRES_DOWN)//֮ǰ�Ǳ����µ�
		{
			tp_dev.sta&=~(1<<7);//��ǰ����ɿ�
		}else//֮ǰ��û�б�����
		{
			tp_dev.x[4]=0;
			tp_dev.y[4]=0;
			tp_dev.x[0]=0xffff;
			tp_dev.y[0]=0xffff;
		}
	}
	return tp_dev.sta&TP_PRES_DOWN;//���ص�ǰ�Ĵ���״̬
}




//У׼�������(0,1)��(2,3)��(0,2)��(1,3),(1,2)��(0,3)���������ľ���
const unsigned char TP_ADJDIS_TBL[3][4]={{0,1,2,3},{0,2,1,3},{1,2,0,3}};//У׼��������

//��������ʼ��
//����ֵ:0,û�н���У׼
//       1,���й�У׼
unsigned char TP_Init(void)
{

	IOWR(TOUCH_MSG_BASE, 1, 0x00000000);
	IOWR(TOUCH_PEN_INTR_BASE, 1, 0x00000000);
	IOWR(TOUCH_CTRL_BASE, 1, 0x00000007);
	//������ض˿ڵ��������

	//�趨touch_pen�����ʲ������ڣ���λms������С��λ��0.16384ms)
	float smpPeriod = 1;
	IOWR_ALTERA_AVALON_PIO_DATA( PEN_SMP_SPEED_BASE, (alt_u16)(smpPeriod / 0.16384 ));

	//�жϳ�ʼ��
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(RT_PEN_BASE, 1);// ʹ���ж�
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(RT_PEN_BASE, 1);// ���жϱ��ز���Ĵ���
	// ע��ISR

	alt_ic_isr_register(
		  TOUCH_PEN_INTR_IRQ_INTERRUPT_CONTROLLER_ID,// �жϿ�������ţ���system.h����
		  TOUCH_PEN_INTR_IRQ,    // Ӳ���жϺţ���system.h����
	      intr_scan_headler ,             // �жϷ����Ӻ���
	      NULL,       // ָ�����豸����ʵ����ص����ݽṹ��
	      NULL);                  // flags������δ��



	//����ͨ�Ų���
	touchPosFlag.scrTaskBusy = true;
	touchPosFlag.touchOccur = false;
	touchPosFlag.touchIntrStatus = true;
	touchPosFlag.touchPos.x_pos = 1;
	touchPosFlag.touchPos.y_pos = 1;

	return 1;
}

void physical2logical(unsigned short xp, unsigned short yp, unsigned short* addx,  unsigned short* addy){						//ת���������굽�߼�����
	unsigned short tempx, tempy;
	double xfac = -0.087397;//(320-1)/(170-3820)
	double xoff = 1-3820*xfac;
	double yfac = -0.126385;//(480-1)/(120-3910)
	double yoff = 1-3910*yfac;
	tempx = (unsigned short)(xoff + xfac*xp);
	tempy = (unsigned short)(yoff + yfac*yp);

	if(tempx > 320) *addx = 320;
	else if( tempx < 1) *addx = 1;
	else *addx = tempx;

	if(tempy > 480) *addy = 480;
	else if( tempy < 1) *addy = 1;
	else *addy = tempy;

	return;
}


void intr_scan_headler(void* isr_context){

	int PEN = IORD(RT_PEN_BASE, 0);


	if ( touchPosFlag.scrTaskBusy )//������æ
	{
		IOWR_ALTERA_AVALON_PIO_EDGE_CAP(RT_PEN_BASE, 0x1);// ���жϱ��ز���Ĵ���
		return;//�˴δ�����Ч
	}

	usleep(10000);

	touchPosFlag.touchOccur = true;

	unsigned short tempx, tempy;

	TP_Read_XY2(&tempx, &tempy);//��ȡ��������
	physical2logical(tempx, tempy, &(touchPosFlag.touchPos.x_pos), &(touchPosFlag.touchPos.y_pos));

	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(RT_PEN_BASE, 0x1);// ���жϱ��ز���Ĵ���

	return;
}