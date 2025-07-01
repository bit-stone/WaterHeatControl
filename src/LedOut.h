#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>

void init_led_output();

void set_white_led();
void clear_white_led();
void set_green_led();
void clear_green_led();
void set_yellow_led();
void clear_yellow_led();
void set_red_led();
void clear_red_led();

void set_all_led();
void clear_all_led();

void applyDeltaT(float delta_T);