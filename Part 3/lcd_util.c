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
#include "lcd_util.h"

#define PADDLE_HEIGHT	8
#define BALL_RADIUS		3	
	
#define RED_LED_PIN		7
#define RED_LED_PORT	PORTD
#define RED_LED_DDR		DDRD

#define GREEN_LED_PIN	0
#define GREEN_LED_DDR	DDRB
#define GREEN_LED_PORT	PORTB

#define BLUE_LED_PIN	2
#define BLUE_LED_DDR	DDRB
#define BLUE_LED_PORT	PORTB

extern Paddle left_p, right_p;
extern Ball ball;
extern Score left_s, right_s;

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
	
	// Draw dotted line in the center
	draw_dotted_line();
}

void draw_dotted_line()
{
	// Draw the dotted line
	draw_dotted_line_vert(buff, 64, 1);
}

void draw_static_pong_gui()
{
	clear_buffer(buff);
	
	draw_gridlines();
	
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

void paddle_init(bool refresh)
{
	// left_p paddle
	left_p.height = PADDLE_HEIGHT;
	left_p.x_start = 2;
	left_p.y_start = 26;	// Middle
	draw_paddle(left_p, refresh);
	
	// right_p paddle
	right_p.height = PADDLE_HEIGHT;
	right_p.x_start = 126;
	right_p.y_start = 26;	// Middle	
	draw_paddle(right_p, refresh);
}

void ball_init(bool refresh)
{
	// Initialize in the centre of the screen
	ball.radius = BALL_RADIUS;
	ball.x_centre = 64;
	ball.y_centre = 32;
	
	draw_ball(ball, refresh);	
}

void score_init(bool refresh)
{
	// Left score
	left_s.x_units = 48;
	left_s.line = 0;
	left_s.x_tens = 40;
	left_s.s[0] = 0;
	left_s.s[1] = 0;
	draw_score(left_s, refresh);
	
	// Right score
	right_s.x_units = 75;
	right_s.line = 0;
	right_s.x_tens = 68;
	right_s.s[0] = 0;
	right_s.s[1] = 0;
	draw_score(right_s, refresh);
}

void update_paddle_position(unsigned int y, Side side, bool refresh)
//void update_paddle_position(uint8_t y, Side side, bool refresh)
{
	if(side == LEFT)
	{
		// Clear the old paddle
		clear_paddle(left_p, refresh);
	
		if (y + PADDLE_HEIGHT > 63)
			left_p.y_start = 63 - PADDLE_HEIGHT;
		if (y < 0)
			left_p.y_start = 0;
		else
			left_p.y_start = y;
		
		// Draw the new paddle
		draw_paddle(left_p, refresh);
	}
	else if (side == RIGHT)
	{
		// Clear the old paddle
		clear_paddle(right_p, refresh);

		if (y + PADDLE_HEIGHT > 63)
			right_p.y_start = 63 - PADDLE_HEIGHT;
		else if (y < 0)
			right_p.y_start = 0;
		else
			right_p.y_start = y;
			
		
		
		// Draw the new paddle
		draw_paddle(right_p, refresh);
	}
}

void update_ball_position(uint8_t x, uint8_t y, bool refresh)
{
	// Clear old ball
	clear_ball(ball, refresh);
	
	ball.x_centre = x;
	ball.y_centre = y;
	
	// Draw the ball at new location
	draw_ball(ball, refresh);
}

void update_score(uint8_t digit_units, uint8_t digit_tens, Side side, bool refresh)
{
	if (side == LEFT)
	{
		// Clear the old score
		clear_score(left_s, refresh);
		
		left_s.s[0] = digit_units;
		left_s.s[1] = digit_tens;

		// Draw the new score
		draw_score(left_s, refresh);
	}
	else if (side == RIGHT)
	{
		
		// Clear the old score
		clear_score(right_s, refresh);
		
		right_s.s[0] = digit_units;
		right_s.s[1] = digit_tens;
		
		// Draw the new score
		draw_score(right_s, refresh);
	}
}

void draw_paddle(Paddle paddle, bool refresh)
{
	drawline(buff, paddle.x_start, paddle.y_start, paddle.x_start, paddle.y_start + paddle.height, 1);
	drawline(buff, paddle.x_start + 1, paddle.y_start, paddle.x_start + 1, paddle.y_start + paddle.height, 1);
	
	if (refresh)
		write_buffer(buff);
}

void clear_paddle(Paddle paddle, bool refresh)
{
	clearline(buff, paddle.x_start, paddle.y_start, paddle.x_start, paddle.y_start + paddle.height, 1);
	clearline(buff, paddle.x_start + 1, paddle.y_start, paddle.x_start + 1, paddle.y_start + paddle.height, 1);
	
	if (refresh)
		write_buffer(buff);
}

void draw_ball(Ball ball, bool refresh)
{
	fillcircle(buff, ball.x_centre, ball.y_centre, ball.radius, 1);
	
	if (refresh)
		write_buffer(buff);
}

void clear_ball(Ball ball, bool refresh)
{
	clearfilledcircle(buff, ball.x_centre, ball.y_centre, ball.radius, 1);
	
	if (refresh)
		write_buffer(buff);
}

void draw_score(Score score, bool refresh)
{
	// Display tens digit only if score is greater than 9
	if (score.s[1] > 0)
		drawchar(buff, score.x_tens, score.line, score.s[1] + 48);	// 48 - ASCII for 0
		
	drawchar(buff, score.x_units, score.line, score.s[0] + 48);
	
	if (refresh)
		write_buffer(buff);
}

void clear_score(Score score, bool refresh)
{
	if (score.s[1] > 0)
		clearchar(buff, score.x_tens, score.line, score.s[1] + 48);	// 48 - ASCII for 0
	
	clearchar(buff, score.x_units, score.line, score.s[0] + 48);
	
	if (refresh)
		write_buffer(buff);
}

void lcd_led_init()
{
	RED_LED_DDR |= (1 << RED_LED_PIN);
	GREEN_LED_DDR |= (1 << GREEN_LED_PIN);
	BLUE_LED_DDR |= (1 << BLUE_LED_PIN);
	
	// Default color - Blue
	BLUE_LED_PORT |= (1 << BLUE_LED_PIN);		
}

void change_lcd_color(Color color)
{
	if (color == RED)
	{		
		RED_LED_PORT |= (1 << RED_LED_PIN);			// Switch on RED
		GREEN_LED_PORT &= ~(1 << GREEN_LED_PIN);	// Switch off GREEN
		BLUE_LED_PORT &= ~(1 << BLUE_LED_PIN);		// Switch off BLUE		  		
	}
	else if (color == GREEN)
	{
		RED_LED_PORT &= ~(1 << RED_LED_PIN);			// Switch off RED
		GREEN_LED_PORT |= (1 << GREEN_LED_PIN);	// Switch on GREEN
		BLUE_LED_PORT &= ~(1 << BLUE_LED_PIN);		// Switch off BLUE
	}	
	else if (color == BLUE)
	{
		RED_LED_PORT &= ~(1 << RED_LED_PIN);			// Switch off RED
		GREEN_LED_PORT &= ~(1 << GREEN_LED_PIN);	// Switch off GREEN
		BLUE_LED_PORT |= (1 << BLUE_LED_PIN);		// Switch on BLUE
	}
}

void refresh_screen()
{
	write_buffer(buff);
}