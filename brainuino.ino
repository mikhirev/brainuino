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

#include <stdio.h>
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


void ask()
{
// waiting while question is being asked

  digitalWrite(GREENLAMP, LOW);
  digitalWrite(REDLAMP, LOW);

#ifdef RUSSIAN
  printState("Задаётся вопрос ");
#else
  printState("Asking question ");
#endif

  while (true) {

    if (buttonPressed) {
      if (withFalseStart) {
        falseStart(buttonPressed);
      } else {
        answer(buttonPressed);
      }
    }

    // if start button is pressed
    if (digitalRead(CONTROL1) == LOW) {
      if (discuss(timer1) == WRONG) {
        while (discuss(timer2) == WRONG) {}
      }
      return;
    }

    // if stop button is pressed
    if (digitalRead(CONTROL2) == LOW) {
      return;
    }

    delay(100);
  }
}


char discuss(uint16_t timer)
{
// here teams can discuss question or single players think themselves

  char reply = NOREPLY;

  // is random delay needed here when playing with false starts? unsure...

//  time = 0;

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
  printState("Отсчёт:         ");
#else
  printState("Count:          ");
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


char answer(uint8_t num) {

// waiting while answer is given

  digitalWrite(REDLAMP, HIGH);
//  if (timer2 > 0)
//    timer = timer2;
  printPlayer(num);

#ifdef RUSSIAN
  printState("Ответ           ");
#else
  printState("Answer          ");
#endif

  printPreciseTime();
  tone(SPEAKER, 2523, 1000);
  buttonPressed = 0;

  // a pause is needed to release start button, if it was not yet
  delay(2000);

  while (true) {

    // if start button was pressed
    if (digitalRead(CONTROL1) == LOW) {
//      digitalWrite(REDLAMP, LOW);
      buttonPressed = 0;
      return WRONG;
    }

    // if stop button was pressed
    if (digitalRead(CONTROL2) == LOW) {
//        digitalWrite(REDLAMP, LOW);
        buttonPressed = 0;
        return CORRECT;
    }

    delay(100);
  }
}


void falseStart(uint8_t num) {

// if there is false start, show info about it during few time

  tone(SPEAKER, 1784, 1000);
  digitalWrite(REDLAMP, HIGH);
#ifdef RUSSIAN
  printState("   Фальстарт    ");
#else
  printState("  False start   ");
#endif
  printPlayer(num);
  delay(3000);
  digitalWrite(REDLAMP, LOW);
  return;
}


void refresh() {

// displaying current timer state

  time = millis() - startTime;
  printTime();
}


void printState(char *state) {

// displaying string in the beginning of LCD second row

  lcd.setCursor(0, 1);
  uprint(state, &lcd);
}


void printTime() {

// displaying time passed after starting timer

  char timestr[33];
  char integer[10];
  char fractional[2];

  sprintf(integer, "%u", time/1000);
  sprintf(fractional, "%u", time%1000/100);
  sprintf(timestr, "%s.%s", integer, fractional);
  lcd.setCursor(8, 1);
  uprint(timestr, &lcd);
}


void printPreciseTime() {

// displaying time passed after starting timer
// or that it was not started yet

  char timestr[33];
  char integer[10];
  char fractional[4];

  // if timer was started
  if (startTime > 0) {
    time = millis()-startTime;
    sprintf(integer, "%u", time/1000);
    sprintf(fractional, "%03u", time%1000);
    sprintf(timestr, "%s.%s", integer, fractional);
  }
  // if it was not
  else

#ifdef RUSSIAN
    sprintf(timestr, "досрочно");
#else
    sprintf(timestr, "prematur");
#endif

  lcd.setCursor(8, 1);
  uprint(timestr, &lcd);
}


void readButton() {
  
// scan buttons

  if (buttonPressed > 0)
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


void pinInit() {

// Arduino I/O initialisation, needed on startup only

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


void printGameType() {

// displaying game title on LCD

  lcd.setCursor(0, 0);
  switch (gameType) {
    case BRAIN:

#ifdef RUSSIAN
      uprint("   Брейн-ринг   ", &lcd);
#else
      uprint("   Brain-ring   ", &lcd);
#endif

      break;
    case SI:

#ifdef RUSSIAN
      uprint("   Своя игра    ", &lcd);
#else
      uprint("    Jeopardy    ", &lcd);
#endif

      break;
    case CHGK:

#ifdef RUSSIAN
      uprint("Что? Где? Когда?", &lcd);
#else
      uprint("What?Where?When?", &lcd);
#endif

      break;
  }
}


void printPlayer(uint8_t num) {

// displaying number of player or team

  char printstring[65];

  lcd.setCursor(0, 0);
  switch (gameType) {
    case BRAIN:

#ifdef RUSSIAN
      sprintf(printstring, "Команда %u       ", num);
#else
      sprintf(printstring, "Team %u          ", num);
#endif

      break;
    case SI:

#ifdef RUSSIAN
      sprintf(printstring, "Игрок %u         ", num);
#else
      sprintf(printstring, "Player %u        ", num);
#endif

      break;
  }
  uprint(printstring, &lcd);
  buttonPressed = 0;
}
