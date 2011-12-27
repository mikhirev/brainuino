/*
    Brainuino Aleph

    Copyright (C) 2011  Dmitry Mikhirev

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


/* arduino pinout
digital pins 0 and 1 reserved for PC connection
digital pins 2 and 3 must be used for interrupts */

#ifndef pinout_h
#define pinout_h

// digital pins connected to LCD display
#define LCD4    7
#define LCD6    8
#define LCD11   9
#define LCD12   10
#define LCD13   11
#define LCD14   12
/* pins 1 and 5 of LCD must be connected to GND,
pin 2 - to +5V
pin 3 - to 10K potentiometer
pins 15 and 16 (both optional) - to +5V (thru 50-100 Ohm resistor)
and GND respectively
pins 7-10 - no connection */

// digital pin connected to speaker
#define SPEAKER 5

// digital pin connected to LED lamp
#define GREENLAMP 13
#define REDLAMP   6

// digital pins connected to players' buttons
#define BUTTON1 14
#define BUTTON2 15
#define BUTTON3 16
#define BUTTON4 17

// digital pins connected to control buttons
// start/OK button
#define CONTROL1 18
// break/cansel button
#define CONTROL2 19

// digital pin connected to encoder button
#define ENCPUSH 20

// digital pin connected to encoder 'B' output
#define ENCODERB   4
// encoder 'A' output must be connected to Arduino D3

#endif
