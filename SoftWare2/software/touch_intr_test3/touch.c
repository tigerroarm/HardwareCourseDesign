/*
 * touch.c
 *
 *  Created on: 2019��5��9��
 *      Author: lbl
 */
#include "touch.h"
#include "lcd.h"
#include <math.h>
#include <unistd.h>
#include "alt_types.h"


TouchPos touchPosInst;

unsigned intnirq_isr_context;// ����ȫ�ֱ����Դ���isr_contextָ��

volatile int edge_capture;


_m_tp_dev tp_dev=
{
	TP_Init,
	TP_Scan,
	TP_Adjust,
	0,
	0,
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
//��LCD�����йصĺ���
//��һ��������
//����У׼�õ�
//x,y:����
//color:��ɫ
void TP_Drow_Touch_Point(unsigned short int x,unsigned short int y,unsigned short int color){
	POINT_COLOR=color;
	LCD_DrawLine(x-12,y,x+13,y);//����
	LCD_DrawLine(x,y-12,x,y+13);//����
	LCD_DrawPoint(x+1,y+1);
	LCD_DrawPoint(x-1,y+1);
	LCD_DrawPoint(x+1,y-1);
	LCD_DrawPoint(x-1,y-1);
	LCD_Draw_Circle(x,y,6);//������Ȧ
}

//��һ�����(2*2�ĵ�)
//x,y:����
//color:��ɫ
void TP_Draw_Big_Point(unsigned short int x,unsigned short int y,unsigned short int color){
	POINT_COLOR=color;
	LCD_DrawPoint(x,y);//���ĵ�
	LCD_DrawPoint(x+1,y);
	LCD_DrawPoint(x,y+1);
	LCD_DrawPoint(x+1,y+1);
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

//////////////////////////////////////////////////////////////////////////
//������EEPROM����ĵ�ַ�����ַ,ռ��13���ֽ�(RANGE:SAVE_ADDR_BASE~SAVE_ADDR_BASE+12)
//����У׼����
extern unsigned char adjdata[14];//���������ⲿȫ�ֱ���
unsigned char* add = &adjdata[0];
void TP_Save_Adjdata(void)
{
	int temp;
	//����У�����!
	temp=tp_dev.xfac*100000000;	//����xУ������
    *((int*)(add)) = temp;		//AT24CXX_WriteLenByte(SAVE_ADDR_BASE,temp,4);
	temp=tp_dev.yfac*100000000;	//����yУ������
	*((int*)(add+4)) = temp;	//AT24CXX_WriteLenByte(SAVE_ADDR_BASE+4,temp,4);
	//����xƫ����
    *((short*)(add+8)) = tp_dev.xoff; 	//AT24CXX_WriteLenByte(SAVE_ADDR_BASE+8,tp_dev.xoff,2);
	//����yƫ����
    *((short*)(add+10)) = tp_dev.yoff; 	//AT24CXX_WriteLenByte(SAVE_ADDR_BASE+10,tp_dev.yoff,2);
	//���津������
	*(add+12) = tp_dev.touchtype;		//AT24CXX_WriteOneByte(SAVE_ADDR_BASE+12,tp_dev.touchtype);
	temp=0X0A;//���У׼����
	*(add+13) = temp;					//AT24CXX_WriteOneByte(SAVE_ADDR_BASE+13,temp);
}

//�õ�������EEPROM�����У׼ֵ
//����ֵ��1���ɹ���ȡ����
//        0����ȡʧ�ܣ�Ҫ����У׼
unsigned char TP_Get_Adjdata(void){
	int tempfac;
	unsigned char temp;
	temp = *(add+13);	//��ȡ�����,���Ƿ�У׼����
	if(temp==0X0A)//�������Ѿ�У׼����
	{
		tempfac=*((int*)(add));
		tp_dev.xfac=(float)tempfac/100000000;//�õ�xУ׼����
		tempfac=*((int*)(add+4));
		tp_dev.yfac=(float)tempfac/100000000;//�õ�yУ׼����
		//�õ�xƫ����
		tp_dev.xoff=*((short*)(add+8));
		//�õ�yƫ����
		tp_dev.yoff=*((short*)(add+10));
		tp_dev.touchtype=*(add+12);			//��ȡ�������ͱ��
		if(tp_dev.touchtype)//X,Y��������Ļ�෴
		{
			CMD_RDX=0X90;
			CMD_RDY=0XD0;
		}else				   //X,Y��������Ļ��ͬ
		{
			CMD_RDX=0XD0;
			CMD_RDY=0X90;
		}
		return 1;
	}
	return 0;

}

const unsigned char* TP_REMIND_MSG_TBL="Please use the stylus click the cross on the screen.The cross will always move until the screen adjustment is completed.";
//��ʾУ׼���(��������)
void TP_Adj_Info_Show(unsigned short int x0,unsigned short int y0,unsigned short int x1,unsigned short int y1,unsigned short int x2,unsigned short int y2,unsigned short int x3,unsigned short int y3,unsigned short int fac){
	POINT_COLOR=RED;
	LCD_ShowString(40,160,lcddev.width,lcddev.height,16,"x1:");
	LCD_ShowString(40+80,160,lcddev.width,lcddev.height,16,"y1:");
	LCD_ShowString(40,180,lcddev.width,lcddev.height,16,"x2:");
	LCD_ShowString(40+80,180,lcddev.width,lcddev.height,16,"y2:");
	LCD_ShowString(40,200,lcddev.width,lcddev.height,16,"x3:");
	LCD_ShowString(40+80,200,lcddev.width,lcddev.height,16,"y3:");
	LCD_ShowString(40,220,lcddev.width,lcddev.height,16,"x4:");
	LCD_ShowString(40+80,220,lcddev.width,lcddev.height,16,"y4:");
	LCD_ShowString(40,240,lcddev.width,lcddev.height,16,"fac is:");
	LCD_ShowNum(40+24,160,x0,4,16);		//��ʾ��ֵ
	LCD_ShowNum(40+24+80,160,y0,4,16);	//��ʾ��ֵ
	LCD_ShowNum(40+24,180,x1,4,16);		//��ʾ��ֵ
	LCD_ShowNum(40+24+80,180,y1,4,16);	//��ʾ��ֵ
	LCD_ShowNum(40+24,200,x2,4,16);		//��ʾ��ֵ
	LCD_ShowNum(40+24+80,200,y2,4,16);	//��ʾ��ֵ
	LCD_ShowNum(40+24,220,x3,4,16);		//��ʾ��ֵ
	LCD_ShowNum(40+24+80,220,y3,4,16);	//��ʾ��ֵ
	LCD_ShowNum(40+56,lcddev.width,fac,3,16); 	//��ʾ��ֵ,����ֵ������95~105��Χ֮��.
}

//У׼�������(0,1)��(2,3)��(0,2)��(1,3),(1,2)��(0,3)���������ľ���
const unsigned char TP_ADJDIS_TBL[3][4]={{0,1,2,3},{0,2,1,3},{1,2,0,3}};//У׼��������
//������У׼����
//�õ��ĸ�У׼����
void TP_Adjust(void)
{
	unsigned short int pos_temp[4][2];//���껺��ֵ
	unsigned char  cnt=0;
	unsigned short int d1,d2;
	unsigned int tem1,tem2;
	float fac;
	unsigned short int outtime=0;
	LCD_Clear(WHITE);	//����
	POINT_COLOR=BLUE;	//��ɫ
	LCD_ShowString(40,40,160,100,16,(unsigned char*)TP_REMIND_MSG_TBL);//��ʾ��ʾ��Ϣ
	TP_Drow_Touch_Point(20,20,RED);//����1
	tp_dev.sta=0;//���������ź�
	tp_dev.xfac=0;//xfac��������Ƿ�У׼��,����У׼֮ǰ�������!�������
	while(1)//�������10����û�а���,���Զ��˳�
	{
READJ:
		tp_dev.scan(1);//ɨ����������
		if((tp_dev.sta&0xc0)==TP_CATH_PRES)//����������һ��(��ʱ�����ɿ���.)
		{
			outtime=0;
			tp_dev.sta&=~(1<<6);	//��ǰ����Ѿ�����������.
			pos_temp[cnt][0]=tp_dev.x[0];
			pos_temp[cnt][1]=tp_dev.y[0];
			cnt++;
			switch(cnt)
			{
				case 1:
					TP_Drow_Touch_Point(20,20,WHITE);				//�����1
					TP_Drow_Touch_Point(lcddev.width-20,20,RED);	//����2
					break;
				case 2:
 					TP_Drow_Touch_Point(lcddev.width-20,20,WHITE);	//�����2
					TP_Drow_Touch_Point(20,lcddev.height-20,RED);	//����3
					break;
				case 3:
 					TP_Drow_Touch_Point(20,lcddev.height-20,WHITE);			//�����3
 					TP_Drow_Touch_Point(lcddev.width-20,lcddev.height-20,RED);	//����4
					break;
				case 4:	 //ȫ���ĸ����Ѿ��õ�
					for(cnt=0;cnt<3;cnt++)//���������ľ����Ƿ���������Χ�ڣ�
					{
						tem1=abs(pos_temp[TP_ADJDIS_TBL[cnt][0]][0]-pos_temp[TP_ADJDIS_TBL[cnt][1]][0]);//x1-x2/x1-x3/x2-x3
						tem2=abs(pos_temp[TP_ADJDIS_TBL[cnt][0]][1]-pos_temp[TP_ADJDIS_TBL[cnt][1]][1]);//y1-y2/y1-y3/y2-y3
						tem1*=tem1;
						tem2*=tem2;
						d1=sqrt(tem1+tem2);//�õ�����֮��ľ���
						tem1=abs(pos_temp[TP_ADJDIS_TBL[cnt][2]][0]-pos_temp[TP_ADJDIS_TBL[cnt][3]][0]);//x3-x4/x2-x4/x1-x4
						tem2=abs(pos_temp[TP_ADJDIS_TBL[cnt][2]][1]-pos_temp[TP_ADJDIS_TBL[cnt][3]][1]);//y3-y4/y2-y4/y1-y4
						tem1*=tem1;
						tem2*=tem2;
						d2=sqrt(tem1+tem2);//�õ�����֮��ľ���
						fac=(float)d1/d2;
						if(fac<0.95||fac>1.05||d1==0||d2==0)//���ϸ�
						{
							cnt=0;
							TP_Drow_Touch_Point(lcddev.width-20,lcddev.height-20,WHITE);	//�����4
							TP_Drow_Touch_Point(20,20,RED);									//����1
							TP_Adj_Info_Show(pos_temp[0][0],pos_temp[0][1],pos_temp[1][0],pos_temp[1][1],pos_temp[2][0],pos_temp[2][1],pos_temp[3][0],pos_temp[3][1],fac*100);//��ʾ����
							goto READJ;	//���ϸ�����У׼
						}
					}
					//��ȷ��
					//������
					tp_dev.xfac=(float)(lcddev.width-40)/(pos_temp[1][0]-pos_temp[0][0]);//�õ�xfac
					tp_dev.xoff=(lcddev.width-tp_dev.xfac*(pos_temp[1][0]+pos_temp[0][0]))/2;//�õ�xoff
 					tp_dev.yfac=(float)(lcddev.height-40)/(pos_temp[2][1]-pos_temp[0][1]);//�õ�yfac
					tp_dev.yoff=(lcddev.height-tp_dev.yfac*(pos_temp[2][1]+pos_temp[0][1]))/2;//�õ�yoff
					if(abs(tp_dev.xfac)>2||abs(tp_dev.yfac)>2)//������Ԥ����෴��.
					{
						cnt=0;
 				    	TP_Drow_Touch_Point(lcddev.width-20,lcddev.height-20,WHITE);	//�����4
   	 					TP_Drow_Touch_Point(20,20,RED);								//����1
						LCD_ShowString(40,26,lcddev.width,lcddev.height,16,"TP Need readjust!");
						tp_dev.touchtype=!tp_dev.touchtype;//�޸Ĵ�������.
						if(tp_dev.touchtype)//X,Y��������Ļ�෴
						{
							CMD_RDX=0X90;
							CMD_RDY=0XD0;
						}else				   //X,Y��������Ļ��ͬ
						{
							CMD_RDX=0XD0;
							CMD_RDY=0X90;
						}
						continue;
					}
					POINT_COLOR=BLUE;
					LCD_Clear(WHITE);//����
					LCD_ShowString(35,110,lcddev.width,lcddev.height,16,"Touch Screen Adjust OK!");//У�����
					delay_us(1000);
					TP_Save_Adjdata();
 					LCD_Clear(WHITE);//����
					return;//У�����
			}
		}
		delay_us(10000);
		outtime++;
		if(outtime>1000)
		{
			TP_Get_Adjdata();
			break;
	 	}
 	}
}

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
	void* edge_capture_ptr = (void*) &edge_capture;

//	int register_flag = 2;
//	alt_irq_register(
//		TOUCH_PEN_INTR_IRQ,
//		edge_capture_ptr,
//		intr_scan_headler );
	alt_ic_isr_register(
		  TOUCH_PEN_INTR_IRQ_INTERRUPT_CONTROLLER_ID,// �жϿ�������ţ���system.h����
		  TOUCH_PEN_INTR_IRQ,    // Ӳ���жϺţ���system.h����
	      intr_scan_headler ,             // �жϷ����Ӻ���
	      NULL,       // ָ�����豸����ʵ����ص����ݽṹ��
	      NULL);                  // flags������δ��
//	printf("%d\n", register_flag);
//	alt_irq_init(ALT_IRQ_BASE); //ʹ���ж�
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

//����ȡ�ĵ�ַ
//void intr_scan_headler(void* isr_context){
//	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(RT_PEN_BASE, 1);// ���жϱ��ز���Ĵ���
////	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(RT_PEN_BASE, 0);
//	int PEN = IORD(RT_PEN_BASE, 0);
//	PEN = PEN&0x00000001;
//	unsigned short tempx, tempy;
//
////			touchPosInst.touchLeaveFlag = false;
////			touchPosInst.touchComeFlag = false;
//	if(PEN == 0){
//		touchPosInst.touchComeFlag = true;
//
////		IOWR_ALTERA_AVALON_PIO_EDGE_CAP(RT_PEN_BASE, 0);// ���жϱ��ز���Ĵ���
//		TP_Read_XY2(&tempx, &tempy);//��ȡ��������
//		physical2logical(tempx, tempy, &(touchPosInst.pos_x), &(touchPosInst.pos_y));
//
//	}
//	else
//	{
//		touchPosInst.touchLeaveFlag = true;
//
////		IOWR_ALTERA_AVALON_PIO_EDGE_CAP(RT_PEN_BASE, 0);// ���жϱ��ز���Ĵ���
//
//	}
//	return;
//}

void intr_scan_headler(void* isr_context){

	int PEN = IORD(RT_PEN_BASE, 0);

	printf( "PEN=%d\n", PEN );

	usleep(10000);



	touchPosInst.touchComeFlag = true;

	unsigned short tempx, tempy;

	TP_Read_XY2(&tempx, &tempy);//��ȡ��������
	physical2logical(tempx, tempy, &(touchPosInst.pos_x), &(touchPosInst.pos_y));

	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(RT_PEN_BASE, 0x1);// ���жϱ��ز���Ĵ���

	return;
}