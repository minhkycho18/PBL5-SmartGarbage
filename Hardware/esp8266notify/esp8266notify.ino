#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

const char* ssid = "RIO TANG 2";
const char* password = "20032001";
// const char* ssid = "Redmi Note 9S";
// const char* password = "fullstack1";
const int trigRightPin = D5;     // chân trig của HC-SR04
const int echoRightPin = D6;     // chân echo của HC-SR04

const int trigLeftPin = D7;     // chân trig của HC-SR04
const int echoLeftPin = D8;     // chân echo của HC-SR04
//Your Domain name with URL path or IP address with path
String serverName = "http://192.168.1.4:8000/notify/";
String RECYCLABLE_ID = "1";
String NON_RECYCLABLE_ID = "2";
String STATUS_FULL = "Full";
String STATUS_CLEAN = "Clean";

bool isRightFull = false;
bool isLeftFull = false;


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

bool checkFull(const int trigPin, const int echoPin, String type_id) {
  int countTime = 10;
  int sideCount = 0;
  while(countTime > 0) {
    int distance = calculateDistance(trigPin,echoPin);
    delay(1000);
    Serial.print("Distance measured: ");
    Serial.print(distance);
    Serial.println("cm");   
    countTime--;
    if(distance < 8 ) {
      sideCount++;
    }
    if(distance > 8) {
      Serial.println("  OUT");
      return false;
    }
    Serial.print("ROUND  "); 
    Serial.print(countTime); 
    Serial.print("\t");
  }
  if(sideCount >= 9){
    String s = sendRequest(type_id, STATUS_FULL);
    Serial.println("-----------------------FULL-------------------");
    return true;
  } 
  return false;
}

bool checkClean(const int trigPin, const int echoPin, String type_id) {
  int countTime = 10;
  int sideCount = 0;
  while(countTime > 0) {
    int distance = calculateDistance(trigPin,echoPin);
    delay(1000);
    Serial.print("Distance measured: ");
    Serial.print(distance);
    Serial.println("cm");        
    countTime--;
    if(distance > 8) {
      sideCount++;
    }
    if(distance < 8) {
      Serial.println("  OUT");
      return true;
    }

    Serial.print("ROUND  "); 
    Serial.print(countTime); 
    Serial.print("\t");
  }
  if(sideCount >= 9){
    String s = sendRequest(type_id, STATUS_CLEAN);
    Serial.println("--------------------------CLEANN-----------------------------");
    return false;
  } 
  return true;
}

void loop() {
  // check left full
  if(!isLeftFull) {
    Serial.println("===================Start Check left full===================");
    bool check = checkFull(trigLeftPin, echoLeftPin, RECYCLABLE_ID);
    isLeftFull = check;
  }
  // check right full
  if(!isRightFull) {
    Serial.println("===================Start Check right full===================");
    bool check = checkFull(trigRightPin, echoRightPin, NON_RECYCLABLE_ID);
    isRightFull = check;
  }
  // check left clean
  if(isLeftFull) {
    Serial.println("===================Start Check left clean===================");
    bool check = checkClean(trigLeftPin, echoLeftPin, RECYCLABLE_ID);
    isLeftFull = check;
  }
  // check right clean
  if(isRightFull) {
    Serial.println("===================Start Check right clean==================");
    bool check = checkClean(trigRightPin, echoRightPin, NON_RECYCLABLE_ID);
    isRightFull = check;
  }
    
}

String sendRequest(String type_id, String status) {
  if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;
      
      // Your Domain name with URL path or IP address with path
      http.begin(client, serverName);
      http.addHeader("Content-Type", "application/json");
      String bodyRequest = "{\"type_id\":\"" + type_id + "\",\"status\":\"" + status + "\"}";
      int httpResponseCode = http.POST(bodyRequest);    
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
  return "123";
}