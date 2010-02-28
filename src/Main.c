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
#include <string.h>
#include <unistd.h>
#include <allegro.h>	/* great library to easily handle graphics */
#include <fcntl.h>
#include <termios.h>
#include <errno.h>

#include "IR.h"		/* this header defines program's values */

int fd;
float SP, iSP, Delay;
unsigned char sResult[0], sCmd[0];

int main(int argc, unsigned char **argv)
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

/* Here we write to serial port */
int writeport(int fd, unsigned char *chars) {
	write(fd, chars, 1);
	return 1;                                                                       	                                
}
    
/* Here we read serial port */
int readport(int fd, unsigned char *result) 
{
  int iIn = read(fd, result, 1); // al parecer el 1 es el buffer
  result[1] = 0;
  return 1;
}

/* This rutine configures Baudrate */
int getbaud(int fd) 
{
  struct termios termAttr;
  int inputSpeed = -1;
  speed_t baudRate;
  tcgetattr(fd, &termAttr);
  /* Get the input speed.                              */
  baudRate = cfgetispeed(&termAttr);
  switch (baudRate) {
  case B0:      inputSpeed = 0; break;
  case B50:     inputSpeed = 50; break;
  case B110:    inputSpeed = 110; break;
  case B134:    inputSpeed = 134; break;
  case B150:    inputSpeed = 150; break;
  case B200:    inputSpeed = 200; break;
  case B300:    inputSpeed = 300; break;
  case B600:    inputSpeed = 600; break;
  case B1200:   inputSpeed = 1200; break;
  case B1800:   inputSpeed = 1800; break;
  case B2400:   inputSpeed = 2400; break;
  case B4800:   inputSpeed = 4800; break;
  case B9600:   inputSpeed = 9600; break;
  case B19200:  inputSpeed = 19200; break;
  case B38400:  inputSpeed = 38400; break;
  }
  return inputSpeed;
}

int initport(int fd) {
	struct termios options;
	// Get the current options for the port...
	tcgetattr(fd, &options);
	// Set the baud rates to 19200...
	cfsetispeed(&options, B9600);
	cfsetospeed(&options, B9600);
	// Enable the receiver and set local mode...
	options.c_cflag |= (CLOCAL | CREAD);

	options.c_cflag &= ~PARENB;
	options.c_cflag &= ~CSTOPB;
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;

	// Set the new options for the port...
	tcsetattr(fd, TCSANOW, &options);
	return 1;
}
