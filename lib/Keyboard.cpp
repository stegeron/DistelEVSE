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

#include <Arduino.h>

#include "Keyboard.h"

Keyboard::Keyboard(ChargeController &chargeController)
{
    this->chargeController = &chargeController;
}

void Keyboard::setup()
{    
    // initialize button pins as inputs
    for(int x=0; x<4; x++)
    {      
      pinMode(_buttons[x], INPUT);
    }
}

void Keyboard::loop()
{
    for(int x=0; x<4; x++)
    {
      // read button
      pressed = digitalRead(_buttons[x]);

      // pokud je stisknuto UP
      if (pressed == HIGH && x == 2) {            
        Serial.print("Tlacitko ");
        Serial.print(x+1);
        Serial.println(" stisknuto");

        // increase charge current
        float currentLimit = this->chargeController->getCurrentLimit();
        this->chargeController->setCurrentLimit(currentLimit+1);

        delay(500);
      } 
      else if (pressed == HIGH && x == 1) {                  
        // decrease charge current
        float currentLimit = this->chargeController->getCurrentLimit();
        this->chargeController->setCurrentLimit(currentLimit-1);

        // volitelná pauza 500 ms
        delay(500);
      }
    }
}