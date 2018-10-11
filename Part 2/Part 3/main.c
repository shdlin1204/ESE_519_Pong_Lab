/*
 * Lab4_3.0.c
 *
 * Created: 03-10-2018 18:32:28
 * Author : rgjus
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include "uart.h"
#include <util/delay.h>

#include "lcd.h"
#include "lcd_util.h"
#include "pong_game.h"

extern Paddle left_p, right_p;
extern Score left_s;


int main(void)
{
	lcd_init();
	lcd_led_init();
	uart_init();
	adc_init();
	
	lcd_command(CMD_DISPLAY_ON);
	lcd_set_brightness(0x18);
	
	clear_buffer(buff);
	
	start_game();
	
	
    while (1) 
    {
    }
}

