#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define API_KEY "" // api_key
#define DATABASE_URL "https://-a99ff-default-rtdb.asia-southeast1.firebasedatabase.app" //database url

#define BUZZER_PIN D6
#define LED1_PIN D1  // Untuk Ruangan1
#define LED2_PIN D2  // Untuk Ruangan2
#define LED3_PIN D5  // Untuk Ruangan3

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

bool signupOK = false;
unsigned long sendDataPrevMillis = 0;

const int ambangCahaya = 2000;
const float ambangSuhu = 30.0;

void aturLED(int pin, bool nyala) {
  digitalWrite(pin, nyala ? HIGH : LOW);
}

bool periksaRuangan(const String& ruangan, float& suhu, int& gerakan, int& cahaya) {
  String base = "/" + ruangan + "/";
  bool valid = true;

  if (Firebase.RTDB.getFloat(&fbdo, base + "suhu"))
    suhu = fbdo.floatData();
  else {
    Serial.println("Gagal baca suhu: " + fbdo.errorReason());
    valid = false;
  }

  if (Firebase.RTDB.getInt(&fbdo, base + "gerakan"))
    gerakan = fbdo.intData();
  else {
    Serial.println("Gagal baca gerakan: " + fbdo.errorReason());
    valid = false;
  }

  if (Firebase.RTDB.getInt(&fbdo, base + "cahaya"))
    cahaya = fbdo.intData();
  else {
    Serial.println("Gagal baca cahaya: " + fbdo.errorReason());
    valid = false;
  }

  Serial.printf("%s | Suhu: %.2f | Gerakan: %d | Cahaya: %d\n", ruangan.c_str(), suhu, gerakan, cahaya);

  if (!valid) return false;

  bool mencurigakan = (gerakan == 0) &&
    ((suhu < ambangSuhu && cahaya < ambangCahaya) ||
     (suhu < ambangSuhu && cahaya > ambangCahaya) ||
     (suhu > ambangSuhu && cahaya < ambangCahaya));

  if (mencurigakan)
    Serial.println("⚠️ Kondisi mencurigakan di " + ruangan);
  else
    Serial.println("✅ Aman di " + ruangan);

  Serial.println("-------------------");
  return mencurigakan;
}

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);

  WiFiManager wm;
  wm.autoConnect("server", "12345678");

  Serial.println("WiFi Tersambung. IP: " + WiFi.localIP().toString());

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback;

  if (Firebase.signUp(&config, &auth, "", "")) {
    signupOK = true;
    Serial.println("Firebase signup berhasil");
  } else {
    Serial.printf("Signup gagal: %s\n", config.signer.signupError.message.c_str());
  }

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void kedipkanLED(int pin, int jumlahKedip = 5, int delayMs = 200) {
  for (int i = 0; i < jumlahKedip; i++) {
    digitalWrite(pin, HIGH);
    delay(delayMs);
    digitalWrite(pin, LOW);
    delay(delayMs);
  }
}

void loop() {
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();

    float suhu1, suhu2, suhu3;
    int gerakan1, gerakan2, gerakan3;
    int cahaya1, cahaya2, cahaya3;

    bool bahaya1 = periksaRuangan("Ruangan1", suhu1, gerakan1, cahaya1);
    bool bahaya2 = periksaRuangan("Ruangan2", suhu2, gerakan2, cahaya2);
    bool bahaya3 = periksaRuangan("Ruangan3", suhu3, gerakan3, cahaya3);

    // LED berkedip jika mencurigakan, mati jika aman
    if (bahaya1) kedipkanLED(LED1_PIN);
    else aturLED(LED1_PIN, LOW);

    if (bahaya2) kedipkanLED(LED2_PIN);
    else aturLED(LED2_PIN, LOW);

    if (bahaya3) kedipkanLED(LED3_PIN);
    else aturLED(LED3_PIN, LOW);

    // Buzzer hanya aktif jika Ruangan1 atau Ruangan3 mencurigakan
    if (bahaya1 || bahaya3) {
      Serial.println("🚨 Kondisi mencurigakan (dengan Buzzer)");
      for (int i = 0; i < 10; i++) {
        digitalWrite(BUZZER_PIN, HIGH);
        delay(200);
        digitalWrite(BUZZER_PIN, LOW);
        delay(200);
      }
    } else {
      Serial.println("✅ Tidak ada kondisi berbahaya yang memicu buzzer.\n");
    }
  }
}

