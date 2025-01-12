#include <Wire.h>
#include <MPU6050.h>
#include <NewPing.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Motor pins
const int motor1_forward = 17;
const int motor1_backward = 16;
const int motor2_forward = 18;
const int motor2_backward = 19;

// Ultrasonic sensor pins and max distance
const int frontTriggerPin = 5;
const int frontEchoPin = 4;
const int maxDistance = 300; // Maximum distance to ping for (in cm)
NewPing frontSonar(frontTriggerPin, frontEchoPin, maxDistance);

// MPU6050
MPU6050 mpu;

// Calibration offsets for MPU6050
const float normalOffsetX = 0.1536;
const float normalOffsetY = -0.0481;
const float normalOffsetZ = 0.8567;
const float inclinedOffsetX = 0.3618;
const float inclinedOffsetY = -0.0520;
const float inclinedOffsetZ = 0.8062;
const float declinedOffsetX = -0.0867;
const float declinedOffsetY = -0.0317;
const float declinedOffsetZ = 0.8459;
const float flippedOffsetX = -0.1624;
const float flippedOffsetY = -0.0193;
const float flippedOffsetZ = -1.1375;
const float tiltedLeftOffsetX = 0.0991;
const float tiltedLeftOffsetY = -0.2642;
const float tiltedLeftOffsetZ = 0.8057;
const float tiltedRightOffsetX = 0.1348;
const float tiltedRightOffsetY = 0.2463;
const float tiltedRightOffsetZ = 0.8049;

// Wi-Fi credentials
const char* ssid = "";
const char* password = "";

// OpenAI API key
const char* apiKey = "";

// Function prototypes
void stopAllMotors();
void moveForward(int speed);
void moveBackward(int speed);
void turnLeft(int speed);
void turnRight(int speed);
unsigned int checkObstacle(NewPing& sonar);
void initializeMPU();
String readAndDetermineOrientation();
String printSensorLog();
bool connectToWiFiAndAPI(const char* ssid, const char* password, const char* apiKey);
String sendSensorLogToOpenAI(const char* apiKey, const String& sensorLog);
void executeAction(String action, int speed, int duration);
void parseAndExecuteAction(const String& command);

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  
  // Initialize motor pins
  pinMode(motor1_forward, OUTPUT);
  pinMode(motor1_backward, OUTPUT);
  pinMode(motor2_forward, OUTPUT);
  pinMode(motor2_backward, OUTPUT);
  
  // Initialize MPU6050
  initializeMPU();
  
  // Connect to Wi-Fi and API
  connectToWiFiAndAPI(ssid, password, apiKey);
  
  // Print initial sensor log
  String initialLog = printSensorLog();
  sendSensorLogToOpenAI(apiKey, initialLog);
}

void loop() {
  // The loop can be empty or handle periodic actions
  delay(1000); // Optional delay
}

void executeAction(String action, int speed, int duration) {
  if (speed < 120 || speed > 250) {
    Serial.println("Invalid speed. Speed should be between 120 and 250.");
    return;
  }

  if (action == "Forward") {
    moveForward(speed);
    Serial.println("Going Forward");
  } else if (action == "Backward") {
    moveBackward(speed);
    Serial.println("Going Backward");
  } else if (action == "Left") {
    turnLeft(speed);
    Serial.println("Turning Left");
  } else if (action == "Right") {
    turnRight(speed);
    Serial.println("Turning Right");
  } else if (action == "Stop") {
    stopAllMotors();
    Serial.println("Stopped");
  } else {
    stopAllMotors();
    Serial.println("Invalid command");
    return;
  }

  delay(duration); // Perform the action for the specified duration
  stopAllMotors();
  
  // Print sensor log after action
  String sensorLog = printSensorLog();
  sendSensorLogToOpenAI(apiKey, sensorLog);
}

void stopAllMotors() {
  analogWrite(motor1_forward, 0);
  analogWrite(motor1_backward, 0);
  analogWrite(motor2_forward, 0);
  analogWrite(motor2_backward, 0);
}

void moveForward(int speed) {
  analogWrite(motor1_forward, speed);
  analogWrite(motor2_forward, speed);
}

void moveBackward(int speed) {
  analogWrite(motor1_backward, speed);
  analogWrite(motor2_backward, speed);
}

void turnLeft(int speed) {
  analogWrite(motor1_backward, speed);
  analogWrite(motor2_forward, speed);
}

void turnRight(int speed) {
  analogWrite(motor1_forward, speed);
  analogWrite(motor2_backward, speed);
}

unsigned int checkObstacle(NewPing& sonar) {
  return sonar.ping_cm();
}

void initializeMPU() {
  Wire.begin();
  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed");
    while (1);
  }
}

String readAndDetermineOrientation() {
  int16_t ax, ay, az;
  float accelX, accelY, accelZ;

  mpu.getAcceleration(&ax, &ay, &az);
  accelX = (float)ax / 16384.0;
  accelY = (float)ay / 16384.0;
  accelZ = (float)az / 16384.0;

  float minDiff = 100.0;
  String orientation;

  // Determine orientation based on acceleration values
  float diff = abs(accelX - normalOffsetX) + abs(accelY - normalOffsetY) + abs(accelZ - normalOffsetZ);
  if (diff < minDiff) {
    minDiff = diff;
    orientation = "Normal";
  }

  diff = abs(accelX - inclinedOffsetX) + abs(accelY - inclinedOffsetY) + abs(accelZ - inclinedOffsetZ);
  if (diff < minDiff) {
    minDiff = diff;
    orientation = "Inclined";
  }

  diff = abs(accelX - declinedOffsetX) + abs(accelY - declinedOffsetY) + abs(accelZ - declinedOffsetZ);
  if (diff < minDiff) {
    minDiff = diff;
    orientation = "Declined";
  }

  diff = abs(accelX - flippedOffsetX) + abs(accelY - flippedOffsetY) + abs(accelZ - flippedOffsetZ);
  if (diff < minDiff) {
    minDiff = diff;
    orientation = "Flipped";
  }

  diff = abs(accelX - tiltedLeftOffsetX) + abs(accelY - tiltedLeftOffsetY) + abs(accelZ - tiltedLeftOffsetZ);
  if (diff < minDiff) {
    minDiff = diff;
    orientation = "Tilted Left";
  }

  diff = abs(accelX - tiltedRightOffsetX) + abs(accelY - tiltedRightOffsetY) + abs(accelZ - tiltedRightOffsetZ);
  if (diff < minDiff) {
    minDiff = diff;
    orientation = "Tilted Right";
  }

  return orientation;
}

String printSensorLog() {
  unsigned int frontDistance = checkObstacle(frontSonar);
  String orientation = readAndDetermineOrientation();

  String sensorLog = "Front Distance: " + String(frontDistance) + " cm\n";
  sensorLog += "Orientation: " + orientation + "\n";

  // Always include instructions
sensorLog += "\nInstructions:\n";
  sensorLog += "This log contains sensor data from the robot. The robot can receive the following commands:\n";
  sensorLog += "1. Forward <Speed> <Duration>\n";
  sensorLog += "2. Backward <Speed> <Duration>\n";
  sensorLog += "3. Left <Speed> <Duration>\n";
  sensorLog += "4. Right <Speed> <Duration>\n";
  sensorLog += "5. Stop.\n";
  sensorLog += "Duration is in milliseconds. 4000-10000ms\n";
  sensorLog += "Onlya avoid obstacles 10 cm - 40 cm away.\n";
  sensorLog += "motor speed must be 120-250.\n";
  sensorLog += "analyze the sensor data and send a command in the format: <Action> <Speed> <Duration> without any additional explanation.\n";

  return sensorLog;
}

bool connectToWiFiAndAPI(const char* ssid, const char* password, const char* apiKey) {
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");

  return WiFi.status() == WL_CONNECTED;
}

String sendSensorLogToOpenAI(const char* apiKey, const String& sensorLog) {
  String apiUrl = "https://api.openai.com/v1/chat/completions";

  // Escape newline characters in sensorLog
  String escapedSensorLog = sensorLog;
  escapedSensorLog.replace("\n", "\\n");

  // Create JSON payload
  String finalPayload = "{\"model\": \"gpt-3.5-turbo-0125\",\"messages\": [{\"role\": \"assistant\", \"content\": \"" + escapedSensorLog + "\"}]}";
  
  HTTPClient http;

  http.begin(apiUrl);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Authorization", "Bearer " + String(apiKey));

  Serial.print("Sensor Log: ");
  Serial.println(sensorLog); // Print the sensor log instead of the payload

  int httpResponseCode = http.POST(finalPayload);

  if (httpResponseCode == 200) {
    String response = http.getString();

    // Parse JSON response
    DynamicJsonDocument jsonDoc(1024);
    deserializeJson(jsonDoc, response);
    String outputText = jsonDoc["choices"][0]["message"]["content"];
    outputText.remove(outputText.indexOf('\n'));

    Serial.print("AI Response: ");
    Serial.println(outputText);

    http.end();
    parseAndExecuteAction(outputText); // Automatically parse and execute the action
    return outputText;
  } else {
    String response = http.getString();
    Serial.printf("Error sending sensor log to OpenAI. HTTP error code: %d\n", httpResponseCode);
    Serial.println("Response: " + response); // Print the full response for debugging
    http.end();
    return "";
  }
}

void parseAndExecuteAction(const String& command) {
  int firstSpaceIndex = command.indexOf(' ');
  int secondSpaceIndex = command.indexOf(' ', firstSpaceIndex + 1);

  if (firstSpaceIndex != -1 && secondSpaceIndex != -1) {
    String action = command.substring(0, firstSpaceIndex);
    int speed = command.substring(firstSpaceIndex + 1, secondSpaceIndex).toInt();
    int duration = command.substring(secondSpaceIndex + 1).toInt();

    executeAction(action, speed, duration);
  } else {
    Serial.println("Invalid command format. Use 'Action Speed Duration'");
  }
}
