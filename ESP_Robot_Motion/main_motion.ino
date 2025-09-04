#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// Replace with your network credentials
const char* ssid = "SSID OF WIFI";
const char* password = "PASSWORD OF WIFI";

// Define motor control pins
#define IN1 D1
#define IN2 D2
#define IN3 D3
#define IN4 D4
#define TRIG_PIN D5  // Trigger pin for ultrasonic sensor
#define ECHO_PIN D6  // Echo pin for ultrasonic sensor

WiFiServer server(80);

// Define movement durations for each destination (in milliseconds)
struct Destination {
  int forward1Time;
  int stop1Time;
  int leftTime;
  int stop2Time;
  int forward2Time;
  int stop3Time;
  int rightTime;
  int backwardTime;
  int finalStopTime;
};

Destination destinations[3];

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Connect to WiFi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println("Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Initialize motor control pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  // Initialize ultrasonic sensor pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Initialize destinations with default values
  for (int i = 0; i < 3; i++) {
    destinations[i] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
  }

  // Start the server
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("Client connected");
    String request = client.readStringUntil('\r');
    Serial.println(request);
    client.flush();

    // Parse the commands
    if (request.indexOf("GET /set_destination") != -1) {
      setDestination(request);
    } else if (request.indexOf("GET /go_to_destination") != -1) {
      int destIndex = request.substring(request.lastIndexOf('=') + 1).toInt();
      goToDestination(destIndex);
    } else if (request.indexOf("GET /voice_command") != -1) {
      String command = request.substring(request.lastIndexOf('=') + 1);
      executeVoiceCommand(command);
    }

    // Send response to the client
    sendResponse(client);

    client.stop();
    Serial.println("Client disconnected");
  }
}

void setDestination(String request) {
  int destIndex = request.substring(request.indexOf("index=") + 6, request.indexOf("&forward1")).toInt();
  int forward1Time = request.substring(request.indexOf("forward1=") + 9, request.indexOf("&stop1")).toInt();
  int stop1Time = request.substring(request.indexOf("stop1=") + 6, request.indexOf("&left")).toInt();
  int leftTime = request.substring(request.indexOf("left=") + 5, request.indexOf("&stop2")).toInt();
  int stop2Time = request.substring(request.indexOf("stop2=") + 6, request.indexOf("&forward2")).toInt();
  int forward2Time = request.substring(request.indexOf("forward2=") + 9, request.indexOf("&stop3")).toInt();
  int stop3Time = request.substring(request.indexOf("stop3=") + 6, request.indexOf("&right")).toInt();
  int rightTime = request.substring(request.indexOf("right=") + 6, request.indexOf("&backward")).toInt();
  int backwardTime = request.substring(request.indexOf("backward=") + 9, request.indexOf("&finalStop")).toInt();
  int finalStopTime = request.substring(request.indexOf("finalStop=") + 10).toInt();

  if (destIndex >= 0 && destIndex < 3) {
    destinations[destIndex] = {forward1Time, stop1Time, leftTime, stop2Time, forward2Time, stop3Time, rightTime, backwardTime, finalStopTime};
    Serial.println("Destination " + String(destIndex + 1) + " set");
  }
}

void goToDestination(int index) {
  if (index >= 0 && index < 3) {
    Destination dest = destinations[index];
    moveForward(dest.forward1Time);
    stopMoving(dest.stop1Time);
    turnLeft(dest.leftTime);
    stopMoving(dest.stop2Time);
    moveForward(dest.forward2Time);
    stopMoving(dest.stop3Time);
    turnRight(dest.rightTime);
    moveBackward(dest.backwardTime);
    stopMoving(dest.finalStopTime);
    Serial.println("Moved to destination " + String(index + 1));
  }
}

void sendResponse(WiFiClient &client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<h1>NodeMCU Robot Control</h1>");
  
  // Destination setting form
  for (int i = 0; i < 3; i++) {
    client.println("<h2>Set Destination " + String(i + 1) + "</h2>");
    client.println("<form action='/set_destination' method='get'>");
    client.println("<input type='hidden' name='index' value='" + String(i) + "'>");
    client.println("Forward 1 Time (ms): <input type='number' name='forward1' value='" + String(destinations[i].forward1Time) + "'><br>");
    client.println("Stop 1 Time (ms): <input type='number' name='stop1' value='" + String(destinations[i].stop1Time) + "'><br>");
    client.println("Left Turn Time (ms): <input type='number' name='left' value='" + String(destinations[i].leftTime) + "'><br>");
    client.println("Stop 2 Time (ms): <input type='number' name='stop2' value='" + String(destinations[i].stop2Time) + "'><br>");
    client.println("Forward 2 Time (ms): <input type='number' name='forward2' value='" + String(destinations[i].forward2Time) + "'><br>");
    client.println("Stop 3 Time (ms): <input type='number' name='stop3' value='" + String(destinations[i].stop3Time) + "'><br>");
    client.println("Right Turn Time (ms): <input type='number' name='right' value='" + String(destinations[i].rightTime) + "'><br>");
    client.println("Backward Time (ms): <input type='number' name='backward' value='" + String(destinations[i].backwardTime) + "'><br>");
    client.println("Final Stop Time (ms): <input type='number' name='finalStop' value='" + String(destinations[i].finalStopTime) + "'><br>");
    client.println("<input type='submit' value='Set Destination " + String(i + 1) + "'>");
    client.println("</form>");
  }

  // Destination selection form
  client.println("<h2>Go to Destination</h2>");
  client.println("<form action='/go_to_destination' method='get'>");
  client.println("<select name='destination'>");
  for (int i = 0; i < 3; i++) {
    client.println("<option value='" + String(i) + "'>Destination " + String(i + 1) + "</option>");
  }
  client.println("</select>");
  client.println("<input type='submit' value='Go'>");
  client.println("</form>");

  // Voice command form
  client.println("<h2>Send Voice Command</h2>");
  client.println("<form action='/voice_command' method='get'>");
  client.println("Command: <input type='text' name='command'><br>");
  client.println("<input type='submit' value='Send Voice Command'>");
  client.println("</form>");

  client.println("</html>");
}

// Movement functions with obstacle detection
void moveForward(int time) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  delayUntilClear(time);
}

void moveBackward(int time) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  delayUntilClear(time);
}

void turnLeft(int time) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  delayUntilClear(time);
}

void turnRight(int time) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  delayUntilClear(time);
}

void stopMoving(int time) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  delay(time);
}

// Delay with obstacle detection
void delayUntilClear(int time) {
  long startTime = millis();
  while (millis() - startTime < time) {
    if (detectObstacle()) {
      stopMoving(0);
      while (detectObstacle()) {
        delay(100);  // Wait until obstacle clears
      }
    }
    delay(100);
  }
}

// Obstacle detection function
bool detectObstacle() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  long distance = duration * 0.034 / 2;

  return distance < 10;  // Return true if obstacle is closer than 10 cm
}

// Execute the voice commands received from the mobile app
void executeVoiceCommand(String command) {
  command.toLowerCase();
  Serial.println("Received voice command: " + command);

  if (command.indexOf("forward") != -1) {
    moveForward(2000);
  } else if (command.indexOf("backward") != -1) {
    moveBackward(2000);
  } else if (command.indexOf("left") != -1) {
    turnLeft(1000);
  } else if (command.indexOf("right") != -1) {
    turnRight(1000);
  } else if (command.indexOf("stop") != -1) {
    stopMoving(1000);
  } else {
    Serial.println("Unknown command");
  }
}
