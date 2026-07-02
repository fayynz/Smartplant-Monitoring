#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <DHT.h>

// Helper tambahan dari library Firebase untuk kelancaran token & penanganan data
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// ================= PIN SENSOR & AKTUATOR =================
#define DHTPIN 13
#define DHTTYPE DHT11

#define SOIL_PIN 34
#define LDR_PIN 35
#define RELAY_POMPA 14 

// ================= WI-FI LOKAL =================
const char* ssid = "0X1";
const char* password = "l0vesong";

// ================= CONFIG FIREBASE ASLI =================
#define API_KEY "AIzaSyCm3TaD8TPpR_AM9wR8CdOiyM0PNw0Y4MA"
#define DATABASE_URL "https://monitoring-tanaman-80a42-default-rtdb.firebaseio.com"

// Objek Firebase
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

DHT dht(DHTPIN, DHTTYPE);
unsigned long waktuLama = 0;

void setup() {
  Serial.begin(115200);
  dht.begin();
  
  // Atur PIN Relay sebagai OUTPUT digital standar
  pinMode(RELAY_POMPA, OUTPUT);
  
  // Set HIGH di awal agar pompa langsung dalam kondisi MATI saat booting (Active Low)
  digitalWrite(RELAY_POMPA, HIGH); 

  Serial.println("=====================================");
  Serial.println("     SMARTPLANT MONITORING LIVE      ");
  Serial.println("=====================================");

  // Koneksi ke Wi-Fi
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");
  Serial.print("IP ESP32: ");
  Serial.println(WiFi.localIP());
  
  // Konfigurasi Kredensial Firebase Proyekmu
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  
  // ================================================================
  // JALUR AUTH ANONYMOUS (SESUAI DENGAN YANG SUDAH KAMU AKTIFKAN)
  // ================================================================
  Serial.print("Logging in to Firebase Anon...");
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println(" Success!");
  } else {
    Serial.printf(" Error! %s\n", config.signer.signupError.message.c_str());
  }
  // ================================================================
  
  // Inisialisasi Firebase
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  Serial.println("\nSystem Ready!");
}

void loop() {
  // Jalankan pengiriman dan pembacaan data setiap 5 detik sekali
  if (Firebase.ready() && (millis() - waktuLama > 5000 || waktuLama == 0)) {
    waktuLama = millis();
    
    // 1. BACA DATA SENSOR
    float suhu = dht.readTemperature();
    float kelembaban = dht.readHumidity();
    int tanahRaw = analogRead(SOIL_PIN);
    int cahaya = analogRead(LDR_PIN);

    // Pencegah error jika kabel sensor DHT sempat longgar/dicopot
    if (isnan(suhu)) suhu = 0.0;
    if (isnan(kelembaban)) kelembaban = 0.0;

    Serial.println("========== SENSOR DATA ==========");
    Serial.print("Suhu                : "); Serial.print(suhu); Serial.println(" °C");
    Serial.print("Kelembaban Udara    : "); Serial.print(kelembaban); Serial.println(" %");
    Serial.print("Soil Moisture Raw   : "); Serial.println(tanahRaw);
    Serial.print("LDR Raw             : "); Serial.println(cahaya);
    Serial.println("=================================");

    // 2. KIRIM DATA LIVE SENSOR KE /sensor (OVERWRITE DATA LAMA)
    FirebaseJson json;
    json.set("suhu", suhu);
    json.set("kelembaban_udara", kelembaban);
    json.set("cahaya", cahaya);
    json.set("kelembaban_tanah", tanahRaw);

    Serial.print("Sending live data to /sensor... ");
    if (Firebase.RTDB.setJSON(&fbdo, "/sensor", &json)) {
      Serial.println("Success!");
    } else {
      Serial.print("Failed! Reason: ");
      Serial.println(fbdo.errorReason());
    }

    // 3. BACA STATUS SAKELAR POMPA DARI FIREBASE ONLINE
    Serial.print("Checking Pump Status... ");
    if (Firebase.RTDB.getInt(&fbdo, "/kontrol/pompa")) {
      int statusPompa = fbdo.intData(); // Mengambil angka murni (0 atau 1) dari Firebase
      Serial.printf("Value: %d\n", statusPompa);

      // LOGIKA KENDALIAN SAKELAR (ACTIVE LOW)
      if (statusPompa == 1) {
        digitalWrite(RELAY_POMPA, LOW);   // LOW = Menyalakan relay pompa
        Serial.println(">>> SAKELAR FISIK: POMPA NYALA <<<");
      } 
      else {
        digitalWrite(RELAY_POMPA, HIGH);  // HIGH = Mematikan relay pompa
        Serial.println(">>> SAKELAR FISIK: POMPA MATI <<<");
      }
    } else {
      Serial.print("Failed to read pump status! Reason: ");
      Serial.println(fbdo.errorReason());
    }

    Serial.println("\nWaiting 5 seconds...\n");
  }
}