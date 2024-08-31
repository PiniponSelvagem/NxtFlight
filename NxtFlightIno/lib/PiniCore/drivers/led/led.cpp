#include "led.h"
#include <Arduino.h>

/**
 * @brief	Led state mask.
 */
#define LED_STATE_MASK	0x1

#define LED_GET_STATE(id)           (_ledsCurrentState & (LED_STATE_MASK << id))
#define LED_SET_STATE(id, state)    _ledsCurrentState = ((_ledsCurrentState & ~(LED_STATE_MASK << id)) | ((state & LED_STATE_MASK) << id))

Led::Led(LED_INFO* leds, uint8_t nLeds) {
    for (uint8_t i=0; i<nLeds && i<LED_MAX; ++i) {
        this->_leds[i].pin = leds[i].pin;
        this->_nLeds++;
    }
}

void Led::init() {
    int i = 0;
	while (i < _nLeds) {
        int p = _leds[i].pin;
        pinMode(p, OUTPUT);
        digitalWrite(p, 0);
		++i;
	}
}

bool Led::getState(uint8_t id) {
	return LED_GET_STATE(id);
}

void Led::set(uint8_t id, bool ledState) {
    if (id>=LED_MAX)
        return;
    uint8_t state = ledState ? LED_STATE_MASK : 0x0;
    digitalWrite(_leds[id].pin, state);
    LED_SET_STATE(id, state);
}

void Led::toggle(uint8_t id) {
    if (id>=LED_MAX)
        return;
    uint8_t state = LED_GET_STATE(id) >> id;
    state = state ^ LED_STATE_MASK;
    digitalWrite(_leds[id].pin, state);
    LED_SET_STATE(id, state);
}

