/**
* @file		button.c
* @brief	Button API.
* @version	1.1
* @date		19 Nov 2020
* @author	PiniponSelvagem
*
* Copyright(C) 2020-2021, PiniponSelvagem
* All rights reserved.
*
***********************************************************************
* Software that is described here, is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
**********************************************************************/

#include "button.h"
#include <Arduino.h>

/**
 * @brief	Button mask used to get 1 bit of each button. Depending if shift is used or not, it can be used to get only the pressed states or the transition states for all buttons.
 * 			Binary representation visualization:
 * 			> 0101 0101 0101 0101 0101 0101 0101 0101
 */
#define BTNS_MASK_ACTIVE	0x55555555

/**
 * @brief	Button mask size.
 *          Transition state (left bit) and pressed state (right bit).
 */
#define BTN_MASK_SIZE		2

/**
 * @brief	Button status masks.
 * @warning	DO NOT change these defines, since pullButtonsEvents and updateButtonEvent algorithm are dependent of these values.
 */
#define BTN_PRESSED_MASK	0x1
#define BTN_CHANGED_MASK	0x2


/**
 * @brief	Checks for button state changes and their active state, and returns it as a mask.
 * @param   id: Button identifier, equal to the possition in the \ref 'btns' array.
 * @param	btnInfo: Struct containing button info, unique ID and other information.
 * @return	Mask containing only the button state, shifted to its correct placement based on its unique ID.
 *			Does NOT contain transition state detection.
 */
static uint32_t updateButtonEvent(uint8_t id, struct BTN_INFO btnInfo) {
	int pinsState = 0;
    int p = btnInfo.pin;
    int b = digitalRead(p);
    pinsState = ~(b) << id * BTN_MASK_SIZE;
    pinsState &= (0x1 << (BTN_MASK_SIZE * id));	// 0x1 -> to make & with the current state.
	return pinsState;
}





Button::Button(BTN_INFO* btns, uint8_t size) {
    for (uint8_t i=0; i<size && i<BUTTON_MAX; ++i) {
        this->btns[i].pin = btns[i].pin;
        this->nButtons++; 
    }
}

void Button::init() {
	int i = 0;
	while (i < nButtons) {
        int p = btns[i].pin;
        pinMode(p, INPUT_PULLUP);
		++i;
	}
}

void Button::pullButtonsEvents() {
	int btnsActive = 0;
    for (uint8_t i=0; i<nButtons; ++i) {
		btnsActive |= updateButtonEvent(i, btns[i]);
	}

	int state = btnsActive | ((btnsActive ^ (BTNS_MASK_ACTIVE & btnsLastState)) << 1);	// 1 -> shift left to place transitions states
	btnsLastState = state;
}

uint32_t Button::getSnapshot() {
	return btnsLastState;
}

#define BTN_IS_PRESSED		(btnsLastState & (BTN_PRESSED_MASK << (id * BTN_MASK_SIZE)))
#define BTN_IS_CHANGED		(btnsLastState & (BTN_CHANGED_MASK << (id * BTN_MASK_SIZE)))
bool Button::isPressed(uint8_t id)			{ return BTN_IS_PRESSED;					 }
bool Button::isChanged(uint8_t id)			{ return BTN_IS_CHANGED;					 }
bool Button::isPressedRepeating(uint8_t id)	{ return BTN_IS_PRESSED && !BTN_IS_CHANGED;  }
bool Button::isSinglePressed(uint8_t id)	{ return BTN_IS_PRESSED && BTN_IS_CHANGED;	 }
bool Button::isSingleReleased(uint8_t id)	{ return !BTN_IS_PRESSED && BTN_IS_CHANGED;	 }
bool Button::isAnyKeyPressed()				{ return (btnsLastState & BTNS_MASK_ACTIVE); }
