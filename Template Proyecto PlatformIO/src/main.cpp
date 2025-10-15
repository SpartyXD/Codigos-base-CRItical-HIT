/*
MIT License
Copyright (c) [2025] [Matias Oteiza]
Github: SpartyXD
*/

#include <Arduino.h>
#include <misc.h>
#include <motors.h>
#include <BluetoothSerial.h>


//Globals
TB6612_MotorShield motors;
BrushlessMotor weapon;
BluetoothSerial server;

//VARIABLES
#define NOMBRE_BLUETOOTH "SparkBot"
int LEFT_SPEED = 250;
int RIGHT_SPEED = 250;

//Bluetooth stop
#define CONECTION_DELAY 5000
unsigned long time_now=0, last_check=0;
bool running = false;


//==========================================
//Funciones
void handleBluetoothStop();
void handleCommand(String command);


//==========================================
//PROGRAMA PRINCIPAL

void setup(){
    Serial.begin(115200);
    server.begin(NOMBRE_BLUETOOTH);
    delay(500);

    //Init all motors
    motors.init(STNDBY_PIN, PWM_A, A1_PIN, A2_PIN, PWM_B, B1_PIN, B2_PIN);
    weapon.init(WEAPON_PIN, WEAPON_CHANNEL);
    Serial.println("Systems ready!\n");
}


void loop(){
  time_now = get_time();

  while(server.available()){
    last_check = time_now;
    handleCommand(server.readStringUntil('\n'));
  }

  //Checkear si bluetooth perdio señal
  handleBluetoothStop();
}

//==========================================
//Implementaciones

void handleCommand(String command){
  char letter = command[0];

  //STOP
  if(letter == 'S'){
      motors.stopMotors();
      running = false;
      Serial.println("STOP!\n");
      return;
  }

  running = true;
  switch (letter)
  {
  case 'F':
      motors.controlMotors(LEFT_SPEED, RIGHT_SPEED);
      Serial.println("Forward!\n");
      break;
    case 'B':
      motors.controlMotors(-LEFT_SPEED, -RIGHT_SPEED);
      Serial.println("Backwards!\n");
      break;
    case 'L':
      motors.controlMotors(-LEFT_SPEED, RIGHT_SPEED);
      Serial.println("Turning Left!\n");
      break;
    case 'R':
      motors.controlMotors(LEFT_SPEED, -RIGHT_SPEED);
      Serial.println("Turning right!\n");
      break;
    case 'W':
        Serial.println("Activating weapon!\n");
        weapon.spin(ATTACK_POWER);
        break;
    case 'w':
        Serial.println("WEAPON OFF!\n");
        weapon.spin(0);
        break;
  //------------------------
  default:
      break;
  }
}


//Detener todo en caso que radiocontrol se desconecte
void handleBluetoothStop(){
  if(running && (time_now-last_check) > CONECTION_DELAY){
    Serial.println("Disconnect error stop!");
    motors.stopMotors();
    running = false;
    weapon.spin(0);
  }
}