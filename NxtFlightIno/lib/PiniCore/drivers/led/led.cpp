/**
* @file		led.c
* @brief	LED configuration API.
* @version	1.1
* @date		12 Jun 2024
* @author	PiniponSelvagem
*
* Copyright(C) 2020-2024, PiniponSelvagem
* All rights reserved.
*
***********************************************************************
* Software that is described here, is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
**********************************************************************/


#include "led.h"
#include <Arduino.h>

Led::Led(bool state, PIN pin) {
    this->currentState = state;
    this->pin = pin;
}

void Led::init() {
    pinMode(pin, OUTPUT);

	if (currentState)
		on();
	else
		off();
}

bool Led::getState() {
	return currentState;
}

void Led::on() {
    digitalWrite(pin, HIGH);
	currentState = true;
}

void Led::off() {
	digitalWrite(pin, LOW);
	currentState = false;
}

void Led::toggle() {
	if (currentState)
		off();
	else
		on();
}

