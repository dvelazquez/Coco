/* 
   Multivariable controller and register for open and closed loop processes, it
   acquires a 1-5 VCD analog signal(PV) with an interfase based on an ADC0804. Sends
   a 4-20 mA analog signal to an I/P using a DAC. Commutation within ADC - DAC - PC
   is done through a tri-state buffer which it's selection is performed by two
   of the PORTA PIC's Control Lines. 
     *For precise information RTFM

   HELP
   ***************************************************************************
   *    By pressing F1 user will call this section for assistance with the     *
   *   most Frequently Asked Questions.						         *
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

/* The super Help section*/
#include <stdio.h>
#include <allegro.h>
#include "IR.h"		/* this header defines program's values */

int Help()
{
  clear_to_color(screen, WHITE);  /* clear the screen to white */
  textout_ex(screen, font, "  Introducción", 180, 120, BLACK, -1);
  textout_ex(screen, font, "Esta es una interfase de adquisición de datos de alta velocidad, alto rendimiento", 35, 150, BLACK, -1);
  textout_ex(screen, font, "y multifuncional para puerto serial DB-9 RS232.", 35, 180, BLACK, -1);
  textout_ex(screen, font, "  Características Principales:", 180, 220, BLACK, -1);
  textout_ex(screen, font, "* Un canal de salida analógica de 0 a 5 V", 35, 250, BLACK, -1);
  textout_ex(screen, font, "* Convertidor Analógico Digital de 8 bits (ADC0804)", 35, 280, BLACK, -1);
  textout_ex(screen, font, "* Control por software la selección de entrada o salida ded voltaje", 35, 310, BLACK, -1);
  textout_ex(screen, font, "* Aislamiento de voltajes de entrada y salida", 35, 340, BLACK, -1);
  textout_ex(screen, font, "* Salida de voltaje de 8bits con rango de 0 a 5 V", 35, 370, BLACK, -1);
  textout_ex(screen, font, "* Comunicacion serial multipunto con el microcontrolador a 9600 bauds.", 35, 400, BLACK, -1);
  textout_ex(screen, font, "Lea la hoja de especificaciones para mayor detalle", 350, 470, GREEN, -1);
  textout_ex(screen, font, "por Daniel Velazquez", 35, 700, BLACK, -1);
  textout_ex(screen, font, "Presione ENTER para Continuar", 600, 700, BLACK, -1);
  readkey();	    /* wait for a keypress */
  readkey();	    /* wait for a keypress */  

  clear_to_color(screen, WHITE);  /* clear the screen to white */
  textout_ex(screen, font, "  Software", 180, 120, BLACK, -1);
  textout_ex(screen, font, "El software de control pide al inicio el registro del usuario y los  parametros de ", 35, 250, BLACK, -1);
  textout_ex(screen, font, "control a utilizar, al terminar el registro corre una gráfica que muestra en tiempo", 35, 280, BLACK, -1);
  textout_ex(screen, font, "real el valor de la variable de proceso (PV), el valor deseado (SP) y la salida de ", 35, 310, BLACK, -1);
  textout_ex(screen, font, "control (OP) . Para aumentar el valor deseado se utilizan las fechas del teclado,", 35, 340, BLACK, -1);
  textout_ex(screen, font, " las cuales incrementarán a razón de 1% el SP, pero las teclas PageUp y PageDn", 35, 370, BLACK, -1);
  textout_ex(screen, font, "aumentarán el valor deseado en 10%.", 35, 400, BLACK, -1);
  textout_ex(screen, font, "by Daniel Velazquez", 35, 700, BLACK, -1);
  textout_ex(screen, font, "Press ENTER to Continue", 600, 700, BLACK, -1);
  readkey();

  Automatic();
  exit;
  release_screen();
  destroy_font(font);
  Automatic();
  return 0;
}

END_OF_MAIN()
