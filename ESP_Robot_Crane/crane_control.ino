#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Replace with your network credentials
const char* ssid = "SSID OF WIFI";
const char* password = "PASSWORD OF WIFI";

// Define motor pins
#define CRANE_MOTOR1_PIN1 D1
#define CRANE_MOTOR1_PIN2 D2
#define CRANE_MOTOR2_PIN1 D3
#define CRANE_MOTOR2_PIN2 D4

ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Initialize motor pins
  pinMode(CRANE_MOTOR1_PIN1, OUTPUT);
  pinMode(CRANE_MOTOR1_PIN2, OUTPUT);
  pinMode(CRANE_MOTOR2_PIN1, OUTPUT);
  pinMode(CRANE_MOTOR2_PIN2, OUTPUT);

  // Define server routes
  server.on("/motor1_up", []() { moveMotor1Up(); server.send(200, "text/plain", "Motor1 Up"); });
  server.on("/motor1_down", []() { moveMotor1Down(); server.send(200, "text/plain", "Motor1 Down"); });
  server.on("/motor2_left", []() { moveMotor2Left(); server.send(200, "text/plain", "Motor2 Left"); });
  server.on("/motor2_right", []() { moveMotor2Right(); server.send(200, "text/plain", "Motor2 Right"); });
  server.on("/stop", []() { stopMotors(); server.send(200, "text/plain", "Motors Stopped"); });

  server.begin();
}

void loop() {
  server.handleClient();
}

// Motor functions
void moveMotor1Up() {
  digitalWrite(CRANE_MOTOR1_PIN1, HIGH);
  digitalWrite(CRANE_MOTOR1_PIN2, LOW);
}

void moveMotor1Down() {
  digitalWrite(CRANE_MOTOR1_PIN1, LOW);
  digitalWrite(CRANE_MOTOR1_PIN2, HIGH);
}

void moveMotor2Left() {
  digitalWrite(CRANE_MOTOR2_PIN1, HIGH);
  digitalWrite(CRANE_MOTOR2_PIN2, LOW);
}

void moveMotor2Right() {
  digitalWrite(CRANE_MOTOR2_PIN1, LOW);
  digitalWrite(CRANE_MOTOR2_PIN2, HIGH);
}

void stopMotors() {
  digitalWrite(CRANE_MOTOR1_PIN1, LOW);
  digitalWrite(CRANE_MOTOR1_PIN2, LOW);
  digitalWrite(CRANE_MOTOR2_PIN1, LOW);
  digitalWrite(CRANE_MOTOR2_PIN2, LOW);
}
