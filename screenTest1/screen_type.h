/*
 * screen_type.h
 *
 * 本库包含内容为：
 * 屏幕显示结构体
 *
 *  Created on: 2019年4月26日
 *      Author: 12757
 */

#ifndef SCREEN_TYPE_H_
#define SCREEN_TYPE_H_

#include <stdbool.h>
#include "alt_types.h"
#include "color.h"





//屏幕基本尺寸
#define SCR_WIDTH 320
#define SCR_HEIGHT 480

#define SCR_HEADER_HEIGHT 40
#define SCR_FOTTER_HEIGHT 60
#define SCR_MAIN_HEIGHT (SCR_HEIGHT - SCR_HEADER_HEIGHT - SCR_FOTTER_HEIGHT)



//默认中文字符大小
#define SCR_OBJ_SIZE_Y OBJ_Y_SIZE
#define SCR_WORD_SIZE_X GBK_X_SIZE
#define SCR_WORD_SIZE_Y SCR_OBJ_SIZE_Y
#define DEFAULT_WORD_SPACING 2//间距（纯中文间距，中英文之间间距）
//默认英文字母大小
#define SCR_LETTER_SIZE_X ASCII_X_SIZE
#define SCR_LETTER_SIZE_Y SCR_OBJ_SIZE_Y
#define DEFAULT_LETTER_SPACING 1 //间距（纯英文间距）


//电子书区域每一行的字节数
#define BOOK_ROW_BYTES 34 //min( 320/(16+2)*2, 320/(8+1) ),再向下取偶数
#define BOOK_ROW_HEIGHT (SCR_WORD_SIZE_Y + 2)
#define BOOK_ROW_WIDTH (( BOOK_ROW_BYTES >> 1) * ( SCR_WORD_SIZE_X + DEFAULT_WORD_SPACING ))
#define BOOK_COL_NUM 21 //SCR_MAIN_HEIGHT / BOOK_ROW_HEIGHT

//目录列表的元素高度
#define CATALOG_ROW_HEIGHT BOOK_ROW_HEIGHT

//默认颜色
#define DEFAULT_BORDER_COLOR 0xffff
#define DEFAULT_BKG_COLOR 0x0000
#define DEFAULT_OBJ_COLOR 0xffff


//默认电子书的颜色(256色)
#define DEFAULT_BOOK_BKG_COLOR 59
#define DEFAULT_BOOK_TEXT_COLOR 211





//屏幕编号
enum scrID { SCR_NONE = 0, SCR_HOME=1, SCR_BOOK=2, \
	SCR_SETTING=3, SCR_COLOR_PICKER = 4  };

//屏幕页面栈(用于保证能够回退到上一个屏幕页面
typedef struct
{
	enum scrID scrIDStack[4];//屏幕页面栈
	short curScrIndex;//当前屏幕在数组中的下标
} ScrMainAreaInfo;


//颜色变量
typedef struct
{
	color_u16 borderColor;//边界颜色
	color_u16 bkgColor;//背景颜色
	color_u16 objColor;//文本或图标颜色
	bool borderVisible;
	bool objVisible;
} ColorInfo;


//区域变量(x为横向坐标，y为纵向坐标）
// D = {(x,y)|x_min < x < x_max && y_min < y < y_max }
typedef struct
{
	short x_min;
	short x_max;
	short y_min;
	short y_max;
} AreaRange;

//位置结构体
typedef struct
{
    short x_pos;
    short y_pos;
} PosType;

//区域定位(通过中心点定位）
typedef struct
{
	short center;
	short half;
} AreaFmtCenter;//1维方向


//区域定位（通过边距定位）
typedef struct
{
	short margin_LU;//左或上
	short margin_RD;//右或下
} AreaFmtMargin;


enum areaFmtID { AREA_FMT_CENTER = 0, AREA_FMT_MARGIN=1 };

union areaFmtVal
{
	AreaFmtCenter fmtCenterVal;
	AreaFmtMargin fmtMarginVal;
};


//区域定位总类型
typedef struct
{
	enum areaFmtID fmtTypeX;
	enum areaFmtID fmtTypeY;
	union areaFmtVal fmtValX;
	union areaFmtVal fmtValY;
} AreaFmt;

//文本变量
typedef struct
{
	char *text;
	short textLen;
} TextType;

//文本列表
typedef struct
{
	TextType *textArray;
	short textNum;
} TextList;

//标签模块
typedef struct
{
	AreaRange textArea;

	ColorInfo tagColorInfo;
	short wordSpacing;//文字间距
	short letterSpacing;//字母间距
	short wordXSize;//汉字横向宽度
	short letterXSize;//字母横向宽度
	short objYSize;//文字纵向高度
	TextType tagText;
} TagBlock;

//静态标签+动态标签
typedef struct
{
	ColorInfo bkgBorderColor;

	AreaRange staticTagArea;
	TagBlock staticTag;

	AreaRange varTagArea;
	TagBlock varTag;
} TagGroup;


//图标模块
typedef struct
{
	AreaRange picArea;

	ColorInfo iconColorInfo;

	const alt_u8 *iconPic;
	short iconXSize;//icon横向宽度
	short iconYSize;//icon纵向高度
} IconBlock;

//数字加减图标+数字显示标签
typedef struct
{
	ColorInfo bkgBorderColor;

	AreaRange plusIconArea;
	IconBlock plusIcon;

	AreaRange minusIconArea;
	IconBlock minusIcon;

	AreaRange valueTagArea;
	TagBlock valueTag;

	AreaRange unitTagArea;//单位
	TagBlock unitTag;

	short value;
	short valueMax;
	short valueMin;
} TagIconGroup;

//单个txt文件的信息
typedef struct
{
	TextType txtFileReadName;//问件读取时需要用该文件名
	TextType txtFileName;//文件名（省略.txt）
	int curPageNum;//当前阅读页数
	int totalPageNum;//总页数
	float curPageRatio;//阅读进度比率 = curPageNum / totalPageNum  (范围0~1)
	unsigned int curPageBytePosStart;//本页开始字节数
	unsigned int curPageBytePosEnd;//本页结束字节数
	unsigned int fileSizeInBytes;//整个txt文件的字节数

} TxtFile;

//时间变量
typedef struct
{
    short timeHour;//阅读时间,时
	short timeMinute;//阅读时间,分
	short timeSecond;//秒（不参与显示，只用于时钟计数）
} TimeType;

//屏幕上边栏：时钟显示区域
typedef struct
{
	ColorInfo bkgBorderColor;

	AreaRange tagTimeArea;//时间标签区域
	TagBlock tagTime;//时间标签

    TimeType curTime;//当前时间
} ScreenHeaderTime;

//屏幕下边栏：基本按钮区
typedef struct
{
	ColorInfo bkgBorderColor;

	AreaRange iconSettingArea;//编辑键区域
	IconBlock iconSetting;//编辑键图标

	AreaRange iconHomeArea;//主页键区域
	IconBlock iconHome;//主页键图标

	AreaRange iconBackArea;//返回键区域（退回上一级屏幕）
	IconBlock iconBack;//返回键图标

} ScreenFooterBtn;

//list（垂直列表）由多个elem（Tag元素）组成，elem为一个长方形框，里面有背景颜色，字符串，边界
typedef struct
{
	AreaRange elemBaseArea;//元素尺寸
	AreaRange elemTextArea;//元素中文本区位置
	short wordSpacing;//文字间距
	short letterSpacing;//字母间距

	ColorInfo elemColorInfo;//元素颜色信息

	short elemNum;//列表元素总个数

	TagBlock *elemBlock;//列表元素显示信息(数组)

} TagList;

//垂直滚动条
typedef struct
{
	ColorInfo barYColorInfo;

	AreaRange iconMoveUpArea;//上移键
	IconBlock iconMoveUp;//上移键图标

	AreaRange iconMoveDownArea;//下移键
	IconBlock iconMoveDown;//下移键图标

	AreaRange barBaseArea;//滚动条基本区域（偏移量为0）
	IconBlock iconBar;//滚动条图标

	AreaRange barMoveArea;//滚动条可以运动的区域(用于点击该区域，则滚动条会相应的移动)
	short barYoffset;//滚动条纵向偏移量
	short barYOffsetMax;
	short barHeight;//滚动条高度
	bool iconBarVisible;//滚动条是否可见


} ScrollBarY;

//进度条
typedef struct
{
	ColorInfo bkgBorderColor;

	AreaRange axisArea;//运动轴（虽然叫运动轴，但它不运动，而是另外一个点在它上面运动）
	IconBlock axis;

	AreaRange dotBaseArea;//进度点基本区域（无偏移量时）
	IconBlock dot;

	AreaRange clickArea;//用户点击区，点击该区便可以使进度点移动

	short xOffset;
	short xOffsetMax;

} ProgressBarX;

//屏幕一：电子书书名页面(HOME)
typedef struct
{
	ColorInfo bkgBorderColor;

	short listYOffset;//列表显示时的纵向偏移量
	short listYOffsetMax;//列表显示时的纵向偏移量最大值
	short listYSize;//列表纵向全长
	short listYStepSize;//列表纵向移动步长
	short listShowHeight;//显示在屏幕上的列表的高度（即列表AreaRange的高度)
	float listShowYRatio;//显示在屏幕上的列表占总列表的比率

	AreaRange numBarArea;//数字编号条
	TagList numBar;

	AreaRange txtCatalogArea;//txt文件名
	TagList txtCatalog;

	bool txtScrollable;//某行允许水平滚动否
	short txtScrollrow;//可滚动的某行行数（一次只有一行可以滚动）
	short txtScrollXOffset;//当前滚动偏移量
	short txtScrollXStepSize;//水平滚动步长
	short txtScrollwidth;//滚动文本的真实像素宽度（可以超出屏幕宽度）
	short txtScrollSpacing;//滚动文本循环时文本尾与新文本头的间隔

	AreaRange turnCatalogBarArea;//文件上下翻动条
	ScrollBarY turnCatalogBarY;

} ScreenHome;



//屏幕二的悬浮窗：进度条区域
typedef struct
{
	ColorInfo bkgBorderColor;

	AreaRange prgBarArea;//水平进度条
	ProgressBarX prgBarX;

	AreaRange tagPagePercTageArea;//"百分数进度"（标签组），格式为"53.33%"
	TagBlock tagPagePercTage;

	AreaRange tagPageNumArea;//页数标签，格式为：" 1234/99999", 1234为当前页数，99999为总页数
	TagBlock tagPageNum;

	//只读txtFileInfo指向的结构体变量
	TxtFile *txtFileInfo;//指向的结构体里面有阅读进度信息，还有当前打开文件的文件名

} ScreenPageInfo;


//屏幕二：电子书txt文本页面（BOOK）
typedef struct
{
	ColorInfo bookColorInfo;
	//注：ColorInfo中的 "背景颜色","字体颜色" 可编辑，颜色源信息在SEETING页面内存储

	AreaRange txtBookArea;//电子书区域
	AreaRange txtBookNarrowArea;//电子书小区域，当页面进度信息显示时，电子书区域下方会被覆盖一部分，范围减小
	TagList txtBook;

	AreaRange turnBackPageAreaPos;//点击该区域（左），翻到上一页
	AreaRange turnNextPageAreaPos;//点击该区域（右），翻到下一页
	AreaRange openPageInfoAreaPos;//点击该区域（中），打开或关闭页数进度条窗口

	AreaRange pageInfoArea;
	ScreenPageInfo pageInfo;
	bool pageInfoVisible;

} ScreenBook;

//屏幕三：电子书设置页面（SETTING ）
typedef struct
{
	ColorInfo bkgBorderColor;

	AreaRange tag1Area;//"休息定时"（闹钟定时提醒读者休息）
	TagBlock tag1;

	AreaRange editAlarmHourArea;//编辑休息定时：时
	TagIconGroup editAlarmHour;

	AreaRange editAlarmMinuteArea;//编辑休息定时：分
	TagIconGroup editAlarmMinute;

	AreaRange editBkgColorArea;//编辑背景颜色
	TagGroup editBkgColor;

	AreaRange editWordColorArea;//编辑文本颜色
	TagGroup editWordColor;

	AreaRange editTurnPageModArea;//自动翻页与手动翻页切换
	TagGroup editTurnPageMod;

	AreaRange tag2Area;//"翻页定时"
	TagBlock tag2;

	AreaRange editTurnPageSecArea;//编辑自动翻页时间间隔
	TagIconGroup editTurnPageSec;

    bool autoTurnPage;//翻页模式(ture表示自动翻页，false表示手动翻页)

	//电子书BOOK页面的颜色信息
	color_u8 bkgColorIndex;//背景颜色(因为要编辑该颜色，所以设置为256色)
	color_u8 txtColorIndex;//字体颜色(因为要编辑该颜色，所以设置为256色)

} ScreenSetting;

//颜色表（16*16=256色)
typedef struct
{
	//每个色块
	short colorXSize;//横向长度
	short colorYSize;//纵向长度
	//色块个数
	short colorXNum;//横向颜色个数
	short colorYNum;//纵向颜色个数

	//256色->16位色，颜色映射表
	const color_u16 *colorSpace;

	//选中的颜色在映射表中的位置（数据在指针指向的color_u8类型变量中）
	color_u8 *colorIndex;

} ColorBoard;

//屏幕四：电子书拾色器页面（COLOR_PICKER）
typedef struct
{
	ColorInfo bkgBorderColor;

	AreaRange colorPickerArea;//取色板

	ColorBoard colorPicker;

} ScreenColorPicker;





typedef struct
{
	AreaRange screenArea;//整个屏幕范围（320*480）（绝对位置）

	AreaRange headerArea;//顶栏
	ScreenHeaderTime scrHeader;

	AreaRange footerArea;//底栏
	ScreenFooterBtn scrFooter;

	AreaRange mainArea;//主栏(主栏为一下4个屏幕页面的共用页面)
	ScreenHome scrHome;
	ScreenBook scrBook;
	ScreenSetting scrSetting;
	ScreenColorPicker scrColorPicker;

	//主栏页面信息
	ScrMainAreaInfo mainAreaInfo;

} ScreenContainer;//容器

#endif /* SCREEN_H_ */
