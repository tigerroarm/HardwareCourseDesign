/*
 * main.c
 *
 *  Created on: 2019Äê5ÔÂ9ÈÕ
 *      Author: lbl
 */
#include <stdio.h>
#include "touch.h"
#include <math.h>

unsigned char adjdata[14];
//extern _m_tp_dev tp_dev;

int main()
{
  printf("Hello from Nios II test touch!\n");
  printf("begin to initialize\n");
  TP_Init();
  unsigned short x, y;
  int counter = 0;
  int curTime = 0;
  printf("after initialize\n");


  while(1)
  {
	  if( touchPosInst.touchComeFlag )
	  {
		  touchPosInst.touchComeFlag = false;
		  printf("touch come\n" );
		  printf("x:%d y:%d\n",touchPosInst.pos_x, touchPosInst.pos_y);
	  }
	  if ( touchPosInst.touchLeaveFlag )
	  {
		  touchPosInst.touchLeaveFlag = false;
		  printf("touch leave\n" );
	  }
  }


  while(1){
//	  if(counter%300000==0){
//		 int PEN = IORD(RT_PEN_BASE, 0);
////	 	 PEN = PEN&0x00000001;
////	 	 intr_scan(&x, &y);
////	 	 printf("%d:%d %d %d\n ", curTime, PEN, x, y);
//	 	printf("%d:%d\n ", curTime, PEN);
//	 	curTime ++;
//	  }
//	 counter++;
	 /* if(tp_dev.x[0] == 0xffff){

	  }else{
		  physical2logical(tp_dev.x[0], tp_dev.y[0], &x, &y);
		  printf("physical coordinate x: %u, y: %u\n", tp_dev.x[0],tp_dev.y[0]);
		  printf("logical coordinate x: %u, y: %u\n", x,y);
	  }
	*/
  }


  return 0;
}

