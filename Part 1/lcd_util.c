/*
 * lcd_util.c
 *
 * Created: 03-10-2018 18:26:23
 *  Author: rgjus
 */ 
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>

#include "lcd.h"

void lcd_test()
{
	clear_buffer(buff);
	
	// Draw pixels
	setpixel(buff, 1, 1);
	setpixel(buff, 1, 9);
	setpixel(buff, 1, 5);
	setpixel(buff, 1, 24);
	setpixel(buff, 20, 64);
	write_buffer(buff);
	_delay_ms(50000);
	
	// Clear pixels
	clearpixel(buff, 1, 1);
	clearpixel(buff, 1, 9);
	clearpixel(buff, 1, 5);
	clearpixel(buff, 1, 24);
	clearpixel(buff, 20, 64);
	write_buffer(buff);
	_delay_ms(50000);
	clear_buffer(buff);
	
	// Draw lines
	drawline(buff, 1, 1, 128, 1, 1);	// Horizontal
	drawline(buff, 32, 1, 32, 64, 1);	// Vertical
	write_buffer(buff);
	_delay_ms(50000);
	clear_buffer(buff);
	
	// Draw String
	drawstring(buff, 10, 2, "Welcome!");
	write_buffer(buff);
	_delay_ms(50000);
	clear_buffer(buff);

	// Draw rectangle
	drawrect(buff, 10, 10, 20, 20, 1);
	write_buffer(buff);
	_delay_ms(50000);
	clear_buffer(buff);
	
	// Draw fill rectangle
	fillrect(buff, 10, 10, 20, 20, 1);
	write_buffer(buff);
	_delay_ms(50000);
	clear_buffer(buff);
	
	// Draw Circle
	drawcircle(buff, 60, 30, 5, 1);
	write_buffer(buff);
	_delay_ms(50000);
	clear_buffer(buff);
	
	// Draw filled Circle
	fillcircle(buff, 60, 30, 5, 1);
	write_buffer(buff);
	_delay_ms(50000);
	clear_buffer(buff);
	
}

void draw_gridlines()
{
	// Draw the top horizontal line
	drawline(buff, 1, 1, 128, 1, 1);
	
	// Draw the bottom horizontal line
	drawline(buff, 1, 64, 128, 64, 1);
	
	// Draw the left vertical line
	drawline(buff, 1, 1, 1, 64, 1);
	
	// Draw the right vertical line
	drawline(buff, 128, 1, 128, 64, 1);
	
	// Draw the dotted line
	draw_dotted_line_vert(buff, 64, 1);
}

void draw_initial_pong_gui()
{
	clear_buffer(buff);
	
	draw_gridlines();
	
	// Draw left hand side paddle
	draw_paddle(buff, 3, 5, 8);
	
	// Draw right hand side paddle
	draw_paddle(buff, 125, 50, 8);
	
	// Draw ball
	fillcircle(buff, 45, 40, 5, 1);
	
	// Left score
	drawchar(buff, 48, 0, '0');
	
	// Right score
	drawchar(buff, 74, 0, '0');
	
	write_buffer(buff);
}

void draw_dotted_line_vert(uint8_t* buff, uint8_t x0, uint8_t y0)
{
	for(int i = 0; i < 64; i = i + 4)
	{
		setpixel(buff,x0, y0 + i);
		setpixel(buff,x0, y0 + i + 1);
	}
}

void draw_paddle(uint8_t* buff, uint8_t x_start, uint8_t y_start, uint8_t height)
{
	drawline(buff, x_start, y_start, x_start, y_start + height, 1);
	drawline(buff, x_start + 1, y_start, x_start + 1, y_start + height, 1);
}
