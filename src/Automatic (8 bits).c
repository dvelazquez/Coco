/* 
   Multivariable controller and register for open and closed loop processes, it
   acquires a 1-5 VCD analog signal(PV) with an interfase based on an ADC0804. Sends
   a 4-20 mA analog signal to an I/P using a DAC. Commutation within ADC - DAC - PC
   is done through a tri-state buffer which it's selection is performed by two
   of the PORTA PIC's Control Lines. 
     *For precise information RTFM

   AUTOMATIC
   ***************************************************************************
   *	  This is the section where the project is based on, the file is     *
   *   Principal.c as so is the function which it handles, here you will     *
   *   find the graph and the ISA diagram.   	   			     *
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
#include <string.h>
#include <unistd.h>
#include <allegro.h>	/* great library to easily handle graphics */
#include <fcntl.h>
#include <errno.h>
#include <termios.h>

#include "IR.h"		/* this header defines program's values */

int i, M, fd, Timer;		/* M is #of Measure */
float SP, OP, PV, SPj, PVj, OPj, Delay;
/* These are global variables because we are going to need them at the simulator */
float K, Tin,Tdn;
/* This are global variables needed for every function */
char Name[35], Number[20], Mode[3], Gain[3], Ti[3], Td[3],  Option;
unsigned char sResult[0];
float iSP;

  FILE *outfile;
  BITMAP *Captura, *Clean, *Screen2, *Disconnected;
  PALETTE pal;
int Automatic(int argc, unsigned char **argv)
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

     /* Things will be drawn more quickly if you always acquire the screen before
     trying to draw onto it. */
  acquire_screen();

  textprintf_ex(screen, font, 75, 75, BLACK, -1, "%s", Gain);
  Graphics();	/* Call to graphics from a different function to avoid messy code */
  
  RLE_SPRITE *rle;

  PVi= 0;  k= 100;  M=0;
  while (!key[KEY_ESC])	/* Only pressing ESC can Exit */
    {

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
	  readport(fd,sResult);
	  PVi= (int) *sResult;
	  PV= PVi;


/*     
      if(PV<=40)
	{
	  PV= 51;
	  system("festival --tts messages/Disconnected&");
	  blit(Disconnected, screen, 0, 0, 70, 290, 887, 52);   
	}
   */   
      if(PV<=48)
        PVi= 51;
      
      PV= 1.794117647*(PVi-51);
      
      if(key[KEY_RIGHT])
	{
	  Simulator();
        }
      if(key[KEY_PGUP])
	SP= SP+46;
      
      if(key[KEY_PGDN])
	SP= SP-46;
      
      if(key[KEY_UP])
	SP= SP+3.66;
      
      if(key[KEY_DOWN])
	{
	  if(SP>=1)
	    SP= SP-3.66;
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

      K= uatof(Gain); Tin= uatof (Ti);  Tdn= uatof(Td);
      //      float K, BIAS, Ti,Td;   K=-2; BIAS=184; Ti=1;  Td=50;
            K=(K/368)*100;   Tin=(Tin/368)*100; //Td=(Td/368)*100;
      //      Gain= (Gain/368)*100;  Ti= (Ti/368)*100;  Td= (Td/368)*100;
      
      /* (P) Proportional only control mode */
      if (ustrcmp(Mode,"P")==0)
        OP= ((((PV/368)-(SP/368))*100)*K)+OP;      
      
	/* (PI) Proportional plus Integral control mode */
      if (ustrcmp(Mode,"PI")==0)
        OP= ((((PV/368)-(SP/368))*100)*K)+OP;      

      /* Ideal PID algorithm */
      if (ustrcmp(Mode,"PID")==0)
        OP = (K*(1 + (1/Tin) * ((SP/368)-(PV/368))*100)) + Tdn;
      
      
//      if (PV>=40)
//	{
	  textprintf_ex(screen, font, 230, 297, BLACK, WHITE, "%3.1f ", (PV/368)*100);	// Medición
	  textprintf_ex(screen, font, 450, 297, BLACK, WHITE, "%3.1f ", (SP/368)*100);	// SP
	  textprintf_ex(screen, font, 710, 297, BLACK, WHITE, "%3.1f ", (OP/368)*100);	// Controlador
	  textprintf_ex(screen, font, 890, 297, BLACK, WHITE, "%3.1f ", ((SP/368)-(PV/368))*100);  //Error
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
      if(SP>=362) SP= 365;
      if(OP>=362) OP= 365;
      
      if(PV<=0) PV= 0;
      if(SP<=0) SP= 0;
      if(OP<=0) OP= 0;
      
      /* Draw the behaviour of the PV and SP over time */
      line(screen, 71+i, 717-SP, 71+i, 717-SPj, GREEN);		
      SPj= SP;	/* Flag for line y2 as a precedent state */
      line(screen, 71+i, 717-PV, 71+i, 717-PVj, RED);	
      PVj= PV;    /* Flag for line y2 as a precedent state */
      line(screen, 71+i, 717-OP, 71+i, 717-OPj, BLUE);	
      OPj= OP;    /* Flag for line y2 as a precedent state */
      
      fprintf(outfile,"%i\t%f\t %f\t %f\t %f\n", M, ((PV/368)*100), ((SP/368)*100), ((OP/368)*100), ((PV/368)*100)-((SP/368)*100));
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
  
  for(ScreenWide=0;ScreenWide<=768;ScreenWide=ScreenWide+10)
    {
      draw_rle_sprite(screen, rle0, 0, ScreenWide);
      draw_rle_sprite(screen, rle1, 0,-768+ScreenWide);
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

/* Graphics under a different section to keep code clean */
int Graphics()
{
  
  /* Declare all images at once */
  Clean = load_bitmap("images/Principal/Clean.pcx", pal);
  Screen2 = load_bitmap("images/Principal/Base.pcx", pal);    
  Disconnected = load_bitmap("images/Principal/Desconectado.pcx", pal);    
  
  blit(Screen2, screen, 0, 0, 0, 0, 1024, 768);
  textprintf_ex(screen, font, 383, 55, 0, -1, "%s  ", Mode);
  
  float Var;  Var=0;
  /* Draw the grid's horizontal lines */
  for(Var=1; Var<=15;Var++)
    {
      hline(screen, 71, 350+(Var*23), 969, GRAY);  	// x1,  y, x2  
    }
  
  Var=0;
  /* Draw the grid's vertical lines */
  for(Var=1; Var<=8;Var++)
    {
      vline(screen, 70+(Var*100), 351, 717, GRAY);
    }
  textprintf_ex(screen, font, 475, 718, BLACK, -1, "%s - %s", Name, Number);
  
  release_screen();
  
  return 0;
}
