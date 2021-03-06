/*   Multivariable controller and register for open and closed loop processes, it
   acquires a 1-5 VCD analog signal(PV) with an interfase based on an ADC0804. Sends
   a 4-20 mA analog signal to an I/P using a DAC. Commutation within ADC - DAC - PC
   is done through a tri-state buffer which it's selection is performed by two
   of the parallel port Control Lines. 
     *For precise information RTFM

   REGISTER
   ***************************************************************************
   *      This is where users registers their Name and Ctrl Nbr in order to  *
   *   take a record of the student(s) that made the practice, this way      *
   *   software prevents to copy-paste the practice results.                 *
   ***************************************************************************


   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful
   for electronics engineering or processes' control students
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   (C) Daniel Velazquez  <dvelazquezb@gmail.com>
   Start date:20060702	(July 2nd, 2006)
*/

#include <string.h>
#include <stdio.h>	/* standard C */
#include <allegro.h>	/* great library to easily handle graphics */

#include "IR.h"		/* this header defines program's values */

  FILE *outfile;
  BITMAP *Screen1, *GainBox, *ModeBox, *TdBox, *TiBox, *Screen2;
  PALETTE pal;

  /* User will enter these values here and them will be used along the program
     since these are the parameters of each control mode */

//float SP, OP, PV, SPj, PVj, OPj, Delay;
float PV;
  char Name[35], Number[20], FName[40], Mode[2], Gain[2], Ti[2], Td[2],  sResult[0];
  char Letteri, Letterj, Letterk, Letterl, Option;
  int i,fd;

/* Section to register users and their parameters of control */
int Register(int argc, char **argv)
{ 
  acquire_screen();
  
  GainBox= load_bitmap("images/Register/Gain.pcx", pal);
  ModeBox= load_bitmap("images/Register/Mode.pcx", pal);
  TiBox= load_bitmap("images/Register/Ti.pcx", pal);
  TdBox= load_bitmap("images/Register/Td.pcx", pal);
  Screen1 = load_bitmap("images/Register/Base.pcx", pal);
  Screen2 = load_bitmap("images/Principal/Base.pcx", pal);
  blit(Screen1, screen, 0, 0, 0, 0, 1024, 768);
  
  font = load_font("config/unifont.dat", NULL, NULL);
  Letteri=Letterk=NULL;	  

  textprintf_ex(screen, font, 150, 250, 0, -1, "Tipo de Control:");
  textprintf_ex(screen, font, 210, 280, DARK_GRAY, -1, "*Manual     *Automatico ");  

  CaptureName();
}

/* Different functions for each field so I can program their navigation easier */
void CaptureName()
{
  while (!key[KEY_ESC])
    {
      Letteri= readkey();   
      if(key[KEY_ENTER])
	break;
      if(key[KEY_RIGHT])
	CaptureNumber();
      if(key[KEY_BACKSPACE])
	{
	  Letteri=NULL;
	  uremove(Name,-1);
	}
      Letterj= Letteri;
      strcat(Name, &Letterj);
      textprintf_ex(screen, font, 70, 190, 0, WHITE, "%s  ", Name);
      if (strcmp(Name, "PaeZ")==0)	Validate();
      if (strcmp(Name, "Inge Cesar")==0)	Validate();
    }
  CaptureNumber();
}

void CaptureNumber()
{    
  while (!key[KEY_ESC])
    {
      Letteri= readkey();   
      if(key[KEY_ENTER])
	break;
      if(key[KEY_ESC])
	exit(0);
      if(key[KEY_LEFT])
	CaptureName();
      if(key[KEY_BACKSPACE])
	{
	  Letteri=NULL;
	  uremove(Number,-1);
	}
      Letterj= Letteri;
      strcat(Number, &Letterj);
      textprintf_ex(screen, font, 770, 190, 0, WHITE, "%s  ", Number);
    }
  ControlType();
}

void ControlType()
{
  textprintf_ex(screen, font, 150, 250, 0, -1, "Tipo de Control:");
  textprintf_ex(screen, font, 210, 280, DARK_GRAY, -1, "*Manual     *Automatico ");  
  
  while (!key[KEY_ESC])
    {
      Letteri= readkey();
      if(key[KEY_ENTER])
        break;        
      if(key[KEY_RIGHT])
	{
	  textprintf_ex(screen, font, 210, 280, DARK_GRAY, -1, "*Manual     *Automatico ");  
	  textprintf_ex(screen, font, 372, 280, 0, -1, "*Automatico");
	  Option='A';
	}
      if(key[KEY_LEFT])
	{
	  textprintf_ex(screen, font, 210, 280, DARK_GRAY, -1, "*Manual     *Automatico ");  
	  textprintf_ex(screen, font, 210, 280, 0, -1, "*Manual");
	  Option='M';
	}
      if(key[KEY_UP])
	CaptureNumber();      
      
    }
  
  if(Option=='M')
    ManSettings();
  
  if(Option=='A')
    AutoSettings();	  
  
  Proceed();
  return 0;
}

void ManSettings()
{
  blit(GainBox, screen, 0, 0, 300, 400, 114,55);    
  while (!key[KEY_ESC])
    {
      Letteri= readkey();   
      if(key[KEY_ENTER])
	break;
      if(key[KEY_ESC])
	exit(0);
      if(key[KEY_BACKSPACE])
	{
	  Letteri=NULL;
	  uremove(Gain,-1);
	}
      if(uisdigit(Letteri)!=0)
	{
	  Letterj= Letteri;
	  strcat(Gain, &Letterj);
	}
      textprintf_ex(screen, font, 320, 415, 0, WHITE, "%s  ", Gain);
    }
  Proceed();
}

void AutoSettings()
{
  blit(ModeBox, screen, 0, 0, 100, 400, 114,55);    
  while (!key[KEY_ESC])
    {
      Letteri= readkey();   
      if(key[KEY_ENTER])
	if ((ustrcmp(Mode,"P")==0) || (ustrcmp(Mode,"PI")==0) || (ustrcmp(Mode,"PID")==0))
	  break;
	else
	  {
	    uremove(Mode,-1);
	    uremove(Mode,-1);	    
	    uremove(Mode,-1);
	    AutoSettings();
	  }
      if(key[KEY_ESC])
	exit(0);
      if(key[KEY_BACKSPACE])
	{
	  Letteri=NULL;
	  uremove(Mode,-1);
	}
      Letterj= Letteri;
      strcat(Mode, &Letterj);
      ustrupr(Mode);
      textprintf_ex(screen, font, 120, 415, 0, WHITE, "%s  ", Mode);
    }
  
  if (ustrcmp(Mode,"P")==0)  
    {
      blit(GainBox, screen, 0, 0, 300, 400, 114,55);    
      while (!key[KEY_ESC])
	{
	  Letteri= readkey();   
	  if(key[KEY_ENTER])
	    break;
	  if(key[KEY_ESC])
	    exit(0);
	  if(key[KEY_BACKSPACE])
	    {
	      Letteri=NULL;
	      uremove(Gain,-1);
	    }
	  Letterj= Letteri;
	  strcat(Gain, &Letterj);
	  textprintf_ex(screen, font, 320, 415, 0, WHITE, "%s  ", Gain);
	}
      Proceed();
    }
  if (ustrcmp(Mode,"PI")==0)
    {
      blit(GainBox, screen, 0, 0, 300, 400, 114,55);    
      while (!key[KEY_ESC])
	{
	  Letteri= readkey();   
	  if(key[KEY_ENTER])
	    break;
	  if(key[KEY_ESC])
	    exit(0);
	  if(key[KEY_BACKSPACE])
	    {
	      Letteri=NULL;
	      uremove(Gain,-1);
	    }
	  Letterj= Letteri;
	  strcat(Gain, &Letterj);
	  textprintf_ex(screen, font, 320, 415, 0, WHITE, "%s  ", Gain);
	}
      blit(TiBox, screen, 0, 0, 500, 400, 114,55);    
      while (!key[KEY_ESC])
	{
	  Letteri= readkey();   
	  if(key[KEY_ENTER])
	    break;
	  if(key[KEY_ESC])
	    exit(0);
	  if(key[KEY_BACKSPACE])
	    {
	      Letteri=NULL;
	      uremove(Gain,-1);
	    }
	  Letterj= Letteri;
	  strcat(Ti, &Letterj);
	  textprintf_ex(screen, font, 520, 415, 0, WHITE, "%s  ", Ti);
	}
      Proceed();      
    }
  if (ustrcmp(Mode,"PID")==0)
    {
      blit(GainBox, screen, 0, 0, 300, 400, 114,55);    
      while (!key[KEY_ESC])
	{
	  Letteri= readkey();   
	  if(key[KEY_ENTER])
	    break;
	  if(key[KEY_ESC])
	    exit(0);
	  if(key[KEY_BACKSPACE])
	    {
	      Letteri=NULL;
	      uremove(Gain,-1);
	    }
	  Letterj= Letteri;
	  strcat(Gain, &Letterj);
	  textprintf_ex(screen, font, 320, 415, 0, WHITE, "%s  ", Gain);
	}
      blit(TiBox, screen, 0, 0, 500, 400, 114,55);    
      while (!key[KEY_ESC])
	{
	  Letteri= readkey();   
	  if(key[KEY_ENTER])
	    break;
	  if(key[KEY_ESC])
	    exit(0);
	  if(key[KEY_BACKSPACE])
	    {
	      Letteri=NULL;
	      uremove(Ti,-1);
	    }
	  Letterj= Letteri;
	  strcat(Ti, &Letterj);
	  textprintf_ex(screen, font, 520, 415, 0, WHITE, "%s  ", Ti);
	}
      blit(TdBox, screen, 0, 0, 700, 400, 114,55);    
      while (!key[KEY_ESC])
	{
	  Letteri= readkey();   
	  if(key[KEY_ENTER])
	    break;
	  if(key[KEY_ESC])
	    exit(0);
	  if(key[KEY_BACKSPACE])
	    {
	      Letteri=NULL;
	      uremove(Td,-1);
	    }
	  Letterj= Letteri;
	  strcat(Td, &Letterj);
	  textprintf_ex(screen, font, 720, 415, 0, WHITE, "%s  ", Td);
	}
      Proceed();
    }
  rest(1000);
  Proceed();
}

void Proceed()
{
  /* This is a transition effect */
  int ScreenWide;
  RLE_SPRITE *rle0, *rle1;

  strcpy(FName, "logs/");  strcat(FName, Name);  strcat(FName, " - "); strcat(FName, Number); strcat(FName, ".log");
  outfile = fopen(FName,"w"); 

  if(Option=='A')
    {
      system("mp3blaster /home/mentesuprema/Residencia/sounds/swing2.wav &");
      rest(5);
      rle0= get_rle_sprite(Screen1);
      rle1= get_rle_sprite(Screen2);

      for(ScreenWide=0;ScreenWide<=1024;ScreenWide=ScreenWide+10)
	{
	  draw_rle_sprite(screen, rle0, ScreenWide, 0);
	  draw_rle_sprite(screen, rle1, -1024+ScreenWide, 0);
	}
      destroy_rle_sprite(rle0);
      release_screen();  
      Automatic();	  
    }
  if(Option=='M')
    {
      system("mp3blaster /home/mentesuprema/Residencia/sounds/swing2.wav &");
      rest(5);
      rle0= get_rle_sprite(Screen1);
      rle1= get_rle_sprite(Screen2);

      for(ScreenWide=0;ScreenWide<=1024;ScreenWide=ScreenWide+10)
	{
	  draw_rle_sprite(screen, rle0, ScreenWide, 0);
	  draw_rle_sprite(screen, rle1, -1024+ScreenWide, 0);
	}
      destroy_rle_sprite(rle0);
      destroy_rle_sprite(rle1);
      destroy_bitmap(Screen1);
      destroy_bitmap(Screen2);
      release_screen();  
      Manual();
    }  
  
  release_screen();  
}
