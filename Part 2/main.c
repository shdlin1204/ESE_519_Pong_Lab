/*
 * Lab4_2.1.c
 *
 * Created: 2018/10/3 20:29:12
 * Author : shlda
 */ 

#define F_CPU 16000000UL
#define BAUD 9600

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "lcd.h"
#include "uart.h"

#define Y_N_PIN				0	// Y-negative
#define Y_P_PIN				2	// Y-positive
#define X_N_PIN				3	// X-negative
#define X_P_PIN				1	// X-positive
#define SCALING_FACTOR_X	0.125	// 128/1024
#define SCALING_FACTOR_Y	0.0625	// 64/1024

uint16_t adc_out;

void adc_init()
{
	ADMUX |= (1 << REFS0);   // Set AVcc as voltage reference

	ADCSRA |= (1 << ADEN);   // Enable ADC
	
	ADMUX |= (1 << ADC0D) | (1 << ADC2D);	// Set the ADC input channels
	
	ADCSRA |= (1 << ADIE);	// Enable interrupt
	
	//ADCSRA |= (1 << ADATE);   // Set ADC to auto trigger mode (free running for ADTS = '000' in ADCSRB register)
	
	ADCSRA |= (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);  // Set pre-scaler to 128(To achieve frequency 125KHz) for '111'
	
	//ADCSRA |= (1 << ADSC);	// Start the conversion
	
	sei(); 
}

ISR(ADC_vect)
{
	adc_out = ADC;
}

uint16_t measure_x_coordinate()
{
	uint16_t touch_x;
	
	// Update the pin values for output pins
	PORTC = 0;	// Reset all the earlier values of all pins
	PORTC |= (1 << X_N_PIN);	// Set X_N as high
	PORTC &= ~(1 << X_P_PIN);	// Set X_P as low
	
	// Update the direction register
	DDRC = 0;	// Reset all the earlier configuration of pins
	DDRC |= (1 << X_P_PIN) | (1 << X_N_PIN);	// Set the X_P and X_N as digital outputs
	DDRC &= ~(1 << Y_P_PIN) & ~(1 << Y_N_PIN);	// Set the Y_P and Y_N as ADC inputs, Not really required as by default they are inputs(0).
	
	//select ADC channel with safety mask
	ADMUX = (ADMUX & 0xF0) | (Y_N_PIN & 0x0F);
	//single conversion mode
	ADCSRA |= (1<<ADSC);
	// wait until ADC conversion is complete
	while( ADCSRA & (1<<ADSC) );
	
	// Read ADC value
	//ADMUX |= (1 << ADC0D);	// Change the ADC input channel
	touch_x = adc_out;
	_delay_ms(2);	// To stabilize ADC
	
	return touch_x;
}

uint16_t measure_y_coordinate()
{
	uint16_t touch_y;
	
	// Update the pin values for output pins
	PORTC = 0;	// Reset all the earlier values of all pins
	PORTC |= (1 << Y_N_PIN);	// Set Y_N as high
	PORTC &= ~(1 << Y_P_PIN);	// Set Y_P as low
	
	// Update the direction register
	DDRC = 0;	// Reset all the earlier configuration of pins
	DDRC |= (1 << Y_P_PIN) | (1 << Y_N_PIN);	// Set the Y_P and Y_N as digital outputs
	DDRC &= ~(1 << X_P_PIN) & ~(1 << X_N_PIN);	// Set the X_P and X_N as ADC inputs, Not really required as by default they are inputs(0).
	
	//select ADC channel with safety mask
	ADMUX = (ADMUX & 0xF0) | (X_N_PIN & 0x0F);
	//single conversion mode
	ADCSRA |= (1<<ADSC);
	// wait until ADC conversion is complete
	while( ADCSRA & (1<<ADSC) );
	
	// Read ADC value
	//ADMUX |= (1 << ADC3D);	// Change the ADC input channel
	_delay_ms(2);	// To stabilize ADC
	touch_y = adc_out;
	
	return touch_y;
}

void get_lcd_coordinates(uint16_t touch_x, uint16_t touch_y, uint16_t* lcd_x, uint16_t* lcd_y)
{
	// Calibrate
	//*lcd_x = SCALING_FACTOR_X * ги173.895 - 0.21882*touch_y);
	//*lcd_y = SCALING_FACTOR_Y * (0.12788*touch_x - 39.233);
	*lcd_x = 180.34 - 0.0028698*touch_x - 0.24795*touch_y;
	*lcd_y = 0.14578*touch_x - 0.0048403*touch_y - 37.062;
}

int main(void)
{
	uint16_t lcd_x, lcd_y;
	uint16_t touch_x, touch_y;
	
	adc_init();
	lcd_init();
	uart_init();
	
	lcd_command(CMD_DISPLAY_ON);
	lcd_set_brightness(0x18);
	clear_buffer(buff);
	setpixel(buff, 10, 10);
	
	while (1)
	{
		touch_x = measure_x_coordinate();
		touch_y	= measure_y_coordinate();
		
		get_lcd_coordinates(touch_x, touch_y, &lcd_x, &lcd_y);
		
		setpixel(buff, lcd_x, lcd_y);
		write_buffer(buff);
		
		//printf("X: %u, Y: %u\n", lcd_x,lcd_y);
		printf("touchX: %u, touchY: %u; lcd_X: %u, lcd_Y: %u\n", touch_x,touch_y,lcd_x,lcd_y);
	}
}