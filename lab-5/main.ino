#define LED_W_R 4
#define LED_W_Y 5
#define LED_W_G 6
#define WEST_BUTTON_PIN 2
#define LED_S_R 8
#define LED_S_Y 9
#define LED_S_G 10
#define SOUTH_BUTTON_PIN 3
#define goW   0
#define waitW 1
#define goS   2
#define waitS 3
struct State {
unsigned long ST_Out;
unsigned long Time;
unsigned long Next[4];}; // next state for inputs 0,1,2,3
typedef const struct State SType;
SType FSM[4]={ {B00001100,2000,{goW,goW,waitW,waitW}},{B00010100,300,{goS,goS,goS,goS}},{B00100001,2000,{goS,waitS,goS,waitS}},{B00100010,300,{goW,goW,goW,goW}}
};
unsigned long S=0; // index to the current state
void setup() {
  pinMode(LED_W_R, OUTPUT);
  pinMode(LED_W_Y, OUTPUT);
  pinMode(LED_W_G, OUTPUT);
  pinMode(WEST_BUTTON_PIN, INPUT);
pinMode(LED_S_R, OUTPUT);
pinMode(LED_S_Y, OUTPUT);
pinMode(LED_S_G, OUTPUT);
} pinMode(SOUTH_BUTTON_PIN, INPUT);
int input,input1, input2;
void loop() {
digitalWrite(LED_W_R, FSM[S].ST_Out & B00000001); digitalWrite(LED_W_Y, FSM[S].ST_Out & B00000010); digitalWrite(LED_W_G, FSM[S].ST_Out & B00000100);
digitalWrite(LED_S_R, FSM[S].ST_Out & B00001000); digitalWrite(LED_S_Y, FSM[S].ST_Out & B00010000); digitalWrite(LED_S_G, FSM[S].ST_Out & B00100000);
  delay(FSM[S].Time);
  input1 = digitalRead(WEST_BUTTON_PIN);
  input2 = digitalRead(SOUTH_BUTTON_PIN);
  input = input2*2+input1;
} S = FSM[S].Next[input];
