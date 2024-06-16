/**
* @file		led.h
* @brief	Contains the led API.
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

#ifndef _PINICORE_LED_H_
#define _PINICORE_LED_H_

#include "piniconst.h"

class Led {
public:
    /**
     * @brief	LED constructor.
     * @param	state: Initial led state, default is turned OFF.
     * @param	pin: Led pin, default is the built in led pin.
     */
    Led(bool state=OFF, PIN pin=LED_BUILTIN);

    /**
     * @brief	Initializes the configured led.
     * @note	This function must be called prior to any other LED functions.
     */
    void init();

    /**
     * @brief	Checks for button state changes and their active state, and returns it as a mask.
     * @return	'true' when led is ON and 'false' when led is OFF.
     * @warning Since this function uses an internal state, if the led state is changed without using this API, it might return incorrect results.
     */
    bool getState();

    /**
     * @brief	Turns the on board led ON.
     */
    void on();

    /**
     * @brief	Turns the on board led OFF.
     */
    void off();

    /**
     * @brief	Toggles led state ON/OFF based on last state.
     * @warning Since this function uses an internal state, if the led state is changed without using this API, it might toggle to an unexpected state.
     */
    void toggle();

private:
    PIN pin;
    bool currentState;
};


#endif /* _PINICORE_LED_H_ */
