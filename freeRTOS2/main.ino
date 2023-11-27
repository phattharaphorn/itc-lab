#include <Arduino_FreeRTOS.h>
#include "semphr.h"
#include "queue.h"
#include "task.h"

#define bred 11
#define byellow 10
#define bgreen 12

#define red 6
#define yellow 7
#define green 8

// red and green LED state
int redst = 0;
int greenst = 0;

// For timer                    R  Y  G
unsigned long CurrentTime[3] = {0, 0, 0};
// Debouncing timer           R  Y  G
unsigned long Debounce[3] = {0, 0, 0};

// Queue for button
QueueHandle_t ledQueue;
// Token for led priority
SemaphoreHandle_t token[3];

void setup()
{
  Serial.begin(9600);
  ledQueue = xQueueCreate(10, sizeof(int32_t));
  xTaskCreate(vSenderTask, "Red SW", 100, bred, 1, NULL);
  xTaskCreate(vSenderTask, "Yellow SW", 100, byellow, 1, NULL);
  xTaskCreate(vSenderTask, "Green SW", 100, bgreen, 1, NULL);

  xTaskCreate(redLED, "Red LED", 100, NULL, 1, NULL);
  xTaskCreate(yellowLED, "Yellow LED", 100, NULL, 1, NULL);
  xTaskCreate(greenLED, "Green LED", 100, NULL, 1, NULL);

  // Givetoken for each LED
  for (int i = 0; i < 3; i++)
  {
    token[i] = xSemaphoreCreateBinary();
    xSemaphoreGive(token[i]);
  }
}

// Task for button
void vSenderTask(void *pvParameters)
{
  BaseType_t qStatus;
  int32_t valueToSend = 0;
  int32_t bt = (int32_t)pvParameters;
  pinMode(bt, INPUT_PULLUP);
  while (1)
  {
    // Check input_pullup
    if (!digitalRead(bt))
    {
      // Send button pin
      valueToSend = bt;
      // add to queue
      qStatus = xQueueSend(ledQueue, &valueToSend, 0);
      vTaskDelay(1);
    }
  }
}

void redLED(void *pvParameters)
{
  int32_t valueReceived;
  pinMode(red, OUTPUT);
  BaseType_t qStatus;
  const TickType_t xTicksToWait = pdMS_TO_TICKS(100);

  while (1)
  {
    qStatus = xQueueReceive(ledQueue, &valueReceived, xTicksToWait);
    if (qStatus == pdPASS)
    {

      // Check red button
      if (valueReceived == bred && millis() - Debounce[0] >= 500)
      {
        Debounce[0] = millis();
        // Check for red token
        if (xSemaphoreTake(token[0], 1) == pdTRUE)
        {
          CurrentTime[0] = millis();
          redst = 1;
        }
        else
        {
          // If push red button for 2nd time, it'll turn off
          redst = 0;
        }
      }
    }
    redLEDControl(redst); // For controlling red LED
  }
}

void yellowLED(void *pvParameters)
{
  int32_t valueReceived;
  pinMode(yellow, OUTPUT);
  BaseType_t qStatus;
  const TickType_t xTicksToWait = pdMS_TO_TICKS(100);

  while (1)
  {
    qStatus = xQueueReceive(ledQueue, &valueReceived, xTicksToWait);
    if (qStatus == pdPASS)
    {
      // Check for yellow button
      if (valueReceived == byellow && millis() - Debounce[1] >= 500)
      {
        Debounce[1] = millis();
        // check for red token
        if (xSemaphoreTake(token[0], 1) == pdTRUE)
        {
          // Unlock red token
          xSemaphoreGive(token[0]);
          // Check for green token
          if (xSemaphoreTake(token[2], 1) == pdTRUE)
          {
            // Unlock greeen token
            xSemaphoreGive(token[2]);
            // Check for yellow token
            if (xSemaphoreTake(token[1], 1) == pdTRUE)
            {
              // Turn yellow LED on
              yellowLEDControl(1);
            }
          }
        }
      }
    }
  }
}

void greenLED(void *pvParameters)
{
  int32_t valueReceived;
  pinMode(green, OUTPUT);
  BaseType_t qStatus;
  const TickType_t xTicksToWait = pdMS_TO_TICKS(100);

  while (1)
  {
    qStatus = xQueueReceive(ledQueue, &valueReceived, xTicksToWait);
    if (qStatus == pdPASS)
    {
      // Check for green button
      if (valueReceived == bgreen && millis() - Debounce[2] >= 500)
      {
        Debounce[2] = millis();
        // Check red token
        if (xSemaphoreTake(token[0], 1) == pdTRUE)
        {
          // Unlock red token
          xSemaphoreGive(token[0]);
          // Check for green token ifself
          if (xSemaphoreTake(token[2], 1) == pdTRUE)
          {
            CurrentTime[2] = millis();
            greenst = 1; // Turn on
          }
          else
          {
            greenst = 0; // Turn off if push green button when green LED is on
          }
        }
      }
    }
    greenLEDControl(greenst); // controlling green LED
  }
}
void loop() {}

void redLEDControl(int state)
{
  // if red state is on
  if (state)
  {
    digitalWrite(red, HIGH);
    // check for timer 3sec.
    if (millis() - CurrentTime[0] >= 3000)
    {
      digitalWrite(red, LOW);
      redst = 0;
      // unlock red token
      xSemaphoreGive(token[0]);
    }
  }
  else
  { // for push button 2nd time
    digitalWrite(red, LOW);
    xSemaphoreGive(token[0]); // unlock
  }
}

void yellowLEDControl(int state)
{
  // if yellow lED on
  if (state)
  {
    // counter for yellow blink
    int cnt_Yellow = 0;
    while (cnt_Yellow < 4)
    {
      // check for each time of blink
      if (millis() - CurrentTime[1] >= 500)
      {
        // switching between on and off
        digitalWrite(yellow, digitalRead(yellow) ^ 1);
        cnt_Yellow++;
        CurrentTime[1] = millis();
      }
    }
    // unlock yellow token
    xSemaphoreGive(token[1]);
  }
}

void greenLEDControl(int state)
{
  // If green led state on
  if (state)
  {
    digitalWrite(green, HIGH);
    // check for timer 3 sec.
    if (millis() - CurrentTime[2] >= 3000)
    {
      digitalWrite(green, LOW);
      greenst = 0;
      // unlock token
      xSemaphoreGive(token[2]);
    }
  }
  else
  { // for push button 2nd time
    digitalWrite(green, LOW);
    // unlock token
    xSemaphoreGive(token[2]);
  }
}