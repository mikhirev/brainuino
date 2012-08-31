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

#include <LiquidCrystal.h>
#include "lcdprint.h"
#include "font.h"

size_t uprint(utf8 str, LiquidCrystal *lcd)
{
  int32_t ucode;
  int i, j;
  int numcodes = sizeof(charmap)/sizeof(charcode);
  char result[str.chars];

  for (i = 0; i < str.chars; i++) {
    ucode = str.get();
    if (ucode > 0x0000) {
      if (ucode <= 0x007d) {
        result[i] = ucode;
      } else {
        result[i] = 0xff;
        for (j = 0; (j < numcodes) && (pgm_read_dword(&charmap[j].uni) <= ucode); j++) {
            if (pgm_read_dword(&charmap[j].uni) == ucode) {
              result[i] = pgm_read_byte(&charmap[j].font);
            }
        }
      }
    } else {
      break;
    }
  }
  result[i] = 0;
  return lcd->print(result);
}
