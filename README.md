# ESE_519_Pong_Lab

Part 1
The task was to create a graphics library with the following functions:
Set Pixel:
The very basic function of the graphics library is set_pixel. Since the organization of LCD’s memory buffer is a little convoluted, it takes some computation to draw a pixel on the screen.

The formula that we used to update the memory buffer to set a pixel is:
buff[(((int)((y - 1)/8) * 128) + x) - 1] |= (1 <<  (7 - ((y - 1) % 8)))

Draw line:
To draw a line, we used a loop starting from point (x0, y0) to (x1, y1) and drew pixel in each iteration. Since, only horizontal and vertical lines are required, we used the loop to set pixels from Point 1 to Point 2 and not any algorithm.
Similarly, for all other functions like rectangle and filled rectangle.

Draw Circle and fill circle:
We used midpoint circle algorithm to draw and fill circle.

Draw String:
Used draw char in a loop to draw the string, incrementing the x-position by 5 in each iteration because the character size is 5 pixels. 




Part 2
This part involves setting a pixel at the location where the touch screen is touched. 

ADC
To get ADC values from the touchscreen, we first introduced adc_init() function, in which we set the ADC to single conversion mode. We get ADC measurement of the X-axis of the touch screen by setting the X- and X+ as digital outputs, and Y- and Y+ as inputs. X- is set high and X+ is set low. Y- is selected as the ADC input channel. Similarly, to get ADC measurement of the Y-axis of the touch screen by setting the Y- and Y+ as digital outputs, and X- and X+ as inputs. Y- is set high and Y+ is set low. X- is selected as the ADC input channel. Interrupt is used to update XY coordinates. 

Calibration
The equations we used for calibration of the touch screen and LCD can be found in Lab 3 doc. 
To obtain ADC values from three reference points, we first printed the three corner points on the LCD: (X,Y) = (1,1), (128,1), and (128,64). We pressed those points and recorded their corresponding X and Y axis ADC values. The corresponding ADC outputs were (285,720), (268, 208), and (700,203). Through calculation, we got the final calibration equation: 

  Xd = AXt + BYt + C, where A = -0.0028698, B = -0.24795, C = 180.34; 
Yd = DXt + EYt + F, where D = 0.14578, E = -0.0048403, F = -37.062

Then we print the pixel with the calculated Xd and Yd values. 


Part 3
This part involves writing the entire game logic and integrating it with touch screen. 
The code structure is as follows:
lcd.c/lcd.h - It contains the basic graphics function like draw char, set pixel, draw line, etc.
lcd_util.c/lcd_util.h - It contains the functions to draw the basic elements of the pong game using the basic graphic library functions in lcd.c/lcd.h. This include functions like draw paddle & clear paddle, draw ball & clear ball, draw scores and clear scores, draw grid lines, etc.
pong _game.c/pong_game.h - It contains the main game function - start_game which runs a while loop and reads the values of the paddle from the touch screen and keeps updating the paddle positions, ball position, and scores. It also contains functions to take inputs from touch screen and calibrate them to act as paddle positions on LCD. It also contains the function to play buzzer whenever a paddle hit or paddle miss happens. It also changes the LCD backlight color when there is a paddle miss. 
The left paddle is controlled by touch screen inputs. The right paddle is a computer player. It updates its position as soon as a ball comes to make it a paddle hit. 

Buzzer
Buzzer was controlled by output compare using timer1 under PWM mode with a prescaler of 128. When a paddle misses the ball, the buzzer plays 440 Hz for 500 ms; when a paddle hits the ball, the buzzer plays 880 Hz for 100 ms. 




Part 4
This part implements the accelerometer and integrates with the Pong game as in Part 3. For the accelerometer, we used moving average filter to calculate the minimum and maximum values of accelerometer by rotating it along the x-axis. The minimum value was around 275 and the maximum value was around 355. So, we mapped them to the y-coordinates of the left paddle. The right paddle is again a computer player as in the first part. Rest of the game remains same as in part 3 except that the left paddle input now comes from the accelerometer readings as we rotate it along x axis instead of touch screen as in part 3.


The link of code on github:
https://github.com/ritgupta123/ESE_519_Pong_Lab
