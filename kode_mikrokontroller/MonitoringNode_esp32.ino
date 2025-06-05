#include <WiFi.h>
#include <WiFiManager.h>
#include <Firebase_ESP_Client.h>
#include <DHT.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"


#define API_KEY "" // api_key
#define DATABASE_URL "https://-a99ff-default-rtdb.asia-southeast1.firebasedatabase.app" //database url
#define DHTPIN 14
#define DHTTYPE DHT11
#define PIR_PIN 13
#define LDR_PIN 34

DHT dht(DHTPIN, DHTTYPE);


FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
const unsigned long interval = 15000;
bool signupOK = false;


void kirimFirebase(const String& path, float value) {
  if (Firebase.RTDB.setFloat(&fbdo, path, value)) {
    Serial.println(path + " terkirim");
  } else {
    Serial.println("Gagal kirim " + path + ": " + fbdo.errorReason());
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(PIR_PIN, INPUT);
  dht.begin();


  WiFiManager wm;
  // wm.resetSettings(); // slalu muncul konfig
  if (!wm.autoConnect("Monitoring Ruanagn 1", "12345678")) {
    Serial.println("Gagal connect ke WiFi");
    while (true);
  }

  Serial.println("WiFi Terhubung: " + WiFi.localIP().toString());

  // ====== Setup Firebase ======
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback;

  if (Firebase.signUp(&config, &auth, "", "")) {
    signupOK = true;
    Serial.println("Firebase signup sukses");
  } else {
    Serial.printf("Signup gagal: %s\n", config.signer.signupError.message.c_str());
  }

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > interval)) {
    sendDataPrevMillis = millis();
    int gerakan1 = digitalRead(PIR_PIN);
    int cahaya1 = analogRead(LDR_PIN);
    float suhu1 = dht.readTemperature();

    kirimFirebase("Ruangan1/suhu", suhu1);
    kirimFirebase("Ruangan1/gerakan", gerakan1);
    kirimFirebase("Ruangan1/cahaya", cahaya1);
  }
}
