#define led1 2 //แดง
#define led2 3 //เหลือง
#define led3 4 //เขียว
#define button1 7 //ซ้าย
#define button2 8 //กลาง
#define button3 9 //ขวา

long timerLed1 = 0;
long timerLed2 = 0;
long timerLed3 = 0;

long timerSwitch1 = 0;
long timerSwitch2 = 0; 
long timerSwitch3 = 0;

int greenOn = 0;
int yellowOn = 0;
int redOn = 0;

int currentRed;
int currentGreen;
int currentYellow;

int yellowDelay = 2000;
int greenDelay = 3000; 
int redDelay = 3000;
 
void setup(){
    pinMode(led1,OUTPUT);
    pinMode(led2,OUTPUT);
    pinMode(led3,OUTPUT);
    pinMode(button1,INPUT);
    pinMode(button2,INPUT);
    pinMode(button3,INPUT_PULLUP);
    
} 
void loop(){
  if(digitalRead(button3) == LOW && redOn == 0){
      digitalWrite(led3, HIGH);
    greenOn = 1;
  }
  if(greenOn == 1){
      currentGreen = millis();
  }
  if(greenOn == 1 && digitalRead(button3) == LOW){
      digitalWrite(led3,LOW);
    greenOn = 0;
    timerLed3 = currentGreen;
  }
  if(currentGreen - timerLed3 >= greenDelay){
      digitalWrite(led3,LOW);
    greenOn = 0;
    timerLed3 = currentGreen;
  }

  
  if(digitalRead(button2) == LOW && redOn == 0 && greenOn == 0){
      digitalWrite(led2, HIGH);
    yellowOn = 1;
    delay(500);
  }
  if(yellowOn == 1){
    currentYellow = millis();
    digitalWrite(led2,LOW);
    delay(500);
    digitalWrite(led2,HIGH);
    delay(500);
    digitalWrite(led2,LOW);
    delay(500);
    yellowOn = 0;
  }
  if(currentYellow - timerLed2 >= yellowDelay){
    timerLed2 = currentYellow;
  }
  

  
  if (digitalRead(button1) == HIGH)
  {
    digitalWrite(led1, HIGH);
    redOn = 1;
  }
  if (redOn == 1)
  {
    currentRed = millis();
  }
  if(redOn == 1 && digitalRead(button1) == HIGH){
      digitalWrite(led1,LOW);
    redOn = 0;
    timerLed1 = currentRed;
  }
  if (currentRed - timerLed1 >= redDelay)
  {
    digitalWrite(led1, LOW);
    redOn = 0;
    timerLed1 = currentRed;
  }
}
