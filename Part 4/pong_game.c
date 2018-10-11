/*
 * pong_game.c
 *
 * Created: 03-10-2018 20:50:45
 *  Author: rgjus
 */ 
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "pong_game.h"
#include "lcd_util.h"

#define BALL_CENTER_X	64
#define BALL_CENTER_Y	32
#define BALL_RADIUS		3		
#define	BALL_Y_MIN		5	// 1 + 3 (1 - Upper grid line, 3 - radius)
#define BALL_Y_MAX		60	
#define BALL_X_MIN		5
#define BALL_X_MAX		123

Paddle left_p, right_p;
Ball ball;
Score left_s, right_s;
bool ball_v_dir = 0;	// 0 - upwards, 1 - downwards
bool ball_h_dir = 0;	// 0 - left, 1 - right
extern uint8_t raw_paddle_y;
volatile uint16_t ctr = 0;
uint16_t ctr_max;

void timer1_init(uint8_t timercount)  // Initialize the timer and interrupts
{
	TCNT1 = 0;	// Initialize the timer count value to 0
	TCCR1A |= (1 << COM1A0);	// Sets the toggle OC1A on Compare Match mode
	OCR1A = timercount;					// Initialize the output compare value   71 for 436 Hz; 35 for 870 Hz
	TIMSK1 |= (1 << OCIE1A);		// Enable output compare A match interrupt enable
	TCCR1B |= (1 << CS12);      // Enable the timer with prescaler(256) specification
	sei();						// Enable global interrupts
}

ISR(TIMER1_COMPA_vect)
{
	TCNT1 = 0;
	ctr++;
}

void play_buzzer(uint8_t timercount)
{
	DDRB |= 0x02;  //set PB1 to output
	PORTB = 0;
	long int countms = 0;
	
	ctr = 0;
	
	// Playing buzzer for 500 ms
	if (timercount == 71)
	ctr_max = 220;	// 220
	else
	ctr_max = 10;	//435
	
	while(ctr <= ctr_max)
	{
		printf("c\n");
		timer1_init(timercount);
	}
	ctr = 0;
	
	TIMSK1 = 0;		// Disable output compare A match interrupt enable
	DDRB &= ~(1 << 1);
}

Side get_paddle_side(uint8_t x)
{
	if (x >=0 && x < 40)
		return LEFT;
	else if (x >= 80 && x <= 128)
		return RIGHT;
}

bool is_hit(uint8_t x, uint8_t y)
{	
	if ((y >= left_p.y_start && y <= left_p.y_start + left_p.height && x == BALL_X_MIN) ||
	(y >= right_p.y_start && y <= right_p.y_start + right_p.height && x == BALL_X_MAX))
		return 1;
	
	return 0;
}


void increment_score(uint8_t current_score_units, uint8_t current_score_tens, uint8_t* updated_score_units, uint8_t* updated_score_tens)
{
	// If score reaches 99, reset it
	if (current_score_units == 9 && current_score_tens == 9)	
	{
		*updated_score_units = 0;
		*updated_score_tens = 0;		
	}
	else if (current_score_units == 9)
	{
		*updated_score_units = 0;
		*updated_score_tens = current_score_tens + 1;
	}
	else
	{
		*updated_score_units = current_score_units + 1;
		*updated_score_tens = current_score_tens;
	}
	
}

void start_game()
{
	// Draw static GUI components
	draw_static_pong_gui();
	
	// Draw dynamic GUI components
	paddle_init(0);
	ball_init(0);
	score_init(0);
	refresh_screen();
	
	uint8_t x = BALL_CENTER_X, y = BALL_CENTER_Y;
	uint8_t random_num_x, random_num_y;
	uint8_t ball_highest_pt, ball_lowest_pt;
	uint8_t updated_score_units = 0;
	uint8_t updated_score_tens = 0;
	uint8_t update_paddle_ctr = 0;
	
	while(true)
	{
		update_paddle_ctr++;
		
		// Decrease the refresh rate of left paddle
		if (update_paddle_ctr == 7)
		{		
			update_paddle_position(raw_paddle_y, LEFT, 1);
			update_paddle_ctr = 0;
		}
		
		// Check boundary conditions
		if (y == BALL_Y_MIN || y == BALL_Y_MAX)	// Y-boundary
		{
			ball_v_dir ^= 1;	// Toggle		
		}
		if (x == BALL_X_MIN || x == BALL_X_MAX)	// X-boundary
		{
			// Automatic computer player on right side
			Side s = get_paddle_side(x);
			if (s == RIGHT)
			{
				// Update
				ball_highest_pt = ball.y_centre - ball.radius;
				ball_lowest_pt = ball.y_centre + ball.radius; 
				if (!(ball_highest_pt > right_p.y_start && ball_highest_pt < (right_p.y_start + right_p.height) &&
					ball_lowest_pt > right_p.y_start && ball_lowest_pt < (right_p.y_start + right_p.height)))
					update_paddle_position(y - BALL_RADIUS, s, 1);
			}
			
			if (is_hit(x, y))
			{
				// Turn on Green LED for sometime and then switch back to blue
				//change_lcd_color(GREEN);
				
				// Increment score 	
				Side paddle_side = get_paddle_side(x);
				updated_score_units = 0; updated_score_tens = 0; 
				increment_score((paddle_side ? right_s.s[0] : left_s.s[0]), (paddle_side ? right_s.s[1] : left_s.s[1]),
					&updated_score_units, &updated_score_tens);
				update_score(updated_score_units, updated_score_tens, paddle_side, 1);
				
				//change_lcd_color(BLUE);
				
				// Play buzzer
				play_buzzer(35);
			}
			else   // Miss
			{
				// Turn on Red LED for sometime and then switch back to blue
				change_lcd_color(RED);
				
				// Reset the ball position to center but it should move in opposite direction horizontally
				//update_ball_position(BALL_CENTER_X, BALL_CENTER_Y, 0);
				x = BALL_CENTER_X;
				y = BALL_CENTER_Y;
				
				// Update the paddles position to center
				update_paddle_position(26, LEFT, 0);
				update_paddle_position(26, RIGHT, 0);
				
				ball_v_dir = 1;
				play_buzzer(71);
				//_delay_ms(10000);
				
				change_lcd_color(BLUE);
				// Play buzzer
			}
			ball_h_dir ^= 1;		
		}
		
		// Random speed of ball in both directions(x and y)
		//random_num_x = (rand() % (3 - 1 + 1)) + 1;
		//random_num_y = (rand() % (3 - 1 + 1)) + 1;	
		
		random_num_x = (rand() % (2 - 1 + 1)) + 1;
		random_num_y = (rand() % (2 - 1 + 1)) + 1;
		
		if (ball_v_dir == 0 && y > BALL_Y_MIN)	// Upwards
			y -= random_num_y;
		if (ball_v_dir == 1 && y < BALL_Y_MAX)	// Downwards
			y += random_num_y;
		if (ball_h_dir == 0 && x > BALL_X_MIN)
			x -= random_num_x;
		if (ball_h_dir == 1 && x < BALL_X_MAX)
			x += random_num_x;
			
		// Check if the ball is crossing any boundary
		if (x  < BALL_X_MIN)
			x = BALL_X_MIN;
		if (x  > BALL_X_MAX)
			x = BALL_X_MAX;
		if (y < BALL_Y_MIN)
			y = BALL_Y_MIN;
		if (y > BALL_Y_MAX)
			y = BALL_Y_MAX;
			
		update_ball_position(x, y, 0);
		
		// Redraw GUI components which may get deleted due to movement of ball
		draw_score(left_s, 0);
		draw_score(right_s, 0);
		draw_paddle(left_p, 0);
		draw_paddle(right_p, 0);
		draw_dotted_line();
		refresh_screen();
	}
}