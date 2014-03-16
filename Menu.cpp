#include "conio.h"
#include "stdio.h"
#include "string.h"
#include "dos.h"

#include "menumy.h"
#include "C_rle.cpp"
#include "LZW.cpp"

void menuwindow(int,int,int,int);
void mainmenu(int);
void submenu(int);
void viewmenu(int);
void viewsubmenu(int,int,int,int,int);
void quit(int);
void Comp(void);

int MMenu[3],cd;
char *menu[3]={{MM1},{MM2},{MM3}};
int x=strlen(menu[0])+4, y=x+strlen(menu[1])+2;

void main()
{
	textbackground(BLACK);
	clrscr();
	initwindow(1,1,80,25);
	mainmenu(-1);
}

void viewmenu(int i)
{
	textbackground(MMenu[i]);
	if (i==0) { gotoxy(2,1); cprintf(menu[i]);}
	if (i==1) { gotoxy(x,1); cprintf(menu[i]);}
	if (i==2) { gotoxy(y,1); cprintf(menu[i]);}
}

void menuwindow(int x1,int y1,int x2,int y2)
{
	textbackground(WHITE);
	window(x1,y1,x2,y2);
	clrscr();

}

void mainmenu(int num)
{
	int i;
	unsigned short l,lp,lk;
	int k;

if (num==-2)
{
initwindow(1,1,80,25);}
menuwindow(1,25,80,25);
menuwindow(1,1,80,1);
window(1,1,80,25);
l=FALSE;
lp=FALSE;
textcolor(BLACK);
if ((num!=-1)&&(num!=-2)&&(num<10))
	{
		for (i=0;i<3;i++)
			{ if (i==num) MMenu[i]=GREEN;
							 else MMenu[i]=WHITE;
						viewmenu(i);
			}
	}
else
{

for (i=0;i<3;i++) MMenu[i]=WHITE;
MMenu[0]=GREEN;
do
{ for (i=0;i<3;i++)
	viewmenu(i);

	k=read_key();
	move(MMenu,3,k);
		switch (k)
		{
		case ENTER: l=TRUE; break;
		case F4:   lp=TRUE; break;
		case F5:   lp=TRUE; break;
		case F6:   lp=TRUE; break;
		case F7:   lp=TRUE; break;
		case ALT_X:lp=TRUE; break;
		}
}
while ((!l) && (!lp));
if (l==TRUE)
	{	if ((MMenu[0]==GREEN)) submenu(0);
		if ((MMenu[1]==GREEN)) submenu(1);
		if ((MMenu[2]==GREEN)) submenu(2);
	}

if (lp==TRUE)
 switch(k)
 {

	case F4:    {Comp();C_rle();};         break;
	case F5:    {cd=1;Comp();cdlzw(cd);};  break;
	case F6:    {Comp();D_rle();};         break;
	case F7:    {cd=0;Comp();cdlzw(cd);};  break;
	case ALT_X: quit(1);break;
 }
 }
}

void submenu(int p)
{
	int SMenu[3],i, max_i;
	int k=0;
	int x1,y1,x2,y2;
	char *text[3];
	unsigned short l1=FALSE,lp=FALSE;
	int menu1[2],view;
if (p==0)
	{ x1=x1sub1;  y1=y1sub1;
		x2=x2sub1;  y2=y2sub1;
		max_i=1;
		text[0]=SM11;
	}
if (p==1)
	{ x1=x1sub2;  y1=y1sub2;
          x2=x2sub2;  y2=y2sub2;
		max_i=3;
		text[0]=SM21; text[1]=SM22;
	}
if (p==2)
	{ x1=x1sub3;  y1=y1sub3;
		x2=x2sub3;  y2=y2sub3;
		max_i=2;
		text[0]=SM31; text[1]=SM32;

	}

	submenuinit(x1,y1,x2,y2);
	for (i=0;i<max_i;i++) SMenu[i]=WHITE;
	SMenu[0]=GREEN;
	view=0;
	do
		{
			textcolor(BLACK);
			 if(view==0)
				for (i=0;i<max_i;i++)
				{
				textbackground(SMenu[i]);
				gotoxy(2,i+2);
				cprintf(text[i]);
				if ((SMenu[i]==GREEN))
					{ window(1,25,80,25);
						textbackground(WHITE);
						clrscr();
						window(x1,y1,x2,y2);
						view=1;
					}

			}


		{ k=0;
			k=read_key();

			if (move(SMenu,max_i,k)!=-1) view=0;
		switch(k)
			{
				case LEFT: lp=TRUE;break;

				case RIGHT:lp=TRUE;break;

				case ENTER:l1=TRUE; break;
				case ESC:  lp=TRUE; break;
				case F4:   lp=TRUE; break;
				case F5:   lp=TRUE; break;
				case F6:   lp=TRUE; break;
				case F7:   lp=TRUE; break;
				case ALT_X:lp=TRUE; break;
			}
		}
		}while((!l1) && (!lp));

if (l1)
 {
	if(p==0)
			if (SMenu[i]==GREEN)
				{
					if (i==0) quit(1);
				}
	if(p==1)
	for (i=0;i<max_i;i++)
		if (SMenu[i]==GREEN)
				{   if (i==0){Comp();C_rle();}
				    if (i==1){cd=1; Comp();cdlzw(cd);}
				}
	if(p==2)
	for (i=0;i<max_i;i++)
		if (SMenu[i]==GREEN)
				{   if (i==0){Comp();D_rle();}
				    if (i==1){cd=0;Comp();cdlzw(cd);}
				}

	}
if (lp)
	switch(k)
	{

		case ESC:		initwindow(x1,y1,x2+2,y2+1);
								mainmenu(-1);
								break;

		case F4:    ;break;
		case F5:    ;break;
		case F6:    ;break;
		case F7:    ;break;
		case ALT_X: quit(1);break;

		case LEFT: initwindow(x1,y1,x2+2,y2+1);
				     if (p==0) {mainmenu(2);
													 submenu(2);}
				     if (p==1) {mainmenu(0);
											 submenu(0);}
				     if (p==2) {mainmenu(1);
													 submenu(1);}
						 break;

		case RIGHT: initwindow(x1,y1,x2+2,y2+1);
				       if (p==0) {mainmenu(1);
														submenu(1);}
				       if (p==1) {mainmenu(2);
														submenu(2);}
				       if (p==2) {mainmenu(0);
														submenu(0);}
						 break;



	}
}
void quit(int n)
{
	textbackground(BLACK);
	window(1,1,80,25);
	clrscr();
	textcolor(DARKGRAY);
	fcloseall();
	exit(n);
}
void Comp()
{       window(22,10,40,30);
	textcolor(WHITE);
	gotoxy(22,10);

}