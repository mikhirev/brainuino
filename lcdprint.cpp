/*
    Brainuino Aleph

    Copyright (C) 2011, 2012, 2014  Dmitry Mikhirev

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

size_t uprint(const char* rawstr, Print* out)
{
  utf8 str = utf8(rawstr);
  return utf8print(&str, out);
}

size_t uprint(const __FlashStringHelper* rawstr, Print* out)
{
  utf8 str = utf8(rawstr);
  return utf8print(&str, out);
}

size_t utf8print(utf8* str, Print* out)
{
  char result[str->chars() + 1];
  wchar_t ucode;
  uint16_t i;

  for (i = 0; i < (str->chars()); i++) {
    ucode = str->get();
    if (ucode != 0x0000) {
      result[i] = codeOf(ucode);
    } else {
      break;
    }
  }
  result[i] = '\0';
  return out->print(result);
}

char codeOf(wchar_t ucode)
{
  if (ucode > 0x0000 && ucode <= 0x007d) // ASCII symbol
    return char(ucode);

  int low, high, mid;

  low = 0;
  high = sizeof(charmap)/sizeof(charcode) - 1;
  while (low <= high) {
    mid = (low + high) >> 1;
    wchar_t u = pgm_read_word(&charmap[mid].uni);
    if (wchar_t(u) > ucode)
      high = mid - 1;
    else if (u < ucode)
      low = mid + 1;
    else
      return pgm_read_byte(&charmap[mid].font);
  }
  return NOCHAR;
}
