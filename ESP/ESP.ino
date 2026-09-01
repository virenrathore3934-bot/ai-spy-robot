#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>

#include "WebPage.h"

// ==========================================
// CONFIGURATION
// ==========================================
const char* ssid     = "ALANKRIT";     
const char* password = "987654321"; 
const char* openAiApiKey = "YOUR_GROK_API-KEY_HERE";
String esp32CamIp = "http://10.192.159.135:81/stream";

// ==========================================
// HARDWARE PINS
// ==========================================
const int IN1 = D1; 
const int IN2 = D2; 
const int IN3 = D3; 
const int IN4 = D4; 
const int ISD_PLAYE = D5; 

const int SERVO_HEAD_PIN       = D6; 
const int SERVO_LEFT_HAND_PIN  = D7; 
const int SERVO_RIGHT_HAND_PIN = D8; 

// Objects
Servo servoHead;
Servo servoLeftHand;
Servo servoRightHand;
ESP8266WebServer server(80);

// Servo Positions
int headPos = 90;
int leftHandPos = 0;
int rightHandPos = 180;

// Failsafe Timer
unsigned long lastDriveTime = 0;
const unsigned long DRIVE_TIMEOUT = 5000;
bool isMoving = false;

void stopMotors() {
  digitalWrite(IN1, LOW);  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);  digitalWrite(IN4, LOW);
  isMoving = false;
}

void markDriveAction() {
  lastDriveTime = millis();
  isMoving = true;
}

void moveForward() { 
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);  
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW); 
  markDriveAction();
}

void moveBackward() { 
  digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH); 
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH); 
  markDriveAction();
}

void turnLeft() { 
  digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH); 
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);  
  markDriveAction();
}

void turnRight() { 
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);  
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH); 
  markDriveAction();
}

void triggerIsdModule() {
  digitalWrite(ISD_PLAYE, HIGH);
  yield(); 
  digitalWrite(ISD_PLAYE, LOW);
}

void moveServo(Servo &s, int pos) {
  s.write(pos);
  yield(); 
}

void setup() {
  Serial.begin(115200);

  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(ISD_PLAYE, OUTPUT);
  stopMotors();
  digitalWrite(ISD_PLAYE, LOW);

  servoHead.attach(SERVO_HEAD_PIN, 500, 2400);
  servoLeftHand.attach(SERVO_LEFT_HAND_PIN, 500, 2400);
  servoRightHand.attach(SERVO_RIGHT_HAND_PIN, 500, 2400);

  moveServo(servoHead, headPos);      
  moveServo(servoLeftHand, leftHandPos);   
  moveServo(servoRightHand, rightHandPos);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }
  Serial.println("\nConnected!");
  WiFi.setSleepMode(WIFI_NONE_SLEEP);

  Serial.print("Spy Robot IP: http://");
  Serial.println(WiFi.localIP());

  server.on("/", []() { server.send(200, "text/html", getDashboardHTML(esp32CamIp, openAiApiKey)); });
  server.on("/up", []() { moveForward(); server.send(200, "text/plain", "F"); });
  server.on("/down", []() { moveBackward(); server.send(200, "text/plain", "B"); });
  server.on("/left", []() { turnLeft(); server.send(200, "text/plain", "L"); });
  server.on("/right", []() { turnRight(); server.send(200, "text/plain", "R"); });
  server.on("/stop", []() { stopMotors(); server.send(200, "text/plain", "S"); });

  server.on("/head_left", []() { headPos = constrain(headPos + 30, 0, 180); moveServo(servoHead, headPos); server.send(200, "text/plain", "HL"); });
  server.on("/head_right", []() { headPos = constrain(headPos - 30, 0, 180); moveServo(servoHead, headPos); server.send(200, "text/plain", "HR"); });
  server.on("/head_center", []() { headPos = 90; moveServo(servoHead, headPos); server.send(200, "text/plain", "HC"); });

  server.on("/right_arm_up", []() { rightHandPos = 0; moveServo(servoRightHand, rightHandPos); server.send(200, "text/plain", "RU"); });
  server.on("/right_arm_down", []() { rightHandPos = 180; moveServo(servoRightHand, rightHandPos); server.send(200, "text/plain", "RD"); });
  server.on("/left_arm_up", []() { leftHandPos = 180; moveServo(servoLeftHand, leftHandPos); server.send(200, "text/plain", "LU"); });
  server.on("/left_arm_down", []() { leftHandPos = 0; moveServo(servoLeftHand, leftHandPos); server.send(200, "text/plain", "LD"); });

  server.on("/arms_up", []() { leftHandPos = 180; rightHandPos = 0; moveServo(servoLeftHand, leftHandPos); moveServo(servoRightHand, rightHandPos); server.send(200, "text/plain", "AU"); });
  server.on("/arms_down", []() { leftHandPos = 0; rightHandPos = 180; moveServo(servoLeftHand, leftHandPos); moveServo(servoRightHand, rightHandPos); server.send(200, "text/plain", "AD"); });

  server.on("/playsound", []() { triggerIsdModule(); server.send(200, "text/plain", "snd"); });

  server.begin();
}

void loop() {
  server.handleClient();

  if (isMoving && (millis() - lastDriveTime >= DRIVE_TIMEOUT)) {
    stopMotors();
  }
}