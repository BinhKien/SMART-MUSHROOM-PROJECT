#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <HardwareSerial.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// WiFi info
#define WIFI_SSID "TOTOLINKB"
#define WIFI_PASSWORD "0123456789"

// Firebase info
#define API_KEY "AIzaSyBzRnhaMbxUYEP075vS8p1iUw2jYH5CPdA"
#define DATABASE_URL "https://doan1-21fc2-default-rtdb.firebaseio.com/"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

HardwareSerial mySerial(2);  // Serial2: RX=21, TX=22

void connectToWiFi() {
  Serial.print("Đang kết nối WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  int attempt = 0;

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
    attempt++;
    if (attempt >= 20) {
      Serial.println("\nKhông kết nối được WiFi. Thử lại...");
      WiFi.disconnect();
      WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
      attempt = 0;
    }
  }

  Serial.println("\nWiFi đã kết nối!");
  Serial.println("IP: " + WiFi.localIP().toString());
}

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600, SERIAL_8N1, 21, 22); // RX=21

  connectToWiFi();

  // Cấu hình Firebase
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  Serial.println("Firebase đã sẵn sàng!");
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Mất kết nối WiFi! Đang thử lại...");
    connectToWiFi();
  }

  if (mySerial.available()) {
    String line = mySerial.readStringUntil('\n');
    Serial.print("ESP nhận: ");
    Serial.println(line);

    // --- Tách giá trị nhiệt độ ---
    int startTemp = line.indexOf("Nhiet do:") + 10;
    int endTemp = line.indexOf("°");
    String nhietdo = line.substring(startTemp, endTemp);

    // --- Tách độ ẩm ---
    int startHum = line.indexOf("Do am:") + 7;
    int endHum = line.indexOf("%");
    String doam = line.substring(startHum, endHum);

    // --- Tách ánh sáng ---
    int startLux = line.indexOf("Anh sang:") + 10;
    int endLux = line.indexOf("lux");
    String anhsang = line.substring(startLux, endLux);
    anhsang.trim(); // loại bỏ khoảng trắng nếu có

    // In ra serial monitor
    Serial.print("Nhiệt độ: "); Serial.println(nhietdo);
    Serial.print("Độ ẩm: "); Serial.println(doam);
    Serial.print("Ánh sáng: "); Serial.println(anhsang);

    // --- Gửi Firebase ---
    if (Firebase.RTDB.setString(&fbdo, "/nhietdo", nhietdo))
      Serial.println("Đã gửi nhiệt độ lên Firebase.");
    else
      Serial.println("Lỗi gửi nhiệt độ: " + fbdo.errorReason());

    if (Firebase.RTDB.setString(&fbdo, "/doam", doam))
      Serial.println("Đã gửi độ ẩm lên Firebase.");
    else
      Serial.println("Lỗi gửi độ ẩm: " + fbdo.errorReason());

    if (Firebase.RTDB.setString(&fbdo, "/anhsang", anhsang))
      Serial.println("Đã gửi ánh sáng lên Firebase.");
    else
      Serial.println("Lỗi gửi ánh sáng: " + fbdo.errorReason());

    Serial.println("----------------------------");
  }
}
