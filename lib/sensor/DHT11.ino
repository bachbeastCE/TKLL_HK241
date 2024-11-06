#include "DHT.h"
#include <Wifi.h>
#include "sensor_module.h"
#include <Arduino.h>


#define WIFI_SSID "your-SSID" //tự taọ ID-PASSWORD
#define WIFI_PASSWORD "your-WIFI-password"

#define FIREBASE_HOST "smarthomeiot-7740b-default-rtdb.firebaseio.com"  
#define FIREBASE_AUTH "RdvTiJl8Wm51jDdu1WJasH9qKTSVFkIhsWAodXtr"

FirebaseData firebaseData
String path = "/"
FirebaseJson json;
FirebaseJsonData jsondata;

DHT dht(4, DHT11); // Khởi tạo đối tượng DHT

void setup() {
  // Kết nối Wi-Fi
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Kết nối Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  if (!Firebase.beginStream(firebaseData.errorReason))
  {
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println();
  }

  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println;

  // Khởi tạo DHT11
  dht.begin();
}

void loop() {
    float temperature = dht.readTemperature();  // Nhiệt độ Celsius
    float humidity = dht.readHumidity();        // Độ ẩm

    // Kiểm tra dữ liệu hợp lệ từ DHT11
    if (isnan(temperature) || isnan(humidity)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
     }

    // In ra nhiệt độ và độ ẩm trên Serial Monitor
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print("°C, Humidity: ");
    Serial.print(humidity);
    Serial.println("%");

    // Gửi dữ liệu lên Firebase
    Firebase.setFloat(firebaseData, "/sensor/temperature", temperature);
    Firebase.setFloat(firebaseData, "/sensor/humidity", humidity);

    // Kiểm tra lỗi
    if (firebaseData.errorReason() != "") {
        Serial.print("Firebase error: ");
        Serial.println(firebaseData.errorReason());
    }

    // Đợi 10 giây trước khi đọc lại
    delay(10000); 
}