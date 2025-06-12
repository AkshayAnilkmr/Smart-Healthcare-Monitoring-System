#include <WiFi.h>
#include <FirebaseESP32.h>
#include "firebase_credentials.h"
#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include "MAX30100_PulseOximeter.h"

#define FIREBASE_HOST "your-project-id.firebaseio.com"
#define FIREBASE_AUTH "your-firebase-database-secret"
#define PULSE_SENSOR_PIN 34

FirebaseData firebaseData;
FirebaseJson json;

void connectToWiFi() {
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi.");
}

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
PulseOximeter pox;
uint32_t tsLastReport = 0;

float bodyTemp = 0.0;
float heartRate = 0.0;
float spo2 = 0.0;

void onBeatDetected() {
  Serial.println("Beat Detected!");
}

void setup() {
  Serial.begin(115200);
  connectToWiFi();
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  if (!mlx.begin()) {
    Serial.println("Error initializing MLX90614");
    while (1);
  }

  if (!pox.begin()) {
    Serial.println("MAX30100 not found");
    while (1);
  }

  pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop() {
  pox.update();
  bodyTemp = mlx.readObjectTempC();
  heartRate = pox.getHeartRate();
  spo2 = pox.getSpO2();

  if (millis() - tsLastReport > 5000) {
    tsLastReport = millis();

    Serial.printf("Temp: %.2f C | HR: %.1f bpm | SpO2: %.1f%%\n", bodyTemp, heartRate, spo2);

    json.clear();
    json.set("temperature", bodyTemp);
    json.set("heart_rate", heartRate);
    json.set("spo2", spo2);

    if (Firebase.updateNode(firebaseData, "/patient_data", json)) {
      Serial.println("Data uploaded to Firebase");
    } else {
      Serial.println("Firebase upload failed: " + firebaseData.errorReason());
    }

    if (heartRate < 50 || heartRate > 120 || spo2 < 90 || bodyTemp > 38.5) {
      Firebase.setString(firebaseData, "/alerts", "Emergency! Check patient vitals.");
      Serial.println("Emergency alert sent!");
    }
  }
}
