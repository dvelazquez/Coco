/* 
   Multivariable controller and register for open and closed loop processes, it
   acquires a 1-5 VCD analog signal(PV) with an interfase based on an ADC0804. Sends
   a 4-20 mA analog signal to an I/P using a DAC. Commutation within ADC - DAC - PC
   is done through a tri-state buffer which it's selection is performed by two
   of the parallel port Control Lines. 
     *For precise information RTFM

   Close
   ***************************************************************************
   *      In this section program asks if user really needs to finish the    * 	
   *   process or get back to the beginning in order to start a new process  *
   *                                                                         *
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

  BITMAP *Screen4;
  PALETTE pal;

/* These are global values added in this section just to erase them in case
   user wants to start a new practice */
char Name[35], Number[20], FName[40], Mode[2], Gain[2], Ti[2], Td[2];
char Letteri, Option;

int Close()
{
  acquire_screen();
  
  Screen4= load_bitmap("images/Close/Base.pcx",pal);
  blit(Screen4,screen,0,0,0,0,1024,768);
  textprintf_ex(screen, font, 360, 290, DARK_GRAY, -1, "*");
  textprintf_ex(screen, font, 360, 360, DARK_GRAY, -1, "*");
  textprintf_ex(screen, font, 360, 425, DARK_GRAY, -1, "*");
  
  while (Option!='Q')
    {
      Letteri= readkey();
      if(key[KEY_ENTER])
	break;                
      if(key[KEY_UP])
	{
	  textprintf_ex(screen, font, 360, 360, DARK_GRAY, -1, "*");
	  textprintf_ex(screen, font, 360, 425, DARK_GRAY, -1, "*");
	  textprintf_ex(screen, font, 360, 290, 0, -1, "*");
	  Option='R';	  
	}
      if(key[KEY_DOWN])
	{
	  textprintf_ex(screen, font, 360, 290, DARK_GRAY, -1, "*");
	  textprintf_ex(screen, font, 360, 425, DARK_GRAY, -1, "*");
	  textprintf_ex(screen, font, 360, 360, 0, -1, "*");	  
	  Option='S';
	}
      
    }
  Finish();
}

int Finish()
{
  if (Option=='R')
    {
      /* Here it erases every value stored in buffer so
	 we can open a new session with differen values */
      int Length1 = ustrsizez(Name);  //Erase Name
      while(Length1!=0)  
	{
	  Length1--;
	  uremove(Name, -1);  
	}
      
      int Length2 = ustrsizez(Number);  //Erase Number
      while(Length2!=0)
	{
	  Length2--;
	  uremove(Number, -1);
	}
      
      int Length3 = ustrsizez(FName);  //Erase FileName
      while(Length3!=0)
	{
	  Length3--;
	  uremove(FName, -1);
	}
      
      int Length4 = ustrsizez(Mode);  //Erase ControlMade
      while(Length4!=0)
	{
	  Length4--;
	  uremove(Mode, -1);
	}
      
      int Length5 = ustrsizez(Gain);  //Erase Gain
      while(Length5!=0)
	{
	  Length5--;
	  uremove(Gain, -1);
	}
      
      int Length6 = ustrsizez(Ti);  //Erase Integral Time
      while(Length6!=0)
	{
	  Length6--;
	  uremove(Ti, -1);
	}

      int Length7 = ustrsizez(Td);  //Erase Differential Time
      while(Length7!=0)
	{
	  Length7--;
	  uremove(Td, -1);
	}

      /* Transition effects */
      int ScreenWide;
      RLE_SPRITE *rle0, *rle1;
      BITMAP *Screen1, *Captura;
      
      Screen1 = load_bitmap("images/Register/Base.pcx", pal);
     system("mp3blaster /home/mentesuprema/Residencia/sounds/swing2.wav &");
     rest(5);

          Captura = create_sub_bitmap(screen, 0, 0, 1024, 768);
          save_bitmap("images/User/Captura.pcx", Captura, pal);
          destroy_bitmap(Captura);

      rle0= get_rle_sprite(Screen1);
      rle1= get_rle_sprite(Screen4);
      
      for(ScreenWide=0;ScreenWide<=768;ScreenWide=ScreenWide+5)
	{
	  draw_rle_sprite(screen, rle1, 0, 768);
	  draw_rle_sprite(screen, rle0, 0, -768+ScreenWide);
	}
      
      destroy_rle_sprite(rle0);  
      destroy_rle_sprite(rle1);    
      destroy_bitmap(Screen1);
      destroy_bitmap(Screen4);
      release_screen();
      
      /* Go back to register a new session */
      Register();
    }

  /* Open up Scilab with the values just captured */ 
  if (Option=='S')
    {
      system("sh /home/mentesuprema/Residencia/src/Scilab.sh &");
      exit(0);
    }
  release_screen();
  exit(0);
  
}
