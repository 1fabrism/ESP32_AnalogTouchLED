/* Analog Touch LED

   The harder you press the touch sensor (predefined on GPIO4), the dimmer
   the LED output (predefined builtin LED on GPIO2).
*/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/touch_pad.h"
#include "driver/ledc.h"
#include "sdkconfig.h"

//Edit the following line and set a number here.
#define BLINK_GPIO 2
#define TOUCH_GPIO 0

void analog_touch_LED_task(void *pvParameter)
{
    //Set a LEDC timer
    ledc_timer_config_t timer_conf;
    timer_conf.duty_resolution = LEDC_TIMER_12_BIT;
    timer_conf.freq_hz = 1000;
    timer_conf.speed_mode = LEDC_HIGH_SPEED_MODE;
    timer_conf.timer_num = LEDC_TIMER_0;
    ledc_timer_config(&timer_conf);
    
    //Set a LEDC channel
    ledc_channel_config_t ledc_conf;
    ledc_conf.channel = LEDC_CHANNEL_0;
    ledc_conf.duty = 0;
    ledc_conf.gpio_num = BLINK_GPIO;
    ledc_conf.intr_type = LEDC_INTR_DISABLE;
    ledc_conf.speed_mode = LEDC_HIGH_SPEED_MODE;
    ledc_conf.timer_sel = LEDC_TIMER_0;
    ledc_channel_config(&ledc_conf);
    
    //Set up touch sensor
    touch_pad_init();
    //touch_pad_io_init(TOUCH_GPIO);
    touch_pad_config(TOUCH_GPIO, 0);
    uint16_t touch_value;
    touch_pad_set_voltage(TOUCH_HVOLT_2V7, TOUCH_LVOLT_0V5, TOUCH_HVOLT_ATTEN_0V);
    
    
    //Where the magic happens:
    while(1) {
        //vTaskDelay(10 / portTICK_PERIOD_MS);
	    touch_pad_read(TOUCH_GPIO, &touch_value);
	    if(touch_value > 1024) touch_value = 1024;
        ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, touch_value);
        ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
    }
}

void app_main()
{
    xTaskCreate(&analog_touch_LED_task, "analog_touch_LED_task", configMINIMAL_STACK_SIZE, NULL, 5, NULL);
}
