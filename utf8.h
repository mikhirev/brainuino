/*
    Brainuino Aleph

    Copyright (C) 2012-2014  Dmitry Mikhirev

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

#ifndef utf8_h
#define utf8_h

#include <avr/pgmspace.h>
#include "Arduino.h"

class utf8 {
  public:
    utf8(const char* string);
    utf8 (const __FlashStringHelper* flashString);
    ~utf8(void);
    wchar_t get();
    uint16_t chars();
    uint16_t bytes();

  private:
    uint16_t _index;
    uint16_t _chars;
    uint16_t _bytes;
    char* _string;
    char* _string_p;
};

#endif
