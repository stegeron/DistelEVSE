/*
 * This file is part of the ArduinoEVSE (https://github.com/tomwetjens/ArduinoEVSE).
 * Copyright (c) 2023 Tom Wetjens.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * @ Author stegeron
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_ _

#include <Arduino.h>
#include "ChargeController.h"

#define PIN_BUTTON_ESC 7
#define PIN_BUTTON_DOWN 6
#define PIN_BUTTON_UP 5
#define PIN_BUTTON_ENTER 4

const int _buttons[] = {PIN_BUTTON_ESC, PIN_BUTTON_DOWN, PIN_BUTTON_UP, PIN_BUTTON_ENTER};

class Keyboard
{
private:
  ChargeController *chargeController;
  unsigned long _touchdelay;
  unsigned long _lastTouchedMillis;
  int pressed;
  

public:
  Keyboard(ChargeController &chargeController);
  void setup();
  void loop();
};

#endif // RELAY_H_