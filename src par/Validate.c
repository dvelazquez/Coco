/*
   Multivariable controller and register for open and closed loop processes, it
   acquires a 1-5 VCD analog signal(PV) with an interfase based on an ADC0804. Sends
   a 4-20 mA analog signal to an I/P using a DAC. Commutation within ADC - DAC - PC
   is done through a tri-state buffer which it's selection is performed by two
   of the parallel port Control Lines. 
     *For precise information RTFM

   VALIDATE
   ***************************************************************************
   *      Only me saying no to undesired people, which I name the excluded   *
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

  BITMAP *B1, *B2;
  PALETTE pal;

int Validate()
{
  acquire_screen();

  B1 = load_bitmap("images/Validate/Base01.pcx", pal);
  B2 = load_bitmap("images/Validate/Base02.pcx", pal);
  system("mp3blaster sounds/GuacaGuaca.wav&");
  while (!key[KEY_ENTER])
  {
    blit(B1, screen, 0, 0, 0, 0, 1024, 768);
    usleep(400000);
    blit(B2, screen, 0, 0, 0, 0, 1024, 768);
    usleep(400000);
    system("mp3blaster sounds/GuacaGuaca.wav&");
  }

  Register();
  release_screen();  
  return 0;
}
