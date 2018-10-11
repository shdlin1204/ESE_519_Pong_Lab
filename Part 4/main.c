#define F_CPU 16000000UL
#define BAUD 9600

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "uart.h"
#include <stdio.h>

#include "lcd.h"
#include "lcd_util.h"
#include "pong_game.h"

extern Paddle left_p, right_p;
extern Score left_s;

uint16_t adc_out;
uint16_t adc_val[10];
uint8_t raw_paddle_y;
uint8_t num_overflows = 0;

void timer0_init()
{
	TCCR0B |= (1 << CS00) | (1 << CS02);      // Enable the timer with 1024 prescaling, Normal timer
	
	TCNT0 = 0;	// Initialize the timer count value to 0
	
	TIMSK1 = (1 <<  TOIE0);		// Enable overflow interrupt

	sei();
}

ISR(TIMER0_OVF_vect)
{
	if (num_overflows == 6)
	{
		num_overflows = 0;
	}
}

void adc_init()
{
	ADCSRA |= (1 << ADEN);   // Enable ADC
	
	ADMUX |= (1 << REFS0);   // Set AVcc as voltage reference and by default
	
	ADMUX |= (1 << MUX2);			//set ADC4 as input
	
	ADCSRA |= (1 << ADIE);	// Enable interrupt
		
	//ADCSRA |= (1 << ADATE);   // Set ADC to auto trigger mode (free running for ADTS = '000' in ADCSRB register)
	
	ADCSRB |= (1 << ADTS2);	// Timer 0 overflow
	
	ADCSRA |= (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);  // Set prescaler to 128(To achieve frequency 125KHz) for '111'
		
	ADCSRA |= (1 << ADSC);	// Start the conversion
	
	sei();
}


ISR(ADC_vect)
{
	//printf("interrupt\n");
	adc_out = ADC;
	//raw_paddle_y = 195 - 0.52 * adc_out;	// Paddle position
	//raw_paddle_y = 230.75 - 0.65 * adc_out;	// Paddle position
	raw_paddle_y = 280 - 0.8 * adc_out;	// Paddle position
	//update_paddle_position(raw_paddle_y, LEFT, 1);
	
	ADCSRA |= (1 << ADSC);	// Start the conversion again
	//left_p.y_start = y;
}

int getMovingAvg(int sensor_Reading[], long *sum, int pos, int len, int next_reading)
{
	//Subtract the oldest number from the previous sum, add the new number
	*sum = *sum - sensor_Reading[pos] + next_reading;
	printf("Sum:%d\n", *sum);
	//Assign the nextNum to the position in the array
	sensor_Reading[pos] = next_reading;
	
	//return the average
	return *sum / len;
}

int movingAvg(int sensor_reading[], int count)
{
	// the size of this array represents how many numbers will be used
	// to calculate the average
	int reading[10] = {0};

	int pos = 0;
	int newAvg = 0;
	long sum = 0;
	int len = sizeof(reading) / sizeof(int);

	for(int i = 0; i < count; i++){
		newAvg = getMovingAvg(reading, &sum, pos, len, sensor_reading[i]);
		//printf("The new average is %d\n", newAvg);
		pos++;
		if (pos >= len){
			pos = 0;
		}
	}
	return newAvg;
}

int main(void)
{	
	char result[64];
	
	
	uart_init();
	lcd_init();
	lcd_led_init();
	lcd_command(CMD_DISPLAY_ON);
	lcd_set_brightness(0x18);
	
	clear_buffer(buff);
	
	int sensor_reading_min[15];
	int sensor_reading_max[15];
	int min_reading = 0, max_reading = 0;
	char c;
	
	adc_init();	
	_delay_ms(500);
	
	/*==========================================================
	  This code was used for generating equations(Used only once)
	============================================================*/
	
	/*
	// Get the sensor readings
	printf("Get the min value \n");
	
	for (int i = 0; i < 15; i++)
	{
		sensor_reading_min[i] = adc_out; 
		printf("Reading: %d\n", adc_out);
		_delay_ms(20);
	}
	
	// Get the sensor readings
	printf("Get the max value \n");
	
	for (int i = 0; i < 15; i++)
	{
		sensor_reading_max[i] = adc_out;
		printf("Reading: %d\n", adc_out);
		_delay_ms(20);
	}
		
	// Calculate the moving averages for both min and max values
	int count = sizeof(sensor_reading_min) / sizeof(int);
	min_reading = movingAvg(sensor_reading_min, count);
	
	count = sizeof(sensor_reading_max) / sizeof(int);
	//max_reading = movingAvg(sensor_reading_max, count);
	
	_delay_ms(1000);
	printf("Min reading: %d, Max reading %d\n", min_reading, max_reading);

	
	*/
	
	start_game();
	
	int y;
	while (1)
	{
		//printf("adc: %u paddle: %u\n", adc_out, raw_paddle_y);
	}
}