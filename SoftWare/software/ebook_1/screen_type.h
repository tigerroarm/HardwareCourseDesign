/*
 * screen_type.h
 *
 * �����������Ϊ��
 * ��Ļ��ʾ�ṹ��
 *
 *  Created on: 2019��4��26��
 *      Author: 12757
 */

#ifndef SCREEN_TYPE_H_
#define SCREEN_TYPE_H_

#include "stdbool.h"
#include "alt_types.h"
#include "color.h"




//��Ļ�����ߴ�
#define SCR_WIDTH 320
#define SCR_HEIGHT 480

#define SCR_HEADER_HEIGHT 40
#define SCR_FOTTER_HEIGHT 60
#define SCR_MAIN_HEIGHT (SCR_HEIGHT - SCR_HEADER_HEIGHT - SCR_FOTTER_HEIGHT)



//Ĭ�������ַ���С
#define SRC_WORD_SIZE_X GBK_X_SIZE
#define SRC_WORD_SIZE_Y GBK_Y_SIZE
#define DEFAULT_WORD_SPACING 2//��ࣨ�����ļ�࣬��Ӣ��֮���ࣩ
//Ĭ��Ӣ����ĸ��С
#define SRC_LETTER_SIZE_X ASCII_X_SIZE
#define SRC_LETTER_SIZE_Y ASCII_Y_SIZE
#define DEFAULT_LETTER_SPACING 1 //��ࣨ��Ӣ�ļ�ࣩ


//����������ÿһ�е��ֽ���
#define BOOK_ROW_BYTES 34 //min( 320/(16+2)*2, 320/(8+1) ),������ȡż��
#define BOOK_ROW_HEIGHT (SRC_WORD_SIZE_Y + 2)
#define BOOK_COL_NUM 21 //SCR_MAIN_HEIGHT / BOOK_ROW_HEIGHT

//Ŀ¼�б���Ԫ�ظ߶�
#define CATALOG_ROW_HEIGHT BOOK_ROW_HEIGHT

//Ĭ����ɫ
#define DEFAULT_BORDER_COLOR 0x0000
#define DEFAULT_BKG_COLOR 0x1234
#define DEFAULT_OBJ_COLOR 0xffff








//��Ļ���
enum scrID { SCR_NONE = 0, SCR_HOME=1, SCR_BOOK=2, \
	SCR_SETTING=3, SCR_COLOR_PICKER = 4  };

//��Ļҳ��ջ(���ڱ�֤�ܹ����˵���һ����Ļҳ��
typedef struct
{
	enum scrID scrIDStack[4];//��Ļҳ��ջ
	short curScrIndex;//��ǰ��Ļ�������е��±�
} ScrMainAreaInfo;

////��ҳģʽ
//enum turnPageMode { TURN_PAGE_MANUAL=0, TURN_PAGE_AUTO=1};


//��ɫ����
typedef struct
{
	color_u16 borderColor;//�߽���ɫ
	color_u16 bkgColor;//������ɫ
	color_u16 objColor;//�ı���ͼ����ɫ
} ColorInfo;



//�������(xΪ�������꣬yΪ�������꣩
// D = {(x,y)|x_min < x < x_max && y_min < y < y_max }
typedef struct
{
	short x_min;
	short x_max;
	short y_min;
	short y_max;
} AreaRange;

//����λ(ͨ�����ĵ㶨λ��
typedef struct
{
	short center;
	short half;
} AreaFmtCenter;//1ά����


//����λ��ͨ���߾ඨλ��
typedef struct
{
	short margin_LU;//�����
	short margin_RD;//�һ���
} AreaFmtMargin;


enum areaFmtID { AREA_FMT_CENTER = 0, AREA_FMT_MARGIN=1 };

union areaFmtVal
{
	AreaFmtCenter fmtCenterVal;
	AreaFmtMargin fmtMarginVal;
};


//����λ������
typedef struct
{
	enum areaFmtID fmtTypeX;
	enum areaFmtID fmtTypeY;
	union areaFmtVal fmtValX;
	union areaFmtVal fmtValY;
} AreaFmt;

//�ı�����
typedef struct
{
	char *text;
	short textLen;
} TextType;

typedef struct
{
	TextType *textArray;
	short textNum;
} TextList;

//��ǩģ��
typedef struct
{
	AreaRange textArea;
	bool borderVisible;
	bool textVisible;

	ColorInfo tagColorInfo;
	short wordSpacing;//���ּ��
	short letterSpacing;//��ĸ���
	short lineSpacing;//�о�
	TextType tagText;
} TagBlock;

//��̬��ǩ+��̬��ǩ
typedef struct
{
	bool borderVisible;
	color_u16 borderColor;//�߽���ɫ
	color_u16 bkgColor;//������ɫ

	AreaRange staticTagArea;
	TagBlock staticTag;

	AreaRange varTagArea;
	TagBlock varTag;
} TagBlockGroup;


//ͼ��ģ��
typedef struct
{
	AreaRange picArea;
	bool borderVisible;
	bool picVisible;
	ColorInfo iconColorInfo;
	const alt_u8 *iconPic;
	short iconWidth;
	short iconXSizeInByte;//icon�����ֽ���
	short iconYSizeInByte;//icon�����ֽ�����ͬʱҲΪicon�ĸ߶�
} IconBlock;

//���ּӼ�ͼ��+������ʾ��ǩ
typedef struct
{
	bool borderVisible;
	color_u16 borderColor;//�߽���ɫ
	color_u16 bkgColor;//������ɫ

	AreaRange plusIconArea;
	IconBlock plusIcon;

	AreaRange minusIconArea;
	IconBlock minusIcon;

	AreaRange valueTagArea;
	TagBlock valueTag;

	AreaRange unitTagArea;//��λ
	TagBlock unitTag;

	short value;
	short valueMax;
	short valueMin;
} TagIconGroup;


//��Ļ�ϱ�����ʱ����ʾ����
typedef struct
{
	bool borderVisible;
	color_u16 borderColor;//�߽���ɫ
	color_u16 bkgColor;//������ɫ

	AreaRange tagTimeArea;//ʱ���ǩ����
	TagBlock tagTime;//ʱ���ǩ

	short timeHour;//�Ķ�ʱ��,ʱ
	short timeMinute;//�Ķ�ʱ��,��
	short timeSecond;//�루��������ʾ��ֻ����ʱ�Ӽ�����
} ScreenHeaderTime;

//��Ļ�±�����������ť��
typedef struct
{
	bool borderVisible;
	color_u16 borderColor;//�߽���ɫ
	color_u16 bkgColor;//������ɫ

	AreaRange iconSettingArea;//�༭������
	IconBlock iconSetting;//�༭��ͼ��

	AreaRange iconHomeArea;//��ҳ������
	IconBlock iconHome;//��ҳ��ͼ��

	AreaRange iconBackArea;//���ؼ������˻���һ����Ļ��
	IconBlock iconBack;//���ؼ�ͼ��

} ScreenFooterBtn;

//list����ֱ�б����ɶ��elem��TagԪ�أ���ɣ�elemΪһ�������ο������б�����ɫ���ַ������߽�
typedef struct
{
	AreaRange elemBaseArea;//Ԫ�سߴ�
	AreaRange textArea;//Ԫ�����ı���λ��
	short wordSpacing;//���ּ��
	short letterSpacing;//��ĸ���
	bool elemBorderVisible;

	ColorInfo elemColorInfo;//Ԫ����ɫ��Ϣ

	short elemNum;//�б�Ԫ���ܸ���

	AreaRange *elemArea;//�б�Ԫ���������飩
	TagBlock *elemBlock;//�б�Ԫ����ʾ��Ϣ(����)

} TagList;

//��ֱ������
typedef struct
{
	bool borderVisible;
	color_u16 borderColor;//�߽���ɫ
	color_u16 bkgColor;//������ɫ

	AreaRange iconMoveUpArea;//���Ƽ�
	IconBlock iconMoveUp;//���Ƽ�ͼ��

	AreaRange iconMoveDownArea;//���Ƽ�
	IconBlock iconMoveDown;//���Ƽ�ͼ��

	AreaRange barBaseArea;//��������������ƫ����Ϊ0��
	short barYoffset;//����������ƫ����
	short barYOffsetMax;
	short barHeight;//�������߶�
	short barMoveHeight;//�������˶����Ƿ�Χ�ĸ߶�
	IconBlock iconBar;//������ͼ��

} ScrollBarY;

//������
typedef struct
{
	bool borderVisible;
	color_u16 borderColor;//�߽���ɫ
	color_u16 bkgColor;//������ɫ

	AreaRange axisArea;//�˶���
	IconBlock axis;

	AreaRange dotBaseArea;//���ȵ����������ƫ����ʱ��
	IconBlock dot;

	short xOffset;
	short xOffsetMax;

} ProgressBar;

//��Ļһ������������ҳ��(HOME)
typedef struct
{
	bool borderVisible;
	color_u16 borderColor;//�߽���ɫ
	color_u16 bkgColor;//������ɫ

	short listYOffset;//�б���ʾʱ������ƫ����
	short listYOffsetMax;//�б���ʾʱ������ƫ�������ֵ
	short listYSize;//�б�����ȫ��
	short listYStepSize;//�б������ƶ�����

	AreaRange numBarArea;//���ֱ����
	TagList numBar;

	AreaRange txtCatalogArea;//txt�ļ���
	TagList txtCatalog;

	bool txtScrollable;//ĳ������ˮƽ������
	short txtScrollrow;//�ɹ�����ĳ��������һ��ֻ��һ�п��Թ�����
	short txtScrollxOffset;//��ǰ����ƫ����
	short txtScrollwidth;//�����ı�����ʵ���ؿ��ȣ����Գ�����Ļ���ȣ�
	short txtScrollSpacing;//�����ı�ѭ��ʱ�ı�β�����ı�ͷ�ļ��

	AreaRange turnCatalogBarArea;//�ļ����·�����
	ScrollBarY turnCatalogBarY;

} ScreenHome;



//��Ļ����������������������
typedef struct
{
	bool borderVisible;
	color_u16 borderColor;//�߽���ɫ
	color_u16 bkgColor;//������ɫ

	AreaRange prgBarArea;//ˮƽ������
	ProgressBar prgBarX;

	AreaRange tagPagePercTageArea;//"�ٷ�������"����ǩ�飩����ʽΪ"53.33%"
	TagBlock tagPagePercTage;

	AreaRange tagPageNumArea;//ҳ����ǩ����ʽΪ��" 1234/99999", 1234Ϊ��ǰҳ����99999Ϊ��ҳ��
	TagBlock tagPageNum;

	int curPageNum;//��ǰ�Ķ�ҳ��
	int totalPageNum;//��ҳ��
	float curPagePercent;//�Ķ����Ȱٷ���(%)

} ScreenPageInfo;


//��Ļ����������txt�ı�ҳ�棨BOOK��
typedef struct
{
	bool borderVisible;
	color_u16 borderColor;//���±߽���ɫ

	color_u8 bkgColorIndex;//������ɫ(��ΪҪ�༭����ɫ����������Ϊ256ɫ)
	color_u8 txtColorIndex;//������ɫ(��ΪҪ�༭����ɫ����������Ϊ256ɫ)

	AreaRange txtBookArea;//����������
	TagList txtBook;

	AreaRange turnBackPageAreaPos;//����������󣩣�������һҳ
	AreaRange turnNextPageAreaPos;//����������ң���������һҳ
	AreaRange openPageInfoAreaPos;//����������У����򿪻�ر�ҳ������������

	AreaRange pageInfoArea;
	ScreenPageInfo pageInfo;
	bool pageInfoVisable;

} ScreenBook;

//��Ļ��������������ҳ�棨SETTING ��
typedef struct
{
	bool borderVisible;
	color_u16 borderColor;//�߽���ɫ
	color_u16 bkgColor;//������ɫ

	AreaRange tag1Area;//"��Ϣ��ʱ"�����Ӷ�ʱ���Ѷ�����Ϣ��
	TagBlock tag1;

	AreaRange editAlarmHourArea;//�༭��Ϣ��ʱ��ʱ
	TagIconGroup editAlarmHour;

	AreaRange editAlarmMinuteArea;//�༭��Ϣ��ʱ����
	TagIconGroup editAlarmMinute;

	AreaRange editBkgColorArea;//�༭������ɫ
	TagBlockGroup editBkgColor;

	AreaRange editWordColorArea;//�༭�ı���ɫ
	TagBlockGroup editWordColor;

	AreaRange editTurnPageModArea;//�Զ���ҳ���ֶ���ҳ�л�
	TagBlockGroup editTurnPageMod;

	AreaRange tag2Area;//"��ҳ��ʱ"
	TagBlock tag2;

	AreaRange editTurnPageSecArea;//�༭�Զ���ҳʱ����
	TagIconGroup editTurnPageSec;


	bool turnPageMod;//��ҳģʽ

} ScreenSetting;

//��ɫ����16*16=256ɫ)
typedef struct
{
	//ÿ��ɫ��
	short colorXSize;//���򳤶�
	short colorYSize;//���򳤶�
	//ɫ�����
	short colorXNum;//������ɫ����
	short colorYNum;//������ɫ����

	//256ɫ->16λɫ����ɫӳ���
	const color_u16 *colorSpace;

	//ѡ�е���ɫ��ӳ����е�λ��
	color_u8 *colorIndex;

} ColorTable;

//��Ļ�ģ�������ʰɫ��ҳ�棨COLOR_PICKER����Ŀǰ��������ʱ������
typedef struct
{
	bool borderVisible;//���±߽磨���������ң�
	color_u16 borderColor;//���±߽���ɫ�����������ң�
	color_u16 bkgColor;//������ɫ

	AreaRange colorPickerArea;//ȡɫ��

	ColorTable colorPicker;

} ScreenColorPicker;





typedef struct
{
	AreaRange screenArea;//������Ļ��Χ��320*480��������λ�ã�

	AreaRange headerArea;//����
	ScreenHeaderTime scrHeader;

	AreaRange footerArea;//����
	ScreenFooterBtn scrFooter;

	AreaRange mainArea;//����(����Ϊһ��4����Ļҳ��Ĺ���ҳ��)
	ScreenHome scrHome;
	ScreenBook scrBook;
	ScreenSetting scrSetting;
	ScreenColorPicker scrColorPicker;

	//����ҳ����Ϣ
	ScrMainAreaInfo mainAreaInfo;

} ScreenContainer;//����

#endif /* SCREEN_H_ */