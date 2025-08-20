#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL6VOQHomYK"
#define BLYNK_TEMPLATE_NAME "Object Detect"
#define BLYNK_AUTH_TOKEN "YourBlynkAuthToken"

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <BlynkSimpleEsp32.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>

// Wi-Fi credentials
char ssid[] = "YourWiFiSSID";
char pass[] = "YourWiFiPassword";
String Message = "Emergency Button Pressed. Please assist immediately."; // Message for emergency alert
String Message2 = "Urgent: Obstacle detected near user. Please assist immediately.";

const int trigPin = 33;  // TRIG connected to GPIO 33
const int echoPin = 18;  // ECHO connected to GPIO 18 (via voltage divider)
const int buzzer = 25;   // Buzzer connected to GPIO 25
const int pushButton = 32; // Push Button for emergency alert
#define MOTOR_PIN 14  // GPIO14 connected to TIP120 Base

// GPS-related variables
static const int RXPin = 16, TXPin = 17;  // Define GPS TX and RX pins (updated)
static const uint32_t GPSBaud = 115200;     // Set the baud rate for GPS communication
HardwareSerial gpsSerial(2);              // Use UART2 for GPS serial communication
TinyGPSPlus gps;                          // Create an instance of TinyGPSPlus to decode GPS data

// Virtual Pin for distance value on the Blynk app
#define VIRTUAL_PIN V0
#define SWITCH_PIN V3  // Virtual Pin for controlling buzzer switch
#define BUZZER_SWITCH_PIN V4  // Virtual Pin for controlling buzzer via a switch

unsigned long old = 0;
unsigned long current = 0;
int interval = 10;
int interval2 = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("\nPlease wait for Blynk Server connection");
  pinMode(trigPin, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(pushButton, INPUT_PULLUP);  // Use internal pull-up resistor for the button

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  wifi_testing();
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass); // Initialize Blynk connection
  
  // Initialize GPS serial communication
  gpsSerial.begin(GPSBaud, SERIAL_8N1, RXPin, TXPin);
  Serial.println("GPS Serial Initialized. Waiting for GPS data...");
}

void loop() {
  Blynk.run();
  wifi_testing();
  
  // Measure distance (existing functionality)
  measureDistance();
  
  // Check emergency button press (existing functionality)
  checkEmergencyButton();  
  
  // GPS-related functionality
  gpsFunctionality();  // Handle GPS data and events

  delay(1000);  // Delay to avoid excessive polling
}

void measureDistance() {
  long duration;
  float distance_cm;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance_cm = (duration * 0.0343) / 2;

  Serial.print("Distance: ");
  Serial.print(distance_cm);
  Serial.println(" cm");

  Blynk.virtualWrite(VIRTUAL_PIN, distance_cm);

  // Trigger buzzer based on distance
  if (distance_cm < 50) {
    // Faster beeping (shorter ON/OFF times)
    digitalWrite(buzzer, HIGH);
    digitalWrite(MOTOR_PIN, HIGH); // Motor ON
    delay(500);
    digitalWrite(buzzer, LOW);
    digitalWrite(MOTOR_PIN, LOW); // Motor OFF
    delay(500);
  } else if (distance_cm < 100) {
    // Slower beeping
    digitalWrite(buzzer, LOW);
    digitalWrite(MOTOR_PIN, HIGH); // Motor ON
    delay(50);   // Beep ON for 50ms
    digitalWrite(buzzer, HIGH);
    digitalWrite(MOTOR_PIN, LOW); // Motor OFF
    delay(50);   // Beep OFF for 50ms
  } else {
    // Stop the buzzer
    digitalWrite(buzzer, HIGH);
    digitalWrite(MOTOR_PIN, LOW); // Motor OFF
  }

  // If distance is below 5 cm, send emergency message to Blynk
  if (distance_cm < 5) {
    Blynk.logEvent("distance_low", Message2);  // Send emergency notification
    Serial.println("Urgent: Obstacle detected near user. Please assist immediately.");
  }
}

void checkEmergencyButton() {
  if (digitalRead(pushButton) == LOW) {  // If button is pressed
    Serial.println("Emergency Button Pressed. Send help immediately.");
    Blynk.logEvent("notify_emergency", Message);  // Send emergency notification

    // Wait until the button is released to avoid multiple triggers
    while (digitalRead(pushButton) == LOW) {
      delay(50);  // Small delay to debounce the button press
    }
  }
}

void gpsFunctionality() {
  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
    
    if (gps.location.isUpdated()) {
      float latitude = gps.location.lat();
      float longitude = gps.location.lng();
      
      // Update the emergency messages with the GPS location
      Message = "Emergency Button Pressed. Please assist immediately. Location - Latitude: " + String(latitude, 6) + " Longitude: " + String(longitude, 6);
      Message2 = "Urgent: Obstacle detected near user. Please assist immediately. Location - Latitude: " + String(latitude, 6) + " Longitude: " + String(longitude, 6);

      // Send updated messages to Blynk (virtual pins for GPS data)
      Blynk.virtualWrite(V1, latitude, 6);   
      Blynk.virtualWrite(V2, longitude, 6);  
      
      // Trigger the emergency message via Blynk event when GPS data updates
      if (latitude != 0.0 && longitude != 0.0) {
        String gpsMessage = "Emergency: User located at Latitude: " + String(latitude, 6) + " Longitude: " + String(longitude, 6);
        Blynk.logEvent("gps_location", gpsMessage);
      }
      
      // Print latitude and longitude to the serial monitor
      Serial.print("Latitude: ");
      Serial.print(latitude, 6);
      Serial.print(" Longitude: ");
      Serial.println(longitude, 6);
    }
  }
}

void wifi_testing() {
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
    current = millis();
    if ((current - old) / 1000 > interval)
      ESP.restart();
  }
}

BLYNK_WRITE(SWITCH_PIN) {
  int switchState = param.asInt();  // Get the value of the switch (1 or 0)
  
  Serial.print("Switch State: ");
  Serial.println(switchState);  // Print the switch state for debugging
  
  if (switchState == 1) {
    Serial.println("Switch ON: Performing related action.");
    // Perform action when the switch is turned ON (e.g., activate motor or alert)
  } else {
    Serial.println("Switch OFF: Performing related action.");
    // Perform action when the switch is turned OFF (e.g., deactivate motor or stop alert)
  }
}

BLYNK_WRITE(BUZZER_SWITCH_PIN) {
  int buzzerState = param.asInt();  // Get the value of the new switch (1 or 0)
  
  Serial.print("Buzzer Switch State: ");
  Serial.println(buzzerState);  // Print the buzzer switch state for debugging
  
  if (buzzerState == 1) {
    Serial.println("Buzzer ON: Activating buzzer.");
    digitalWrite(buzzer, LOW); // Turn on the buzzer (LOW if active low)
  } else {
    Serial.println("Buzzer OFF: Deactivating buzzer.");
    digitalWrite(buzzer, HIGH); // Turn off the buzzer (HIGH if active high)
  }
}
