#include <Arduino.h>
#include <NewPing.h>
#include <string>

#include "BluetoothSerial.h"

#define ECHO 23
#define TRIG 22
#define MAX_DISTANCE 150

#define SAFE_DISTANCE 20

BluetoothSerial BTSerial;

/* Initialize SR04 */
NewPing sonar(TRIG, ECHO, MAX_DISTANCE);
uint8_t distance;
uint8_t message[] = {48, 10};

TaskHandle_t task1;
TaskHandle_t task2;

// Task handlers
void Task1code( void * pvParameters);
void Task2code( void * pvParameters);

void setup() {

  Serial.begin(9600);
  BTSerial.begin("ESP32");
  
  // task configuration
  xTaskCreate(
    Task1code,   /* Task function. */
    "Task1",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &task1); /* Task handle to keep track of created task */

  // second task configuration
  xTaskCreate(
    Task2code,   /* Task function. */
    "Task2",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &task2); /* Task handle to keep track of created task */

  Serial.println("End of setup..");

}

void loop() {
  // put your main code here, to run repeatedly:
}

void Task1code( void * pvParameters ){
  const TickType_t xDelay = 1000 / portTICK_PERIOD_MS;

  for(;;){
    Serial.println(distance);
    vTaskDelay( xDelay );
  } 
}

void Task2code( void * pvParameters){
  const TickType_t xDelay = 25 / portTICK_PERIOD_MS;
  const uint32_t longDelay = 40 * xDelay;
  for(;;){
    distance = sonar.ping_cm();
    if (distance < SAFE_DISTANCE && distance != 0){
      BTSerial.write(message, 2);
      vTaskDelay( longDelay );
    }
    vTaskDelay( xDelay );
  }
}