#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

// const char* ssid = "RIO TANG 2";
// const char* password = "20032001";

const char* ssid = "Redmi Note 9S";
const char* password = "fullstack1";
const int trigRightPin = D1;     // chân trig của HC-SR04
const int echoRightPin = D2;     // chân echo của HC-SR04

const int trigLeftPin = D3;     // chân trig của HC-SR04
const int echoLeftPin = D4;     // chân echo của HC-SR04
//Your Domain name with URL path or IP address with path
String serverName = "http://192.168.43.73:8000/notify/";


void setup() {
  Serial.begin(115200); 

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  pinMode(trigRightPin, OUTPUT);   // chân trig sẽ phát tín hiệu
  pinMode(echoRightPin, INPUT);    // chân echo sẽ nhận tín hiệu
  pinMode(trigLeftPin, OUTPUT);   // chân trig sẽ phát tín hiệu
  pinMode(echoLeftPin, INPUT);    // chân echo sẽ nhận tín hiệu
}

void loop() {
  int countTime = 10;
  int leftCount = 0;
  int rightCount = 0;
  while(countTime > 0) {
    int rightDistance = calculateDistance(trigRightPin,echoRightPin);

    if(rightDistance < 8) {
      rightCount++;
    }
    delay(1000);
    int leftDistance = calculateDistance(trigLeftPin,echoLeftPin);
    Serial.print("Distance measured right: ");
    Serial.print(rightDistance);
    Serial.println("cm");    
    Serial.print("Distance measured Left: ");
    Serial.print(leftDistance);
    Serial.println("cm");   
    if(leftDistance < 8) {
      leftCount++;
    }
    if(leftDistance > 15 && rightDistance > 15) {
      break;
    }
    countTime--;
    Serial.println("-------------------------------------------------------");
  }
  if(leftCount >= 9 || rightCount >= 9){
    String s = sendRequest();
  } 
}
int calculateDistance(const int trigPin, const int echoPin) {
    long duration;
    int distance;
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);

    distance = duration * 0.034 / 2;
    return distance;
}
String sendRequest() {
  if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;
      
      http.begin(client, serverName.c_str());
        
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
  return "123";
}
    