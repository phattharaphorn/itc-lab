#include <Arduino_FreeRTOS.h>
#include "queue.h"

#define RED     9
#define YELLOW  8
#define GREEN   7
#define SW1     12
#define SW2     11
#define SW3     10

QueueHandle_t BlinkQueue;
int GREEN_status;
void setup()
{
  Serial.begin(9600);
  BlinkQueue =  xQueueCreate(10, sizeof(int32_t));
  xTaskCreate(vSenderTask, "Sender Task 1", 100, SW1, 1, NULL);
  xTaskCreate(vSenderTask, "Sender Task 2", 100, SW2, 1, NULL);
  xTaskCreate(vSenderTask, "Sender Task 3", 100, SW3, 1, NULL);
  xTaskCreate(vReceiverTask, "Receiver Task R", 100, RED, 1, NULL);
  xTaskCreate(vReceiverTask, "Receiver Task Y", 100, YELLOW, 1, NULL);
  xTaskCreate(vReceiverTask, "Receiver Task G", 100, GREEN, 1, NULL);
}

void vSenderTask(void *pvParameters)
{
  int32_t valueToSend = 0;
  int SW = (int32_t)pvParameters;
  pinMode(SW, INPUT);
  const TickType_t xTicksToWait = pdMS_TO_TICKS(100);
  while (1)
  {
    if (digitalRead(SW) == HIGH)
      valueToSend = SW;
    else
      valueToSend = 0;

    if (valueToSend)
    {
      xQueueSend(BlinkQueue, &valueToSend, xTicksToWait);
      vTaskDelay(20);
    }
  }
}

void vReceiverTask(void *pvParameters)
{
  int32_t valueReceived;
  BaseType_t qStatus;
  int LED = (int32_t)pvParameters;
  const TickType_t xTicksToWait = pdMS_TO_TICKS(100);
  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(GREEN, OUTPUT);
  int YELLOW_status = 0;
  long RED_TIME=0;
  long GREEN_TIME=0;
  while (1)
  {
    qStatus = xQueueReceive(BlinkQueue, &valueReceived, xTicksToWait);
    if (qStatus  == pdPASS)
    {
      if (valueReceived == SW1 && LED == RED)
      {
        digitalWrite(RED, HIGH);
        RED_TIME=millis();
        while(millis()-RED_TIME<3000);
        digitalWrite(RED, LOW);
      }
      else if (valueReceived == SW2 && LED == YELLOW)
      {
        YELLOW_status = !YELLOW_status;
      }
      else if (valueReceived == SW3 && LED == GREEN)
      {
        GREEN_status=1;
        digitalWrite(GREEN, HIGH);
        GREEN_TIME=millis();
        while(millis()-GREEN_TIME<500);
        digitalWrite(GREEN, LOW);
        GREEN_TIME=millis();
        while(millis()-GREEN_TIME<500);
        digitalWrite(GREEN, HIGH);
        GREEN_TIME=millis();
        while(millis()-GREEN_TIME<500);
        digitalWrite(GREEN, LOW);
        GREEN_TIME=millis();
        while(millis()-GREEN_TIME<500);
        digitalWrite(GREEN, HIGH);
        GREEN_TIME=millis();
        while(millis()-GREEN_TIME<500);
        digitalWrite(GREEN, LOW);
        GREEN_TIME=millis();
        while(millis()-GREEN_TIME<500);
        GREEN_status=0;
      }
      else if (valueReceived == SW2 ||valueReceived == SW1)
      {
        int32_t valueToSend = valueReceived;
        xQueueSend(BlinkQueue, &valueToSend, xTicksToWait);
        vTaskDelay(1);
      }
      else if (GREEN_status==0){
        int32_t valueToSend = valueReceived;
        xQueueSend(BlinkQueue, &valueToSend, xTicksToWait);
        
        vTaskDelay(1);
        GREEN_status=1;
      }
    }
    if (YELLOW_status && LED == YELLOW)
    {
      digitalWrite(YELLOW, HIGH);
      vTaskDelay(10);
      digitalWrite(YELLOW, LOW);
      vTaskDelay(10);
    }
  }
}
void loop() {}
