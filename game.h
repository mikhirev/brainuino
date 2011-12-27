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

#ifndef game_h
#define game_h

// game types
#define BRAIN 0
#define SI    1
#define CHGK  2

uint8_t gameType;
uint8_t withFalseStart;
uint8_t timer1;
uint8_t timer2;
uint32_t timer;
uint8_t preSignal;
uint32_t startTime;
uint32_t time;
uint8_t buttonPressed;

#endif
