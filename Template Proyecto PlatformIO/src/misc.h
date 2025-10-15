#ifndef MISC_H
#define MISC_H

#include <Arduino.h>

//=====================================
//PINS
#define STNDBY_PIN 23 //Enable motors

//Left motor
#define PWM_A 32
#define A1_PIN 17
#define A2_PIN 5

//Right motor
#define PWM_B 13
#define B1_PIN 19
#define B2_PIN 18

// Weapon
#define WEAPON_PIN 25
#define ATTACK_POWER 80

#define MIN_POWER 3277
#define MAX_POWER 6553
#define WEAPON_CHANNEL 0
#define WEAPON_FREC 50
#define WEAPON_RES 16

//=====================================
//MACROS / PARAMETERS / AUX FUNCTIONS
#define rep(i, n) for(int i=0; i<n; i++)
#define MAX_ARDUINO_TIME 3294967295

unsigned long get_time(){
    return (millis()%MAX_ARDUINO_TIME);
}


//=====================================




#endif