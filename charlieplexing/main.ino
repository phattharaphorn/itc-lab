void setup() 
{ 
 
}
void loop() {
 pinMode(2, OUTPUT);
 digitalWrite(2,LOW);
 pinMode(3, OUTPUT); 
 digitalWrite(3,HIGH);
 pinMode(4, INPUT); 

 delay(500);

 pinMode(2, OUTPUT);
 digitalWrite(2,HIGH);
 pinMode(3, OUTPUT); 
 digitalWrite(3,LOW);
 pinMode(4, INPUT); 

delay(500);

 ;pinMode(4, OUTPUT);
 digitalWrite(4,HIGH);
 pinMode(3, OUTPUT); 
 digitalWrite(3,LOW);
 pinMode(2, INPUT); 

 delay(500);
 
 pinMode(3, OUTPUT);
 digitalWrite(3,HIGH);
 pinMode(4, OUTPUT); 
 digitalWrite(4,LOW);
 pinMode(2, INPUT);

 delay(500);
 
 pinMode(4, OUTPUT);
 digitalWrite(4,HIGH);
 pinMode(2, OUTPUT); 
 digitalWrite(2,LOW);
 pinMode(3, INPUT);

 delay(500);
 
 pinMode(2, OUTPUT);
 digitalWrite(2,HIGH);
 pinMode(4, OUTPUT); 
 digitalWrite(4,LOW);
 pinMode(3, INPUT);

 delay(500);

//L5
 pinMode(4, OUTPUT);
 digitalWrite(4,HIGH);
 pinMode(2, OUTPUT); 
 digitalWrite(2,LOW);
 pinMode(3, INPUT);

 delay(500);
//L4
 pinMode(3, OUTPUT);
 digitalWrite(3,HIGH);
 pinMode(4, OUTPUT); 
 digitalWrite(4,LOW);
 pinMode(2, INPUT);
 
 delay(500);
// L3
 pinMode(4, OUTPUT);
 digitalWrite(4,HIGH);
 pinMode(3, OUTPUT); 
 digitalWrite(3,LOW);
 pinMode(2, INPUT);

  delay(500);
//L2
 pinMode(2, OUTPUT);
 digitalWrite(2,HIGH);
 pinMode(3, OUTPUT); 
 digitalWrite(3,LOW);
 pinMode(4, INPUT); 
 
 delay(500);

 
}
 
