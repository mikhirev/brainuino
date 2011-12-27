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

#include "lcdrus.h"

String convert(String str)
{
  struct toFrom {
    prog_char to;
    prog_char from[5];
  };
  toFrom symbols[] PROGMEM = {{0x41, "А"},
    {0x42, "В"},
    {0x43, "С"},
    {0x45, "Е"},
    {0x48, "Н"},
    {0x4B, "К"},
    {0x4D, "М"},
    {0x4F, "О"},
    {0x50, "Р"},
    {0x54, "Т"},
    {0x58, "Х"},
    {0x61, "а"},
    {0x62, "Ь"},
    {0x63, "с"},
    {0x65, "е"},
    {0x6F, "о"},
    {0x70, "р"},
    {0x78, "х"},
    {0x79, "у"},
    {0x7E, "↵"},
    {0xA0, "Б"},
    {0xA1, "Г"},
    {0xA2, "Ё"},
    {0xA3, "Ж"},
    {0xA4, "З"},
    {0xA5, "И"},
    {0xA6, "Й"},
    {0xA7, "Л"},
    {0xA8, "П"},
    {0xA9, "У"},
    {0xAA, "Ф"},
    {0xAB, "Ч"},
    {0xAC, "Ш"},
    {0xAD, "Ъ"},
    {0xAE, "Ы"},
    {0xAF, "Э"},
    {0xB0, "Ю"},
    {0xB1, "Я"},
    {0xB2, "б"},
    {0xB3, "в"},
    {0xB4, "г"},
    {0xB5, "ё"},
    {0xB6, "ж"},
    {0xB7, "з"},
    {0xB8, "и"},
    {0xB9, "й"},
    {0xBA, "к"},
    {0xBB, "л"},
    {0xBC, "м"},
    {0xBD, "н"},
    {0xBE, "п"},
    {0xBF, "т"},
    {0xC0, "ч"},
    {0xC1, "ш"},
    {0xC2, "ъ"},
    {0xC3, "ы"},
    {0xC4, "ь"},
    {0xC5, "э"},
    {0xC6, "ю"},
    {0xC7, "я"},
    {0xC8, "«"},
    {0xC9, "»"},
    {0xCA, "〟"},
    {0xCB, "〞"},
    {0xCD, "¿"},
    {0xCE, "ƒ"},
    {0xCF, "£"},
    {0xD3, "‼"},
    {0xD9, "↑"},
    {0xDA, "↓"},
    {0xE0, "Д"},
    {0xE1, "Ц"},
    {0xE2, "Щ"},
    {0xE3, "д"},
    {0xE4, "ф"},
    {0xE5, "ц"},
    {0xE6, "щ"},
    {0xE9, "~"},
    {0xEA, "é"},
    {0xEB, "ç"},
    {0xF0, "¼"},
    {0xF1, "⅓"},
    {0xF2, "½"},
    {0xF3, "¾"},
    {0xFD, "§"},
    {0xFE, "¶"}};
  
  uint8_t i;
  
  for (i = 0; i < sizeof(symbols)/sizeof(toFrom); i++)
    str = str.replace((char*)symbols[i].from, symbols[i].to);
  return str;
}
