/*
    Brainuino Aleph

    Copyright (C) 2011-2014  Dmitry Mikhirev

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

#include "pinout.h"
#include "game.h"
#include "lcdprint.h"

LiquidCrystal lcd(LCD4, LCD6, LCD11, LCD12, LCD13, LCD14);

void setup()
{
  lcd.begin(16, 2);
  pinInit();
  time = 0;
  
  // TODO: the following settings should be readen from EEPROM
  gameType = SI;
  withFalseStart = false;
  timer1 = 5;
  timer2 = 5;
  preSignal = 0;
}

void loop()
{
  buttonPressed = 0;
  startTime = 0;
//  timer = timer1;
  printGameType();

// ensure that stop button is released
  while (digitalRead(CONTROL2) == LOW) {
    delay(20);
  }
  ask();
}

// ask() - wait until start button is pressed, then run timer
// the question should be asked at this time
void ask()
{
  digitalWrite(GREENLAMP, LOW);
  digitalWrite(REDLAMP, LOW);

#ifdef RUSSIAN
  printState(F("Задаётся вопрос "));
#else
  printState(F("Asking question "));
#endif

  while (true) {

    if (buttonPressed) {
      if (withFalseStart) {
        falseStart(buttonPressed);
      } else {
        answer(buttonPressed);
      }
    }

    if (digitalRead(CONTROL1) == LOW) {  // start button is pressed
      if (discuss(timer1) == WRONG) {
        while (discuss(timer2) == WRONG) {}
      }
      return;
    }

    if (digitalRead(CONTROL2) == LOW) { // stop button is pressed
      return;
    }

    delay(100);
  }
}

// discuss() - start timer and wait for button being pressed
// here teams can discuss question or single players think themselves
answer_t discuss(uint16_t timer)
{
  answer_t reply = NOREPLY;

  // is random delay needed here when playing with false starts? unsure...

  // reset timer or continue count
  if (timer > 0) {
    time = 0;
    startTime = millis();
  } else {
    startTime = millis() - time;
  }

  // we need sound signal here only if we are playing with false starts
  // or am i wrong?
  if (withFalseStart)
    tone(SPEAKER, 3000, 1000);

  digitalWrite(GREENLAMP, HIGH);
  digitalWrite(REDLAMP, LOW);
  printGameType();

#ifdef RUSSIAN
  printState(F("Отсчёт:         "));
#else
  printState(F("Count:          "));
#endif

  while (!buttonPressed && time < (timer - preSignal) * 1000) {
    refresh();
    delay(25);
    if (digitalRead(CONTROL2) == LOW) {
      tone(SPEAKER, 1500, 500);
      return reply;
    }
  }
  if (!buttonPressed)
    tone(SPEAKER, 2121, 1000);
  while (!buttonPressed && time < timer * 1000) {
    refresh();
    delay(25);
    if (digitalRead(CONTROL2) == LOW) {
      tone(SPEAKER, 1784, 500);
      return reply;
    }
  }

  // the time is out or someone pressed a button... hmm...
  if (buttonPressed) {
    reply = answer(buttonPressed);
  } else {
      tone(SPEAKER, 3000, 1000);
  }

  if (reply != WRONG)  {
    digitalWrite(GREENLAMP, LOW);
    digitalWrite(REDLAMP, LOW);
    startTime = 0;
    delay(1000);
  }
  return reply;
}

// answer() - wait while the answer is given
// return answer correctness
answer_t answer(uint8_t num)
{
  digitalWrite(REDLAMP, HIGH);
//  if (timer2 > 0)
//    timer = timer2;
  printPlayer(num);

#ifdef RUSSIAN
  printState(F("Ответ           "));
#else
  printState(F("Answer          "));
#endif

  printPreciseTime();
  tone(SPEAKER, 2523, 1000);
  buttonPressed = 0;

  // a pause is needed to release start button, if it was not yet
  delay(2000);

  while (true) {

    // if start button was pressed
    if (digitalRead(CONTROL1) == LOW) {
      buttonPressed = 0;
      return WRONG;
    }

    // if stop button was pressed
    if (digitalRead(CONTROL2) == LOW) {
      buttonPressed = 0;
      return CORRECT;
    }

    delay(100);
  }
}

// falseStart() - display false start message
void falseStart(uint8_t num)
{
  tone(SPEAKER, 1784, 1000);
  digitalWrite(REDLAMP, HIGH);
#ifdef RUSSIAN
  printState(F("   Фальстарт    "));
#else
  printState(F("  False start   "));
#endif
  printPlayer(num);
  delay(3000);
  digitalWrite(REDLAMP, LOW);
  return;
}

// refresh() - display current timer state
void refresh()
{
  time = millis() - startTime;
  printTime();
}

// printState() - display string in the beginning of LCD second row
void printState(const char *state)
{
  lcd.setCursor(0, 1);
  uprint(state, &lcd);
}

// printState() variant getting strings stored in flash
void printState(const __FlashStringHelper *state)
{
  lcd.setCursor(0, 1);
  uprint(state, &lcd);
}

// printTime() - display time passed since timer was started
void printTime()
{
  char s[4];

  lcd.setCursor(8, 1);
  itoa(time/1000, s, 10);
  uprint(s, &lcd);
  uprint(".", &lcd);
  itoa(time%1000/100, s, 10);
  uprint(s, &lcd);
}

// printPreciseTime() - display time passed since timer was started
// or that it was not started yet
void printPreciseTime()
{
  char s[5];
  uint16_t ms;

  lcd.setCursor(8, 1);

  if (startTime > 0) { // timer was started
    time = millis()-startTime;
    itoa(time/1000, s, 10);
    uprint(s, &lcd);
    uprint(".", &lcd);
    ms = time%1000;
    if (ms < 100) {
      uprint("0", &lcd);
      if (ms < 10) uprint("0", &lcd);
    }
    itoa(ms, s, 10);
    uprint(s, &lcd);
  } else { // timer was not started yet
#ifdef RUSSIAN
    uprint(F("досрочно"), &lcd);
#else
    uprint(F("prematur"), &lcd);
#endif
  }
}

// readButton() - scan buttons
void readButton()
{
  if (buttonPressed > 0 || gameType == CHGK)
    return;
  if (digitalRead(BUTTON1) == LOW) {
    buttonPressed = 1;
    return;
  }
  if (digitalRead(BUTTON2) == LOW) {
    buttonPressed = 2;
    return;
  }
  if (digitalRead(BUTTON3) == LOW) {
    buttonPressed = 3;
    return;
  }
  if (digitalRead(BUTTON4) == LOW) {
    buttonPressed = 4;
    return;
  }
}

// pinInit() - Arduino I/O initialization, needed on startup only
void pinInit()
{
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(BUTTON1, HIGH);
  digitalWrite(BUTTON2, HIGH);
  digitalWrite(BUTTON3, HIGH);
  digitalWrite(BUTTON4, HIGH);
  digitalWrite(CONTROL1, HIGH);
  digitalWrite(CONTROL2, HIGH);
  digitalWrite(ENCPUSH, HIGH);
  pinMode(GREENLAMP, OUTPUT);
  pinMode(REDLAMP, OUTPUT);
  pinMode(SPEAKER, OUTPUT);
  attachInterrupt(0, readButton, FALLING);
}

// printGameType() - display game title on LCD
void printGameType()
{
  lcd.setCursor(0, 0);
  switch (gameType) {
    case BRAIN:

#ifdef RUSSIAN
      uprint(F("   Брейн-ринг   "), &lcd);
#else
      uprint(F("   Brain-ring   "), &lcd);
#endif

      break;
    case SI:

#ifdef RUSSIAN
      uprint(F("   Своя игра    "), &lcd);
#else
      uprint(F("    Jeopardy    "), &lcd);
#endif

      break;
    case CHGK:

#ifdef RUSSIAN
      uprint(F("Что? Где? Когда?"), &lcd);
#else
      uprint(F("What?Where?When?"), &lcd);
#endif

      break;
  }
}

// printPlayer() - display number of player or team
void printPlayer(uint8_t num)
{
  char nums[2];
  char printstring[65];

  itoa(num, nums, 10);
  lcd.setCursor(0, 0);
  switch (gameType) {
    case BRAIN:

#ifdef RUSSIAN
      uprint(F("Команда "), &lcd);
      uprint(nums, &lcd);
      uprint(F("       "), &lcd);
#else
      uprint(F("Team "), &lcd);
      uprint(nums, &lcd);
      uprint(F("          "), &lcd);
#endif

      break;
    case SI:

#ifdef RUSSIAN
      uprint(F("Игрок "), &lcd);
      uprint(nums, &lcd);
      uprint(F("         "), &lcd);
#else
      uprint(F("Player "), &lcd);
      uprint(nums, &lcd);
      uprint(F("        "), &lcd);
#endif

      break;
  }
  buttonPressed = 0;
}
