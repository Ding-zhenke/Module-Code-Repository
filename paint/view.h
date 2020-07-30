#include "touch.h" 
#include "sys.h"
#include "key.h"

#ifndef __VIEW_H__
#define __VIEW_H__

char startview(void);
void interface(void);
void project1(void);//·¢Éä»ú
void project2(void);//É¨Æµ

void paint(int *p,u32 *fre,int length,u16 color,int max_location);
void showtwo(u16 x,u16 y,double num,u16 color);
#endif


