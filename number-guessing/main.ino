้ั#define button1 12
#define button2 13
int number;
int randNumber = 0;
const byte numPins = 8;
int select = 0;

int bitPattern[9] = {
  //7654321
  //GFEDCBA
  B1111001,//1
  B0100100,//2
  B0110000,//3
  B0011001,//4
  B0010010,//5
  B0000010,//6
  B1111000,//7
  B0000000,//8
  B0010000 //9
};

int check[3] {
  B1000000,//0
  B1000010,//G
  B1000111//L
};
int lastsw1 = 0;
int lastsw2 = 0;
//                          A  B  C  D  E  F  G
const int segmentPins[8] = {2, 3, 4, 5, 6, 7, 8};
int count = 0;

void switchA() {
  if (digitalRead(button1) == LOW) {
    Serial.print('A');
    if ((millis() - lastsw1) >= 400) {
      select++;
      if (select > 8) {
        select = 0;
      }
      lastsw1 = millis();
    }
  }
}

void switchB() {
  int isBitSet;
  if (digitalRead(button2) == LOW) {
    Serial.print('B');
    if ((millis() - lastsw2) >= 400) {
      //random thing
      lastsw2 = millis();
      if (randNumber == select + 1) {
        for (int segment = 0; segment < 7; segment++) {
          isBitSet = bitRead(check[0], segment);
          digitalWrite(segmentPins[segment], isBitSet);
        }
        delay (2000);
        randNumber = random(1, 10);
      }
      else if (randNumber < select + 1) {
        for (int segment = 0; segment < 7; segment++) {
          isBitSet = bitRead(check[1], segment);
          digitalWrite(segmentPins[segment], isBitSet);
        }
        delay (2000);
      }
      else if (randNumber > select + 1) {
        for (int segment = 0; segment < 7; segment++) {
          isBitSet = bitRead(check[2], segment);
          digitalWrite(segmentPins[segment], isBitSet);
        }
        delay (2000);

      }
    }
  }
}

void setup()
{
  randomSeed(analogRead(A0));
  randNumber = random(1, 10);
  pinMode(button1, INPUT);//pinMode(12,INPUT)
  pinMode(button2, INPUT);//pinMode(13,INPUT)
  Serial.begin(9600);
  for (int i = 0; i < numPins; i++) {
    pinMode(segmentPins[i], OUTPUT);
  }
}

void loop()
{

  switchA();
  switchB();
  delay(100);
  int isBitSet;
  for (int segment = 0; segment < 7; segment++) {
    isBitSet = bitRead(bitPattern[select], segment);
    digitalWrite(segmentPins[segment], isBitSet);
  }
  Serial.println(randNumber);
}
