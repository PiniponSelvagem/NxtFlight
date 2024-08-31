#include "rotaryencoder.h"
#include <Arduino.h>

// The array holds the values -1 for the entries where a position was decremented,
// a 1 for the entries where the position was incremented
// and 0 in all the other (no change or not valid) cases.
const int8_t KNOBDIR[] = {
    0, -1, 1, 0,
    1, 0, 0, -1,
    -1, 0, 0, 1,
    0, 1, -1, 0};

// Configuration for different types of rotary encoders.
// For more information, refer to http://svglobe.com/arduino/in_encoder.html
//
// The detents in the encoder type settings below are indexed
// by value, not by the order in the quadrature cycle. For example, a rotary
// encoder with detents at 00 and 11 (positions 0 and 2 in the
// quadrature) but are indexed based on their decimal values (0 and 3).
// This allows simple lookup of detent positions by value.
const encoderType encoderTypes[] {
    /**
     * DO NOT CHANGE THE ORDER OF THESE WITHOUT
     * REORDERING THE RotaryEncoderType!!! 
     */

    // 1 detents per cycle:  00 ,  10 , [11],  01
    {{false, false, false, true}, 2},

    // 1 detents per cycle: [00],  10 ,  11 ,  01
    {{true, false, false, false}, 2},

    // 2 detents per cycle: [00],  10 , [11],  01
    {{true, false, false, true}, 1},

    // 2 detents per cycle:  00 , [10],  11,  [01]
    {{false, true, true, false}, 1},

    // 4 detents per cycle: [00], [10], [11], [01]
    {{true, true, true, true}, 0},
};


RotaryEncoder::RotaryEncoder(uint8_t pin0, uint8_t pin1, RotaryEncoderType typeEncoder) {
    this->_pos = 0;
    this->_pin0 = pin0;
    this->_pin1 = pin1;
    this->_encoderType = encoderTypes[typeEncoder];

    // start with position 0;
    _oldState         = 0;
    _position         = 0;
    _positionExt      = 0;
    _detentCounter    = 0;
    _positionTimePrev = 0;   // for first startup avoid calculation of a high speed, _positionTimePrev
    _positionTime     = 500; // and _positionTime must be initialized to avoid that they have the same value
}

void RotaryEncoder::init() {
    pinMode(_pin0, INPUT_PULLUP);
    pinMode(_pin1, INPUT_PULLUP);
}

void RotaryEncoder::pullEvents() {
    tick();

    int16_t pos = getPosition();
    if (pos == _pos) {
        // nothing happened
        return;
    }

    bool dir = true;
    int16_t delta = pos - _pos;
    _delta += delta;

    if (delta < 0)
        dir = false;

    // protect from overflow
    if ((dir && (pos + delta * 2) > ROTENC_MAX) || (!dir && (pos - delta * 2) < -ROTENC_MAX)) {
        setPosition(0);
        pos = 0;
    }
    _pos = pos;
}

void RotaryEncoder::resetPosition() {
    setPosition(0);
}

int16_t RotaryEncoder::getPosition() {
    return _positionExt;
}

int32_t RotaryEncoder::getDelta() {
    uint32_t retDelta = _delta;
    _delta = 0;
    return retDelta;
}



void RotaryEncoder::tick() {
    bool     sig1      = !digitalRead(_pin0); // to keep backwards compatibility for encoder type digitalRead must be negated
    bool     sig2      = !digitalRead(_pin1); // to keep backwards compatibility for encoder type digitalRead must be negated
    int      speed     = 0;
    uint32_t currentMs = millis();

    int8_t thisState = sig1 | (sig2 << 1);

    if (currentMs - _lastFastDec > 100 && _detentCounter > 1) {
        _lastFastDec = currentMs;
        _detentCounter--;
    } else if (currentMs - _positionTimePrev > 500) { // if more than 500ms no step detected, set fast acceleration to 0
        _lastFastDec   = currentMs;
        _detentCounter = 0;
    }

    if (_oldState != thisState) {

        if (_detentCounter > ROTENC_FAST_TRIGGER) { // at minimum X detents have to be detected before fast step can be detected
            speed = ROTENC_FAST_SPEED;
        } else {
            speed = 1;
        }

        _position += ((KNOBDIR[thisState | (_oldState << 2)] * speed)) << _encoderType.resolutionShift;
        if (_encoderType.detents[thisState]) {
            _positionTimePrev = _positionTime;
            _positionTime     = currentMs;
            _positionExt      = _position >> 2; //_encoderType.resolutionShift;
            _detentCounter    = min(_detentCounter + 1, 12);
        }
        _oldState = thisState;
    }
}

void RotaryEncoder::setPosition(int16_t newPosition) {
    // only adjust the external part of the position.
    _position    = ((newPosition >> _encoderType.resolutionShift) | (_position & 0x03));
    _positionExt = newPosition;
}