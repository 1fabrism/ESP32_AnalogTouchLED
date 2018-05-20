# Analog Touch LED

Starts a FreeRTOS task that analog reads one of the on-board touch sensors and uses PWM via the LEDC library to control the brightness of the builtin LED. The harder the button is pressed, the dimmer the light.

Made by reusing code from the touch_pad and ledc examples from the ESP_IDF.
