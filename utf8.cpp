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

#include "utf8.h"
#include <string.h>

utf8::utf8 (const char* input)
{
  _bytes = strlen(input);
  _string = strdup(input);
  _string_p = NULL;
  _chars = 0;
  for (_index = 0; _index < _bytes; _index++) {
    if ((_string[_index] & 0x80) == 0x00) {
      _chars++;
    } else if ((_string[_index] & 0x20) == 0x00) {
      _chars++;
      _index ++;
    } else if ((_string[_index] & 0x10) == 0x00) {
      _chars++;
      _index += 2;
    } else if ((_string[_index] & 0x08) == 0x00) {
      _chars++;
      _index += 3;
    }
  }
  _index = 0;
}

utf8::utf8 (const __FlashStringHelper* input)
{
  _string_p = (char*) input;
  _string = NULL;
  _bytes = strlen_P(_string_p);
  _chars = 0;
  for (_index = 0; _index < _bytes; _index++) {
    if ((pgm_read_byte(_string_p + _index) & 0x80) == 0x00) {
      _chars++;
    } else if ((pgm_read_byte(_string_p + _index) & 0x20) == 0x00) {
      _chars++;
      _index ++;
    } else if ((pgm_read_byte(_string_p + _index) & 0x10) == 0x00) {
      _chars++;
      _index += 2;
    } else if ((pgm_read_byte(_string_p + _index) & 0x08) == 0x00) {
      _chars++;
      _index += 3;
    }
  }
  _index = 0;
}

utf8::~utf8 ()
{
  if (_string != NULL)
    free(_string);
}

wchar_t utf8::get()
{
  wchar_t code;
  if (_string_p == NULL) { // string in RAM
    if ((_string[_index] & 0x80) == 0) {
      code = wchar_t(_string[_index]);
      _index++;
    } else if ((_string[_index] & 0x20) == 0) {
      code = wchar_t(_string[_index]   & 0x1f) <<  6 |
             wchar_t(_string[_index+1] & 0x3f);
      _index += 2;
    } else if ((_string[_index] & 0x10) == 0) {
      code = wchar_t(_string[_index]   & 0x0f) << 12 |
             wchar_t(_string[_index+1] & 0x3f) <<  6 |
             wchar_t(_string[_index+2] & 0x3f);
      _index += 3;
#if WCHAR_MAX - WCHAR_MIN >= 16777215
    } else if ((_string[_index] & 0x8) == 0) {
      code = wchar_t(_string[_index]   & 0x07) << 18 |
             wchar_t(_string[_index+1] & 0x3f) << 12 |
             wchar_t(_string[_index+2] & 0x3f) <<  6 |
             wchar_t(_string[_index+3] & 0x3f);
#endif
    } else {
      code = '?';
      _index++;
    }
  } else { // string in flash
    byte first = pgm_read_byte(_string_p + _index);
      if ((first & 0x80) == 0) {
      code = wchar_t(first);
      _index++;
    } else if ((first & 0x20) == 0) {
      code = wchar_t(first                                 & 0x1f) <<  6 |
             wchar_t(pgm_read_byte(_string_p + _index + 1) & 0x3f);
      _index += 2;
    } else if ((first & 0x10) == 0) {
      code = wchar_t(first                                 & 0x0f) << 12 |
             wchar_t(pgm_read_byte(_string_p + _index + 1) & 0x3f) <<  6 |
             wchar_t(pgm_read_byte(_string_p + _index + 2) & 0x3f);
      _index += 3;
#if WCHAR_MAX - WCHAR_MIN >= 16777215
    } else if ((first & 0x8) == 0) {
      code = wchar_t(first                                 & 0x07) << 18 |
             wchar_t(pgm_read_byte(_string_p + _index + 1) & 0x3f) << 12 |
             wchar_t(pgm_read_byte(_string_p + _index + 2) & 0x3f) <<  6 |
             wchar_t(pgm_read_byte(_string_p + _index + 3) & 0x3f);
#endif
    } else {
      code = '?';
      _index++;
    }
  }
  if (_index >= _bytes) _index = 0;
  return code;
}

uint16_t utf8::chars()
{
  return _chars;
}

uint16_t utf8::bytes()
{
  return _bytes;
}
