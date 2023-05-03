#include "LoadBalancing.h"
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
 */

float LoadBalancing::calculateCurrentLimit()
{
    // Determine the current load on the mains (highest of the 3 phases)
    float mainsLoad = max(mainsMeter->importCurrent.l1, max(mainsMeter->importCurrent.l2, mainsMeter->importCurrent.l3));

    // Take out the load generated by the charger itself currently
    float houseLoad = mainsLoad - chargeController->actualCurrent;

    // Calculate max current charger may take, giving priority to the house load
    return min(chargeController->maxCurrent, max(0, settings.maxMainsCurrent - houseLoad));
}

void LoadBalancing::balanceLoad()
{
    unsigned long now = millis();

    // If any meter values changed since the last time we checked
    if (mainsMeter->updated > lastChecked || chargeController->actualCurrentUpdated > lastChecked)
    {
        // Only when all required meter values are recent
        if (now - mainsMeter->updated < settings.meterTimeout && now - chargeController->actualCurrentUpdated < settings.meterTimeout)
        {
            float currentLimit = calculateCurrentLimit();

            setCurrentLimit(currentLimit);
        }

        lastChecked = now;
    }
}

void LoadBalancing::fallbackCurrentIfNeeded()
{
    // When fallback is enabled (timeout is configured)
    if (settings.fallbackTimeout > 0)
    {
        // When current limit is outdated (timeout exceeded)
        if (millis() - currentLimitLastUpdated >= settings.fallbackTimeout)
        {
            // Fall back to a safe charging current (for safety reasons)

            // When not already at a safe current
            if (chargeController->getCurrentLimit() > settings.fallbackCurrent)
            {
                Serial.println("Current limit timeout. Falling back to safe charging current");
                chargeController->setCurrentLimit(settings.fallbackCurrent);
            }
        }
    }
}

LoadBalancing::LoadBalancing(ChargeController &chargeController, MainsMeter &mainsMeter)
{
    this->chargeController = &chargeController;
    this->mainsMeter = &mainsMeter;
}

void LoadBalancing::setup(LoadBalancingSettings settings)
{
    this->settings = settings;
    currentLimitLastUpdated = 0;
}

void LoadBalancing::loop()
{
    balanceLoad();
    fallbackCurrentIfNeeded();
}

void LoadBalancing::setCurrentLimit(float amps)
{
    chargeController->setCurrentLimit(amps);
    currentLimitLastUpdated = millis();
}
