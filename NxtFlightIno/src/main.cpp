#include <Arduino.h>

#include "pinicore.h"

//#include "avr8-stub.h"
//#include "app_api.h"

LED_INFO leds[] = { {LED_BUILTIN}, {22}, {23} };
Led led = Led(leds, sizeof(leds));

BTN_INFO btns[] = { {24}, {26}, {28} };
Button button = Button(btns, sizeof(btns));

RotaryEncoder rotary = RotaryEncoder(40, 42, RotaryEncoderType::T_00_11);

void setup() {
    //debug_init();
    Serial.begin(115200);
    led.init();
    button.init();
    rotary.init();
}

int count = 0;
unsigned long previousMillis = 0;
const long interval = 10;

int currentPosition = 0;
int currentDelta = 0;

void loop() {
    button.pullEvents();
    rotary.pullEvents();

    if (count >= 10) {
        currentPosition = rotary.getPosition();
        currentDelta += rotary.getDelta();
        Serial.print("currPos ");
        Serial.print(currentPosition);
        Serial.print(" | currDelta ");
        Serial.println(currentDelta);

        count = 0;
    }
    ++count;

    if (button.isSinglePressed(0)) {
        led.toggle(0);
    }
    if (button.isSinglePressed(1)) {
        led.toggle(1);
    }
    if (button.isSinglePressed(2)) {
        led.toggle(2);
    }

    unsigned long currentMillis = millis();
    unsigned long elapsedTime = millis() - currentMillis;
    if (elapsedTime < interval) {
        delay(interval - elapsedTime);
    }
    previousMillis = currentMillis;
}