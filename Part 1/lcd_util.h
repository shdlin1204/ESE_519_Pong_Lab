/*
 * lcd_util.h
 *
 * Created: 03-10-2018 18:28:53
 *  Author: rgjus
 */ 

#ifndef LCD_UTIL_H_
#define LCD_UTIL_H_

void draw_initial_pong_gui();
void lcd_test();
void draw_gridlines();
void draw_dotted_line_vert(uint8_t* buff, uint8_t x0, uint8_t y0);
void draw_paddle(uint8_t* buff, uint8_t x_start, uint8_t y_start, uint8_t height);


#endif /* LCD_UTIL_H_ */