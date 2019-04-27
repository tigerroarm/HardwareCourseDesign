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

#include "stdbool.h"

#define SRC_WORD_SIZE_X 16
#define SRC_WORD_SIZE_Y 16

//屏幕编号
enum scrID { SCR_NONE = 0, SCR_HOME=1, SCR_BOOK=2, \
	SCR_SETTING=3, SCR_COLOR_PICKER = 4  };

//屏幕页面栈(用于保证能够回退到上一个屏幕页面
typedef struct
{
	short scrIDStack[4];//屏幕页面栈
	short curScrIndex;//当前屏幕在数组中的下标
} ScrMainAreaInfo;

//翻页模式
enum turnPageMode { TURN_PAGE_MANUAL=0, TURN_PAGE_AUTO=1};




typedef unsigned short color_u16;

//颜色变量
typedef struct
{
	color_u16 borderColor;//边界颜色
	color_u16 bkgColor;//背景颜色
	color_u16 objColor;//文本或图标颜色
} ColorInfo;

typedef struct
{
	unsigned char redVal;  //5bit [15:11]
	unsigned char greenVal;//6bit [10: 5]
	unsigned char blueVal; //5bit [4 : 0]
} ColorRGB;


//区域变量(x为横向坐标，y为纵向坐标）
// D = {(x,y)|x_min < x < x_max && y_min < y < y_max }
typedef struct
{
	short x_min;
	short x_max;
	short y_min;
	short y_max;
} AreaRange;

//文本变量
typedef struct
{
	char *text;
	short textSize;
} TextType;


//标签模块
typedef struct
{
	AreaRange textArea;
	bool borderVisible;
	bool textVisible;

	ColorInfo tagColorInfo;
	short wordSpacing;//文字间距
	TextType tagText;
} TagBlock;

//静态标签+动态标签
typedef struct
{
	bool borderVisible;
	color_u16 borderColor;//边界颜色
	color_u16 bkgColor;//背景颜色

	AreaRange staticTagArea;
	TagBlock staticTag;

	AreaRange varTagArea;
	TagBlock varTag;
} TagBlockGroup;


//图标模块
typedef struct
{
	AreaRange picArea;
	bool borderVisible;
	bool picVisible;
	ColorInfo tagColorInfo;
	unsigned char *iconPic;
} IconBlock;

//数字加减图标+数字显示标签
typedef struct
{
	bool borderVisible;
	color_u16 borderColor;//边界颜色
	color_u16 bkgColor;//背景颜色

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


//屏幕上边栏：时钟显示区域
typedef struct
{
	bool borderVisible;
	color_u16 borderColor;//边界颜色
	color_u16 bkgColor;//背景颜色

	AreaRange tagTimeArea;//时间标签区域
	TagBlock tagTime;//时间标签

	short timeHour;//阅读时间,时
	short timeMinute;//阅读时间,分
	short timeSecond;//秒（不参与显示，只用于时钟计数）
} ScreenHeaderTime;

//屏幕下边栏：基本按钮区
typedef struct
{
	bool borderVisible;
	color_u16 borderColor;//边界颜色
	color_u16 bkgColor;//背景颜色

	AreaRange iconSettingArea;//编辑键区域
	IconBlock iconSetting;//编辑键图标

	AreaRange iconHomeArea;//主页键区域
	IconBlock iconHome;//主页键图标

	AreaRange iconBackArea;//返回键区域（退回上一级屏幕）
	IconBlock iconBack;//返回键图标

} ScreenFooterBtn;

//list（垂直列表）由多个elem（元素）组成，elem为一个长方形框，里面有背景颜色，字符串，边界
typedef struct
{
	AreaRange elemArea;//元素尺寸
	AreaRange textArea;//元素中文本区位置
	short wordSpacing;//文字间距
	bool elemBorderVisible;

	ColorInfo elemColorInfo;//元素颜色信息

	TextType *elemText;//列表元素文本数组

	short elemNum;//列表元素总个数
} VirtualList;

//垂直滚动条
typedef struct
{
	AreaRange iconMoveUpArea;//上移键
	IconBlock iconMoveUp;//上移键图标

	AreaRange iconMoveDownArea;//下移键
	IconBlock iconMoveDown;//下移键图标

	AreaRange iconBarMoveArea;//滚动条移动范围
	AreaRange iconBarArea;//滚动条
	IconBlock iconBar;//滚动条图标

} ScrollBarY;

//进度条
typedef struct
{
	AreaRange axisArea;//运动轴
	IconBlock iconAxis;

	AreaRange dotMoveArea;//进度点移动范围
	AreaRange dotArea;
	IconBlock dot;

} ProgressBar;

//屏幕一：电子书书名页面(HOME)
typedef struct
{
	bool borderVisible;
	color_u16 borderColor;//边界颜色
	color_u16 bkgColor;//背景颜色

	AreaRange realHomePageArea;//主页面滚动区显示区在虚拟页面中的区域

	AreaRange numberBarArea;//数字编号条
	VirtualList numberBar;

	AreaRange txtContentArea;//txt文件名
	VirtualList txtContent;
	bool txtScrollable;//允许滚动否
	short txtScrollrow;//可滚动的行数（一次只有一行可以滚动）

	AreaRange turnContentBarArea;//文件上下翻动条
	ScrollBarY turnContentBarY;

} ScreenHome;


//屏幕二的悬浮窗：进度条区域
typedef struct
{
	bool borderVisible;
	color_u16 borderColor;//边界颜色
	color_u16 bkgColor;//背景颜色

	AreaRange prgBarArea;//水平进度条
	ProgressBar prgBarX;


	AreaRange tagCurPageArea;//"当前阅读页数"（标签组）
	TagBlockGroup tagCurPage;


	AreaRange tagTotalPageArea;//"总页数"（标签组）
	TagBlockGroup tagTotalPage;

	AreaRange tagCurPagePctArea;//"进度"（标签组）
	TagBlockGroup tagCurPagePct;

	int curPageNum;//当前阅读页数
	int totalPageNum;//总页数
	float curPagePercent;//阅读进度百分数(%)

} ScreenPageInfo;


//屏幕二：电子书txt文本页面（BOOK）
typedef struct
{
	bool borderVisible;
	color_u16 borderColor;//边界颜色
	color_u16 bkgColor;//背景颜色

	AreaRange txtBookArea;//电子书区域
	VirtualList txtBook;

	AreaRange tureBackPageAreaPos;//点击该区域（左），翻到上一页
	AreaRange tureNextPageAreaPos;//点击该区域（右），翻到下一页
	AreaRange txtPageInfoAreaPos;//点击该区域（中），打开或关闭页数进度条窗口

	AreaRange pageInfoArea;
	ScreenPageInfo pageInfo;
	bool pageInfoVisable;

} ScreenBook;

//屏幕三：电子书设置页面（SETTING ）
typedef struct
{
	bool borderVisible;
	color_u16 borderColor;//边界颜色
	color_u16 bkgColor;//背景颜色

	AreaRange tag1Area;//"休息定时"（闹钟定时提醒读者休息）
	TagBlock tag1;

	AreaRange editAlarmHourArea;//编辑休息定时：时
	TagIconGroup tagAlarmHour;

	AreaRange editAlarmMinuteArea;//编辑休息定时：分
	TagIconGroup tagAlarmMinute;

	AreaRange editBkgColorArea;//编辑背景颜色
	TagBlockGroup editBkgColor;

	AreaRange editWordColorArea;//编辑文本颜色
	TagBlockGroup editWordColor;

	AreaRange editTurnPageModArea;//自动翻页与手动翻页切换
	TagBlockGroup editTurnPageMod;

	AreaRange tag2Area;//"翻页定时"
	TagBlock tag2;

	AreaRange editTurnPageSecArea;//编辑自动翻页时间间隔
	TagIconGroup editTurnPageSec;

	bool turnPageMod;//翻页模式

} ScreenSetting;

//屏幕四：电子书拾色器页面（COLOR_PICKER）（目前保留，暂时不做）
typedef struct
{
	bool borderVisible;
	color_u16 borderColor;//边界颜色
	color_u16 bkgColor;//背景颜色

	AreaRange areaAbsPos;//本区域位置(绝对位置)
	short wordSpacing;//文字间距
	short lineSpacing;//行距

	AreaRange redBarArea;
	ProgressBar redBarY;

	AreaRange greenBarArea;
	ProgressBar greenBarY;

	AreaRange blueBarArea;
	ProgressBar blueBarY;

	//三基色的值
	ColorRGB colorValue;

	AreaRange tag1Area;//"点击色块确认"
	TagBlock tag1;

	//调色板调出的颜色
	AreaRange tagMixColorArea;//调色板区域
	TagBlock tagMixColor;//调色板,文字采用"调色板",颜色采用混合得到的颜色

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
