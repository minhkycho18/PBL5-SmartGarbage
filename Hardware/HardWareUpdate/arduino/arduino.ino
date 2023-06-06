// Cam bien khi gas
#include <Servo.h>
Servo myservo; // create a servo object
const int trig = 6;     // chân trig của HC-SR04
const int echo = 5;     // chân echo của HC-SR04

long duration;
int distance;
void setup() {
  Serial.begin(9600);
  myservo.attach(9); // attaches the servo on pin 9 to the servo object
  pinMode(trig,OUTPUT);   // chân trig sẽ phát tín hiệu
  pinMode(echo,INPUT);    // chân echo sẽ nhận tín hiệu
}
void loop() {
  //cảm biến sóng âm  
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);

  distance = duration * 0.034 / 2;

  Serial.print("Distance measured: ");
  Serial.print(distance);
  Serial.println("cm");
  if(distance < 55 && distance != 0)
  {
    for(int pos = 0;  pos <=90 ;pos +=1)
        {
            myservo.write(pos);
            delay(5) ;
        }
    delay(5000);
    for(int pos = 90;  pos  >0 ;pos -=1)
        {
            myservo.write(pos);
            delay(7) ;
        }
  } else
    myservo.write(0);
}