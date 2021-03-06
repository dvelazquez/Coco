/*
   Multivariable controller and register for open and closed loop processes, it
   acquires a 1-5 VCD analog signal(PV) with an interfase based on an ADC0804. Sends
   a 4-20 mA analog signal to an I/P using a DAC. Commutation within ADC - DAC - PC
   is done through a tri-state buffer which it's selection is performed by two
   of the parallel port Control Lines. 
     *For precise information RTFM

   MAIN
   ***************************************************************************
   *      This is where the program begins, from here it'll start Screen's   *
   *   Resolution, Open Port, keyboard, Timer and then will call the         *
   *   Register function in order to start the user interface.               *
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

int fd;
float SP, iSP, Delay;

int main()
{
  /* Variables located at IR.config */
  int Resol_X, Resol_Y, count;
  char **screen;
  
  if (allegro_init() != 0)   /* you should always do this at the start of Allegro programs */
    return 1;
  
  set_config_file("config/Coco.config");  /* Read configuration file */
  
  install_keyboard();   /* set up the keyboard handler */
  install_timer();	/* set up the timer handler */
  
  set_color_depth(32);  
  /* Get screen resolution from Coco.config */
  screen = get_config_argv("content", "SCREEN_SIZE", &count);
  Resol_X = atoi(screen[0]);  Resol_Y = atoi(screen[1]);
  if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, Resol_X, Resol_Y, 0, 0) != 0)   /* for X */
    {
      if (set_gfx_mode(GFX_SAFE, Resol_X, Resol_Y, 0, 0) != 0)        /* for console */
	{
	  set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
	  allegro_message("Unable to set any graphic mode\n%s\n", allegro_error);
	  return 1;
	}
    }
  
  /* Check if all images exists before trying to blit'em */
       /* Add some protection */

  fd=OpenPort("/dev/parport0");
  
  set_palette(desktop_palette);   /* set the color palette */
  
  /* Read desired sampling time in IR.config */
  Delay= (get_config_float("content", "Delay", 0));  
  
  iSP= (get_config_float("content", "iSP", 0));  
  SP= iSP*3.68;
  
  
  
  /* Go to Register the user and run the program after having hardware initialized */
  Register();
  
  return 0;
}
END_OF_MAIN()
