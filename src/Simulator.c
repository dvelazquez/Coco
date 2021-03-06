/*    
   Multivariable controller and register for open and closed loop processes, it
   acquires a 1-5 VCD analog signal(PV) with an interfase based on an ADC0804. Sends
   a 4-20 mA analog signal to an I/P using a DAC. Commutation within ADC - DAC - PC
   is done through a tri-state buffer which it's selection is performed by two
   of the parallel port Control Lines. 
     *For precise information RTFM

   SIMULATOR
   ***************************************************************************
   *      In this part the plant is virtualized, an static image of it is    *
   *   displayed and bars of each value are generated along the plant with   *
   *   the intention that the students can recognize them easier.   	     *
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
#include <sys/types.h>	
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/ppdev.h>
#include <sys/ioctl.h>
#include<linux/parport.h>

#include "IR.h"		/* this header defines program's values */

int i, fd, PVi;
float SP, OP, PV, SPj, PVj, OPj, Delay, iSP;
float K, Tin,Tdn;
  FILE *outfile;
  BITMAP *Screen3;
  PALETTE pal;

int Simulator()
{

  set_config_file("IR.config");  /* Read configuration file */
  Delay= (get_config_float("content", "Delay", 0));  

  fd = open("/dev/parport0", O_RDWR );

  Screen3= load_bitmap("images/Simulator/Base.pcx", pal);
  blit(Screen3, screen, 0, 0, 0, 0, 1024, 768);  
  /* Boxes to show numerical values */
  rectfill(screen, 895, 270, 945, 303, WHITE);   
  rect(screen, 895, 270, 945, 303, 1);	// x1, y1, x2, y2
  
  textprintf_ex(screen, font, 320, 0, BLACK, -1, "Simulacion de la Planta");  


  while (!key[KEY_LEFT])	/* Only pressing KEY_LEFT will return */
    {
    /*SERIAL CODE HERE */

      if(PVi<=48)
      {
	PVi= 51;
        system("festival --tts Messages/Disconnected &");
      }

//      if(PV<=48)
//        PVi= 51;

      PV= 1.367647*(PVi-51);//      279= X 204


      if(key[KEY_PGUP])
	SP= SP+45;

      if(key[KEY_PGDN])
	SP= SP-45;
      
      if(key[KEY_UP])
	SP= SP+5;

      if(key[KEY_DOWN])
	{
	  if(SP>=1)
	    SP= SP-5;
	}

      if(PVi>=253)
	{
	  textprintf_ex(screen, font, 320, 50, BLACK, RED, "Nivel Critico, revise el controlador");  
	  system("/usr/bin/mpg123 Sounds/Alarm.wav &");
	}

//      PV= (200/298)*100;

//	504  381  544  381
//	504  83   544  381
//      rectfill(screen, 832, 513-(SP/300), 883, 513, LIGHT_BLUE);
//      rectfill(screen, 832, 213, 883, 513-(SP/300), GRAY);

      /* Fill the tank with clear water */
      rectfill(screen, 687, 386-(SP/300), 710, 381, LIGHT_BLUE);
      rectfill(screen, 687, 83, 710, 386-(SP/300), GRAY);

      float K, BIAS;   K=-2; BIAS=184;
      K=(K/368)*100;

      /* (P) Proportional only control mode */
      OP= ((((SP/298)-(PV/298))*100)*K)+(OP/298);

	
      /* PV bar */
      rectfill(screen, 440, 381-(PV/298), 480, 381, RED);
      rectfill(screen, 440, 83, 480, 381-(PV/298), GRAY);

      /* SP bar */
      if (SP>=88734)	SP=88734;      
      if (SP<=1)	SP=1;
      rectfill(screen, 504, 381-(SP/298), 544, 381, GREEN);
      rectfill(screen, 504, 83, 544, 381-(SP/298), GRAY);

      /* OP bar */
      if (OP>=88734)	OP=88734;      
      if (OP<=1)	OP=1;
      rectfill(screen, 569, 381-(OP/298), 609, 381, BLUE);
      rectfill(screen, 569, 83, 609, 381-(OP/298), GRAY);


      textprintf_ex(screen, font, 900, 264, BLACK, WHITE, "%3.1fOP", (OP/88734)*100);
//      textprintf_ex(screen, font, 900, 364, BLACK, WHITE, "%3.1fOP", (OP/298)*100);      
      textprintf_ex(screen, font, 900, 364, BLACK, WHITE, "%3.1fSP", (SP/88734)*100);

      rest(Delay);
    }
  Automatic();      
  return 1;
}
