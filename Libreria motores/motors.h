/*
MIT License
Copyright (c) [2025] [Matias Oteiza]
Github: SpartyXD
*/

#ifndef MOTORS_H
#define MOTORS_H

#include <Arduino.h>

struct TB6612_MotorShield{
    int enable_pin;
    int left_pwm_pin, left_a_pin, left_b_pin;
    int right_pwm_pin, right_a_pin, right_b_pin;
    int MAX_SPEED = 255;

    TB6612_MotorShield(){}

    /*
    Inicializa motores del driver TB6612
    */
    void init(int en, int pwm_A, int a_1, int a_2, int pwm_B, int b_1, int b_2, int max_speed=255){
        enable_pin = en;
        left_pwm_pin = pwm_A;
        left_a_pin = a_1;
        left_b_pin = a_2;
        
        right_pwm_pin = pwm_B;
        right_a_pin = b_1;
        right_b_pin = b_2;
        
        MAX_SPEED = max_speed;
        
        pinMode(enable_pin, OUTPUT);
        pinMode(left_pwm_pin, OUTPUT); pinMode(left_a_pin, OUTPUT); pinMode(left_b_pin, OUTPUT);
        pinMode(right_pwm_pin, OUTPUT); pinMode(right_a_pin, OUTPUT); pinMode(right_b_pin, OUTPUT);
        stopMotors();
    }

    /*
    Frena ambos motores
    */
    void stopMotors(){
        controlMotors(0, 0);
        digitalWrite(enable_pin, LOW);
    }


    /*
    Modifica la velocidad de 1 motor
    @param motor: index del motor (0-izquierda | 1-derecha)
    */
    void setMotorSpeed(int motor, int speed){
        bool reverse = speed<0;
        speed = constrain(abs(speed), 0, MAX_SPEED);

        if(motor == 0){
            //Left
            analogWrite(left_pwm_pin, speed);
            digitalWrite(left_a_pin, !reverse);
            digitalWrite(left_b_pin, reverse);
        }
        else{
            //Right
            analogWrite(right_pwm_pin, speed);
            digitalWrite(right_a_pin, !reverse);
            digitalWrite(right_b_pin, reverse);
        }
    }


    /*
    Modificar velocidades de motores
    Si el numero es negativo sera reversa (tope 255)
    */
    void controlMotors(int left_speed, int right_speed){
        digitalWrite(enable_pin, HIGH);
        setMotorSpeed(0, left_speed);
        setMotorSpeed(1, right_speed);
    }

};


struct BrushlessMotor{
    int pin=0;
    int max_power=0;
    int min_power=0;
    int channel=0;
    int frec=0;
    int res=0;

    BrushlessMotor(){}

    /*
    Inicializa brushless

    @param pin: pin donde esta conectado
    @param channel: canal pwm de la ESP a usar (depende del pin)
    @param demas_parametros: opcionales por si tu driver tiene configuracion de pulsos diferente
    */
    void init(int pin, int channel, int min_power=3277, int max_power=6553, int frecuency=50, int resolution=16){
        this->pin = pin;
        this->channel = channel;
        this->min_power = min_power;
        this->max_power = max_power;
        this->frec = frecuency;
        this->res = resolution;

        //Inicializar driver (safe mode)
        ledcSetup(channel, frec, res);
        ledcAttachPin(pin, channel);

        ledcWrite(channel, min_power);
        delay(2000);
    }

    /*
    Hace girar al brushless

    @param power: porcentaje (0-100) de potencia para girar motor
    */
    void spin(int power){
        power = constrain(power, 0, 100);
        int duty_cycle = map(power, 0, 100, min_power, max_power);
        ledcWrite(pin, duty_cycle);
        Serial.println("Brushless Power: " + String(duty_cycle) + "%");
    }
};


#endif