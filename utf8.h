/*
    Brainuino Aleph

    Copyright (C) 2012  Dmitry Mikhirev

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

#include "Arduino.h"

class utf8 {
  public:
    utf8(char* string);
    ~utf8(void);
    uint16_t chars;
    uint16_t bytes;
    int32_t get();
    char* string;

  private:
    uint16_t _index;
};

#endif
