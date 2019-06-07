/*
 * touch_resp.h
 *
 * ����ʵ�ֶԴ�����������Ӧ��������Ӧ�ĺ�������
 *
 *  Created on: 2019��4��27��
 *      Author: 12757
 */

#ifndef TOUCH_RESP_H_
#define TOUCH_RESP_H_

#include <stdbool.h>
#include "alt_types.h"
#include "screen_type.h"




//�жϴ���������������ͨ�Žṹ��

typedef struct
{
    bool touchIntrStatus;//�����жϿ����رյ�״̬��¼

    bool touchOccur;//����������־
    bool scrTaskBusy;//��������Ļ����æ��־(������æ��ʱ�򣬴����жϺ���ɶҲ�������ԹԵȴ���

    PosType touchPos;
} TouchTaskInfo;

extern TouchTaskInfo touchPosFlag;


//�жϴ���λ���Ƿ��ھ���������
bool posValidInAbsPos( PosType touchPos, AreaRange *absPos );

//�������Դ���λ�ý�����Ӧ����
bool screenRespTask( ScreenContainer *screenInst );

//�ϱ����Դ���λ�ý�����Ӧ����
bool scrHeaderResp( ScreenHeaderTime *scrHeaderInst,\
                        AreaRange *absPos, PosType touchPos );

//�±����Դ���λ�ý�����Ӧ����
bool scrFooterResp( ScreenFooterBtn *scrFooterInst, ScrMainAreaInfo *mainAreaInfo,\
                        AreaRange *absPos, PosType touchPos );


//����home�Դ���λ�ý�����Ӧ����
bool scrMainHomeResp( ScreenHome *scrHomeInst, ScrMainAreaInfo *mainAreaInfo,\
                        AreaRange *absPos, PosType touchPos, ScreenBook *scrBookInst );

//����book�Դ���λ�ý�����Ӧ����
bool scrMainBookResp( ScreenBook *scrBookInst, ScrMainAreaInfo *mainAreaInfo,\
                        AreaRange *absPos, PosType touchPos );

//����setting�Դ���λ�ý�����Ӧ����
bool scrMainSettingResp( ScreenSetting *scrSettingInst, ScrMainAreaInfo *mainAreaInfo,\
                        AreaRange *absPos, PosType touchPos, ScreenColorPicker *scrColorPickerInst );

//����colorPicker�Դ���λ�ý�����Ӧ����
bool scrMainColorPickerResp( ScreenColorPicker *scrColorPickerInst, ScrMainAreaInfo *mainAreaInfo,\
                        AreaRange *absPos, PosType touchPos );


//ColorBoard�Դ���λ�ý�����Ӧ����
bool colorBoardResp( ColorBoard *colorPicker, AreaRange *absPos, PosType touchPos );

//ҳ�������Ϣ�Դ���λ�ý�����Ӧ����
bool scrPageInfoResp( ScreenPageInfo *scrPageInfoInst, AreaRange *absPos, PosType touchPos );


//ҳ����Ⱥ���������Դ���λ�ý�����Ӧ����(����ֵΪtrue��ʾ�����˽������������Ϊfalse��ʾû�а�����
bool prgBarXResp( ProgressBarX *prgBarXInst, AreaRange *absPos, PosType touchPos );

//����Ŀ¼��������Ӧ����
bool scrollBarYResp( ScrollBarY *scrollBarYInst, AreaRange *absPos, PosType touchPos, ScreenHome *scrHomeInst );

#endif /* TOUCH_RESP_H_ */
