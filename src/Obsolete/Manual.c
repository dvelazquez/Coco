/* 
   Multivariable controller and register for open and closed loop processes, it
   acquires a 1-5 VCD analog signal(PV) with an interfase based on an ADC0804. Sends
   a 4-20 mA analog signal to an I/P using a DAC. Commutation within ADC - DAC - PC
   is done through a tri-state buffer which it's selection is performed by two
   of the PORTA PIC's Control Lines. 
     *For precise information RTFM

   MANUAL
   ***************************************************************************
   *   This part uses user (manual) control to generate the OP value
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

#include <stdio.h>	/* standard C */
#include <unistd.h>
#include <allegro.h>	/* great library to easily handle graphics */
#include <fcntl.h>
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */


#include "IR.h"		/* this header defines program's values */

int i, M, fd, Timer, OPi;
float SP, OP, PV, SPj, OPj, PVj, Delay;
char Name[35], Number[20], Mode[3], Gain[3], Option;
/* unsigned char for the serial values since -128 < char < 128
    and 0 < unsigned char < 255  */
unsigned char sResult[0], sCmd[0];	
float iSP, K;

  FILE *outfile;
  BITMAP *Captura, *Clean, *Screen2, *Disconnected;
  PALETTE pal;
int Manual(int argc, unsigned char **argv)
{
  /* Serial port initialization */

  fd = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NDELAY);
  if (fd == -1) {
    perror("open_port: Unable to open /dev/ttyS0 - ");
    return 1;
  } else {
    fcntl(fd, F_SETFL, 0);
  }
  getbaud(fd);

  initport(fd);

  int PVi, k;
        K= uatof(Gain);
  /* Things will be drawn more quickly if you always acquire the screen before
     trying to draw onto it. */
  acquire_screen();
  
  Graphics();	/* Call to graphics from a different function to avoid messy code */
  
  RLE_SPRITE *rle;
 
            sCmd[0]= 'C';
            writeport(fd, sCmd);

  PVi= 0;  k= 100;  M=0;
  while (!key[KEY_ESC])	/* Only pressing ESC can Exit */
    {
	char ValveState;
	if(ValveState=='O')
	{
            sCmd[0]= 'A';
            writeport(fd, sCmd);
	}
	if(ValveState=='C')
	{
            sCmd[0]= 'C';
            writeport(fd, sCmd);
	}
	


      M++;
      /* Check if user needs some help */
      if(key[KEY_F1])
	Help();

      k++;
      i= 897;
      
      Captura = create_sub_bitmap(screen, 72, 350, 898, 368);
      rle = get_rle_sprite(Captura);
      destroy_bitmap(Captura);
      draw_rle_sprite(screen, rle, 71, 350);
      destroy_rle_sprite(rle);

      /* This line reads data from the interfase which is
	 the process variable(measured variable) of the system */
          fcntl(fd, F_SETFL, FNDELAY); // don't block serial read
	  readport(fd,sResult);
	  PVi= (int) *sResult;
	  PV= PVi;      
/*
      if(PVi<=40)
	{
	  PV= 51;
	  system("festival --tts Messages/Disconnected&");
	  blit(Disconnected, screen, 0, 0, 70, 290, 887, 52);   
	}
   */   
      if(PV<=48)
        PVi= 51;
      
      PV= 1.794117647*(PVi-51);
      SP= PV;
      
      if(key[KEY_RIGHT])
	{
          //fd = close("/dev/parport0");	
	  Simulator();
        }
      if(key[KEY_PGUP])
	OP= (OP+46);
      
      if(key[KEY_PGDN])
	OP= (OP-46);
      
      if(key[KEY_UP])
	{
	    OP=162; //(OP+3.66);
            sCmd[0]= 'A';
            writeport(fd, sCmd);
	    ValveState='O';
	}

      if(key[KEY_DOWN])
	{
	  if(OP>=1)
	    OP= 0;//(OP-3.66);
            sCmd[0]= 'C';
            writeport(fd, sCmd);
	    ValveState='C';
	}
      
      if(key[KEY_PRTSCR])
	{
          Captura = create_sub_bitmap(screen, 0, 0, 1024, 768);
          save_bitmap("images/User/Captura.pcx", Captura, pal);
          destroy_bitmap(Captura);
        }
      
      Timer++;
      
      if(OP<=0)
	OP= 0;
      
      
//      if (PV>=40)
//	{
	  textprintf_ex(screen, font, 230, 297, BLACK, WHITE, "%3.1f", (PV/368)*100);	// Medición
	  textprintf_ex(screen, font, 450, 297, BLACK, WHITE, "%3.1f", (SP/368)*100);	// SP
	  textprintf_ex(screen, font, 710, 297, BLACK, WHITE, "%3.1f", (OP/368)*100);	// Controlador
//	}
      
      if(k>=100)
	{
 	  k= 0;
	  vline(screen, 967, 351, 717, GRAY);
          blit(Clean, screen, 0, 0, 968, 350, 2, 368);
	}
      
      int Recorder;
      Recorder++;
      if(Recorder>=900)
	{
 	  Recorder= 0;
	  Captura = create_sub_bitmap(screen, 258, 350, 715, 368);
	}
      
      Captura = create_sub_bitmap(screen, 248, 350, 705, 368);
      
      
      if(PV>=362) PV= 365;
      if(OP>=367) OP= 365;
      
      if(PV<=0) PV= 0;
      if(OP<=0)
	 OP= 1;
/*
      OPi= fixtoi(itofix((OP*0.69234783)/255*100));
      sCmd[0]= (unsigned char)OPi+50;
      sCmd[0]= sCmd[0]+0.00;
      writeport(fd, sCmd);
*/
//     textprintf_ex(screen, font, 30, 297, BLACK, WHITE, "%i - %s - %3.1f", PVi, sResult, PV);	// Medición      

      /* Draw the behaviour of the PV, SP and OP over time */
      line(screen, 71+i, 717-PV, 71+i, 717-PVj, RED);	
      PVj= PV;    /* Flag for line y2 as a precedent state */
      line(screen, 71+i, 717-OP, 71+i, 717-OPj, BLUE);	
      OPj= OP;    /* Flag for line y2 as a precedent state */
      
      fprintf(outfile,"%i\t%f\t %f\t %f\n", M, ((PV/368)*100), ((SP/368)*100), ((OP/368)*100));
      rest(Delay);
    }
  int ScreenWide;
  RLE_SPRITE *rle0, *rle1;
  BITMAP *Screen3;

  Screen3 = load_bitmap("images/Close/Base.pcx", pal);
  system("mp3blaster /home/mentesuprema/Residencia/sounds/swing2.wav &");
  rest(5);

  rle0= get_rle_sprite(Screen2);
  rle1= get_rle_sprite(Screen3);
  
  for(ScreenWide=0;ScreenWide<=768;ScreenWide=ScreenWide+5)
    {
      draw_rle_sprite(screen, rle0, 0, 768);
      draw_rle_sprite(screen, rle1, 0, -768+ScreenWide);
    }

  destroy_rle_sprite(rle0);  
  destroy_rle_sprite(rle1);    
  destroy_bitmap(Screen2);
  destroy_bitmap(Screen3);
  destroy_bitmap(Clean);
  destroy_bitmap(Disconnected);
  release_screen();
  Close();
  exit(0);
} 

