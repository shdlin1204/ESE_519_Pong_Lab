/*
 * pong_game.h
 *
 * Created: 03-10-2018 20:51:02
 *  Author: rgjus
 */ 


#ifndef PONG_GAME_H_
#define PONG_GAME_H_

#include <stdio.h>

typedef enum _side
{
	LEFT = 0,
	RIGHT
}Side;

typedef struct _paddle
{
	uint8_t x_start;
	uint8_t y_start;
	uint8_t height;
	
}Paddle;

typedef struct _ball
{
	uint8_t x_centre;
	uint8_t y_centre;
	uint8_t radius;
		
}Ball;

typedef struct _score
{
	uint8_t x_tens;
	uint8_t x_units;
	uint8_t line;
	uint8_t s[2];		// Oth byte is units digit, 1st byte is tens digit
		
}Score;

void start_game();

#endif /* PONG_GAME_H_ */