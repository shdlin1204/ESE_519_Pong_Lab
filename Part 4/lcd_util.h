/*
 * lcd_util.h
 *
 * Created: 03-10-2018 18:28:53
 *  Author: rgjus
 */ 


#ifndef LCD_UTIL_H_
#define LCD_UTIL_H_

#include <stdbool.h>

#include "pong_game.h"

typedef enum _color
{
	RED,
	GREEN,
	BLUE
}Color;

void draw_static_pong_gui();
void lcd_test();
void draw_gridlines();
void draw_dotted_line();
void draw_dotted_line_vert(uint8_t* buff, uint8_t x0, uint8_t y0);
void paddle_init(bool refresh);
void ball_init(bool refresh);
void score_init(bool refresh);
void draw_paddle(Paddle paddle, bool refresh);
void clear_paddle(Paddle paddle, bool refresh);
void update_paddle_position(uint8_t y, Side side, bool refresh);
void update_ball_position(uint8_t x, uint8_t y, bool refresh);
void update_score(uint8_t digit_units, uint8_t digit_tens, Side side, bool refresh);
void draw_ball(Ball ball, bool refresh);
void clear_ball(Ball ball, bool refresh);
void draw_score(Score score, bool refresh);
void clear_score(Score score, bool refresh);
void lcd_led_init();
void change_lcd_color(Color color);
void refresh_screen();

#endif /* LCD_UTIL_H_ */