#include <Arduino.h>
#include <WiFi.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "esp_camera.h"
#include <ESP32Servo.h>
Servo servo_13;
Servo servo_12;

const int trigPin = 15;     // chân trig của HC-SR04
const int echoPin = 14;     // chân echo của HC-SR04


long duration;
int distance;

// const int FLASH_PIN = 4;

// const char* ssid = "RIO TANG 2";
// const char* password = "20032001";
const char* ssid = "Redmi Note 9S";
const char* password = "fullstack1";

String serverName = "192.168.43.73";   // REPLACE WITH YOUR Raspberry Pi IP ADDRESS
//String serverName = "example.com";   // OR REPLACE WITH YOUR DOMAIN NAME

// String serverPath = "/SyncFolderPBL4/api/folders/file/1/upload";     // The default serverPath should be upload.php
String serverPath = "/upload/";     // The default serverPath should be upload.php

const int serverPort = 8000;



// CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

const int timerInterval = 30000;    // time between each HTTP POST image
unsigned long previousMillis = 0;   // last time image was sent
String response;
void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); 
  Serial.begin(115200);
  servo_13.attach(13);
  servo_12.attach(12);
 	// Serial.begin(115200);
  pinMode(trigPin,OUTPUT);   // chân trig sẽ phát tín hiệu
  pinMode(echoPin,INPUT);    // chân echo sẽ nhận tín hiệu


  servo_12.write(0);
  servo_13.write(90);
  // pinMode(FLASH_PIN, OUTPUT);
  // 

  WiFi.mode(WIFI_STA);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("ESP32-CAM IP Address: ");
  Serial.println(WiFi.localIP());

   camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.frame_size = FRAMESIZE_UXGA;
  config.pixel_format = PIXFORMAT_JPEG; // for streaming
  //config.pixel_format = PIXFORMAT_RGB565; // for face detection/recognition
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 4;
  config.fb_count = 1;
  
  // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
  //                      for larger pre-allocated frame buffer.
  if(config.pixel_format == PIXFORMAT_JPEG){
    if(psramFound()){
      config.jpeg_quality = 10;
      config.fb_count = 2;
      config.grab_mode = CAMERA_GRAB_LATEST;
    } else {
      // Limit the frame size when PSRAM is not available
      config.frame_size = FRAMESIZE_VGA;
      config.fb_location = CAMERA_FB_IN_DRAM;
    }
  } else {
    // Best option for face detection/recognition
    config.frame_size = FRAMESIZE_240X240;
#if CONFIG_IDF_TARGET_ESP32S3
    config.fb_count = 2;
#endif
  }

#if defined(CAMERA_MODEL_ESP_EYE)
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
#endif
  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
  delay(2000); 
  Serial.printf("success");
  
}

void loop() {
  delay(1000);
  String s = sendPhoto();
}

String sendPhoto() {
  String getAll;
  String getBody;

  camera_fb_t * fb = NULL;
  fb = esp_camera_fb_get();
  esp_camera_fb_return(fb);
  fb = esp_camera_fb_get();
  if(!fb) {
    Serial.println("Camera capture failed");
    delay(1000);
    ESP.restart();
  }
  Serial.print(" - Size: ");
  Serial.print(fb->len);
  Serial.println(" bytes");

  // digitalWrite(FLASH_PIN, LOW);
  Serial.println("Connecting to server: " + serverName);
  WiFiClient client;
  if (client.connect(serverName.c_str(), serverPort)) {
    Serial.println("Connection successful!");    
    String filename = "esp32-cam" + String(millis()) + ".jpg";
    Serial.println(filename);
    String head = "--RandomNerdTutorials\r\nContent-Disposition: form-data; name=\"file_uploaded\"; filename=\"" + filename + "\"\r\nContent-Type: image/jpeg\r\n\r\n";
    String tail = "\r\n--RandomNerdTutorials--\r\n";

    uint32_t imageLen = fb->len;
    uint32_t extraLen = head.length() + tail.length();
    uint32_t totalLen = imageLen + extraLen;
  
    client.println("POST " + serverPath + " HTTP/1.1");
    client.println("Host: " + serverName);
    client.println("Content-Length: " + String(totalLen));
    client.println("Content-Type: multipart/form-data; boundary=RandomNerdTutorials");
    client.println();
    client.print(head);
  
    uint8_t *fbBuf = fb->buf;
    size_t fbLen = fb->len;
    for (size_t n=0; n<fbLen; n=n+1024) {
      if (n+1024 < fbLen) {
        client.write(fbBuf, 1024);
        fbBuf += 1024;
      }
      else if (fbLen%1024>0) {
        size_t remainder = fbLen%1024;
        client.write(fbBuf, remainder);
      }
    }   
    client.print(tail);
    // Đợi phản hồi từ server
    Serial.println("Response:");
    while (client.connected()) {
      if(client.available())
        {
            response = client.readString();
            Serial.println("Here is header : ");
            Serial.println("<");
            Serial.println(response);
            Serial.println(">");
            String resCode = response.substring(response.indexOf("Không tái chế"));
            String resNothing = response.substring(response.indexOf("Nothing"));
            resCode.remove(resCode.length()-1);
            resNothing.remove(resNothing.length()-1);
            
            Serial.println(resNothing);
            if(resNothing.equals("Nothing")) {
              break;
            } else {
              if(resCode.equals("Không tái chế"))
              {
                Serial.println(resCode);
                for(int pos = 90;  pos  >10 ;pos -=1)
                  {
                    servo_13.write(pos);
                    delay(10) ;
                  }
              } else {
                for(int pos = 90;  pos  <170 ;pos +=1)
                  {
                    servo_13.write(pos);
                    delay(10) ;
                  }
              }
            }           
            delay(1000);  
            int angle = servo_13.read();
              if(angle > 90)
              {
                for(angle;  angle  >= 90 ;angle -=1)
                      {
                        servo_13.write(angle);
                        delay(10) ;
                      }

              } else {
                for(angle;  angle  <= 90 ;angle +=1)
                      {
                        servo_13.write(angle);
                        delay(10) ;
                      }
              }
              delay(1000); 
              
            break;
          }
        }
    
    esp_camera_fb_return(fb);
    
    int timoutTimer = 10000;
    long startTimer = millis();
    boolean state = false;
    // digitalWrite(FLASH_PIN, LOW);
  }
  return "123";
}
    
    