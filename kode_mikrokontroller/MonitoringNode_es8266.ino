#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <Firebase_ESP_Client.h>
#include <DHT.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"


#define API_KEY "" // api_key
#define DATABASE_URL "https://-a99ff-default-rtdb.asia-southeast1.firebasedatabase.app" //database url

#define DHTPIN D4     // GPIO2 (pin D4 di NodeMCU)
#define DHTTYPE DHT11
#define PIR_PIN D6    // GPIO12
#define LDR_PIN A0    // Satu-satunya pin analog ESP8266

DHT dht(DHTPIN, DHTTYPE);

// Firebase
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

bool signupOK = false;
unsigned long sendDataPrevMillis = 0;
const unsigned long interval = 15000;

void kirimFirebase(const String& path, float value) {
  if (Firebase.RTDB.setFloat(&fbdo, path, value)) {
    Serial.println("[Firebase] " + path + " -> Terkirim: " + String(value));
  } else {
    Serial.println("[Firebase] Gagal kirim " + path + ": " + fbdo.errorReason());
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(PIR_PIN, INPUT);
  dht.begin();

  // WiFiManager
  WiFiManager wm;
  if (!wm.autoConnect("Monitoring Ruangan 2", "12345678")) {
    Serial.println("[WiFi] Gagal koneksi!");
    while (true);
  }

  Serial.println("[WiFi] Terhubung ke: " + WiFi.localIP().toString());

  // Firebase setup
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback;

  if (Firebase.signUp(&config, &auth, "", "")) {
    signupOK = true;
    Serial.println("[Firebase] Signup berhasil");
  } else {
    Serial.printf("[Firebase] Signup gagal: %s\n", config.signer.signupError.message.c_str());
  }

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > interval)) {
    sendDataPrevMillis = millis();

    float suhu = dht.readTemperature();
    int gerakan = digitalRead(PIR_PIN);
    int cahayaAsli = analogRead(LDR_PIN);        
    int cahaya = 1023 - cahayaAsli;

    // Tampilkan di Serial Monitor
    Serial.println("\n===== Data Sensor Ruangan 2 =====");
    Serial.println("Suhu     : " + String(suhu) + "°C");
    Serial.println("Gerakan  : " + String(gerakan == 1 ? "Terdeteksi" : "Tidak ada"));
    Serial.println("Cahaya   : " + String(cahaya));
    Serial.println("=================================\n");

    // Kirim ke Firebase
    kirimFirebase("Ruangan2/suhu", suhu);
    kirimFirebase("Ruangan2/gerakan", gerakan);
    kirimFirebase("Ruangan2/cahaya", cahaya);
  }
}
