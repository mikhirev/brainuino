/*
    Brainuino Aleph

    Copyright (C) 2011-2012  Dmitry Mikhirev

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


#include "lcdprint.h"
#include "font_ru-en.h"
//#include "font_en.h"

char* convert(utf8 str)
{
  int32_t ucode;
  int i, j;
  int numcodes = sizeof(charmap)/sizeof(charcode);
  char result[str.chars];

  for (i = 0; i < str.chars; i++) {
    ucode = str.get();
    if (ucode > 0x0000) {
      if (ucode <= 0x007d) {
        result[i] = char(ucode);
      } else {
        result[i] = 0xff;
        for (j = 0; j < numcodes && charmap[j].uni < ucode; j++) {
            if (charmap[j].uni == ucode) {
              result[i] = charmap[j].font;
            }
        }
      }
    } else {
      result[i] = 0x00;
      break;
    }
  }

  return result;
}
