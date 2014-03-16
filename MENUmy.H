#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <dos.h>

#define FALSE 0
#define TRUE 	1

#define MM1 "File"
#define MM2 "Compress"
#define MM3 "Extract"


#define SM11 " Exit            Alt+X"

#define SM21 " Compress by RLE      F4"
#define SM22 " Compress by LZW      F5"
#define SM31 " Extract by RLE       F6"
#define SM32 " Extract by LZW       F7"

#define ESC 27
#define TAB 9
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define F1 59
#define F2 60
#define F3 61
#define F4 62
#define F5 63
#define F6 64
#define F7 65
#define F8 66
#define ALT_X 45
#define ENTER 13
#define DEL 83

#define x1sub1 2
#define	y1sub1 2
#define x2sub1 28
#define y2sub1 4

#define x1sub2 8
#define	y1sub2 2
#define x2sub2 36
#define y2sub2 6

#define x1sub3 18
#define y1sub3 2
#define x2sub3 44
#define y2sub3 6

#define x1save 20
#define y1save 12
#define x2save 60
#define y2save 16

int move(int,int,int);
void initwindow(int,int,int,int);
void submenuinit(int,int,int,int);
int  read_key();

int move(int Menu[],int i_max,int key)
{ int i,tmp,tmp2=-1;
	for (i=0;i<i_max;i++)
		if (Menu[i]==GREEN) tmp=i;
	switch(key)
	{
		case UP:   Menu[tmp]=WHITE;
					       tmp--;
					       if (tmp<0) tmp=i_max-1;
					       Menu[tmp]=GREEN;
					       return(tmp);

		case DOWN: Menu[tmp]=WHITE;
					       tmp++;
					       if (tmp>i_max-1) tmp=0;
					       Menu[tmp]=GREEN;
					       return(tmp);

		case RIGHT: Menu[tmp]=WHITE;
					       tmp++;
					       if (tmp>i_max-1) tmp=0;
					       Menu[tmp]=GREEN;
					       return(tmp);

		case LEFT:  Menu[tmp]=WHITE;
					       tmp--;
					       if (tmp<0)  tmp=i_max-1;
					       Menu[tmp]=GREEN;
					       return(tmp);
	}
	return(tmp2);
}

void initwindow(int x1,int y1,int x2,int y2)
{ int i;
	textbackground(BLUE);
	window(x1,y1,x2,y2);
	clrscr();
}

void submenuinit(int x1sub,int y1sub,int x2sub,int y2sub)
{
 textbackground(BLACK);
 window(x1sub+1,y1sub+1,x2sub+2,y2sub+1);
 clrscr();
 textbackground(WHITE);
 window(x1sub,y1sub,x2sub,y2sub);
 clrscr();
 window(x1sub+1,y1sub,x2sub-1,y2sub);
 textcolor(BLACK);
 for (int i=0;i<x2sub-x1sub-1;i++)
			cprintf("Ä");
 textcolor(BLACK);
 window(x1sub,y1sub,x2sub,y2sub);

}

int read_key()
{ int key;
	_AH=0x6;
	_DL=0xFF;
	geninterrupt(0x21);
	if (_AL==0) geninterrupt(0x21);
	key=_AL;
	return(key);
}
