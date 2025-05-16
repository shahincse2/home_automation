#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>  // ইনস্টল করো: WiFiManager by tzapu

const int ledPin = D0;
ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);  // শুরুতে LED OFF

  // WiFiManager দিয়ে অটো কানেকশন / কনফিগার পোর্টাল
  WiFiManager wifiManager;

  // যদি সংরক্ষিত WiFi তে কানেক্ট না হতে পারে, তাহলে কনফিগার পোর্টাল চালু হবে
  wifiManager.setBreakAfterConfig(true);  // কনফিগার করার পর রিস্টার্ট লাগবে না

  if (!wifiManager.autoConnect("ESP8266-Setup")) {
    Serial.println("WiFi Connect Failed or Timeout!");
    delay(3000);
    ESP.restart();
  }

  Serial.println("Connected to WiFi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // LED Toggle API
  server.on("/led_toggle", HTTP_GET, []() {
    int state = digitalRead(ledPin);
    if (state == HIGH) {
      digitalWrite(ledPin, LOW);
      server.send(200, "text/plain", "OFF");
    } else {
      digitalWrite(ledPin, HIGH);
      server.send(200, "text/plain", "ON");
    }
  });

  // LED Status API
  server.on("/led_status", HTTP_GET, []() {
    int state = digitalRead(ledPin);
    server.send(200, "text/plain", (state == HIGH) ? "ON" : "OFF");
  });

  server.begin();
}

void loop() {
  server.handleClient();
}















// #include <ESP8266WiFi.h>
// #include <ESP8266WebServer.h>
// #include <WiFiManager.h>

// const int ledPin = 2;  // D4 (GPIO2) পিন

// ESP8266WebServer server(80);  // HTTP সার্ভার

// void setup() {
//   Serial.begin(115200);
//   pinMode(ledPin, OUTPUT);
//   digitalWrite(ledPin, LOW);  // LED OFF দিয়ে শুরু

//   WiFiManager wifiManager;

//   // wifiManager.resetSettings(); // একবার ব্যবহারের পর এটা comment করে রাখুন

//   wifiManager.autoConnect("SmartSwitchAP");

//   Serial.println("WiFi Connected!");
//   Serial.print("Device IP: ");
//   Serial.println(WiFi.localIP());

//   // LED Toggle Endpoint
//   server.on("/led_toggle", HTTP_GET, []() {
//   int state = digitalRead(ledPin);
//   if (state == HIGH) {
//     digitalWrite(ledPin, LOW);
//     server.send(200, "text/plain", "OFF");
//   } else {
//     digitalWrite(ledPin, HIGH);
//     server.send(200, "text/plain", "ON");
//   }
// });


//   // LED Status Endpoint
//   server.on("/led_status", HTTP_GET, []() {
//     String status = digitalRead(ledPin) == HIGH ? "ON" : "OFF";
//     server.send(200, "text/plain", status);
//   });
//   server.begin();
// }
// void loop() {
//   server.handleClient();
// }










// #include <ESP8266WiFi.h>
// #include <ESP8266WebServer.h>

// // WiFi credentials
// const char* ssid = "White House";     // আপনার WiFi নাম
// const char* password = "@Shahin20#";  // আপনার WiFi পাসওয়ার্ড

// // LED Pin (D0)
// const int ledPin = D0;

// ESP8266WebServer server(80);  // ওয়েব সার্ভার পোর্ট 80 তে রান করবে

// void setup() {
//   // Serial Monitor শুরু করুন
//   Serial.begin(115200);

//   // LED পিন সেট করা
//   pinMode(ledPin, OUTPUT);
//   digitalWrite(ledPin, LOW);  // LED শুরুতে অফ থাকবে

//   // WiFi কানেক্ট করা
//   WiFi.begin(ssid, password);
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(1000);
//     Serial.println("Connecting to WiFi...");
//   }
//   Serial.println("Connected to WiFi!");
//   Serial.print("IP Address: ");
//   Serial.println(WiFi.localIP());

//   // LED toggle endpoint
//   server.on("/led_toggle", HTTP_GET, []() {
//     int ledState = digitalRead(ledPin);
//     if (ledState == HIGH) {
//       digitalWrite(ledPin, LOW);  // LED অফ করুন
//       server.send(200, "text/plain", "OFF");
//     } else {
//       digitalWrite(ledPin, HIGH);  // LED অন করুন
//       server.send(200, "text/plain", "ON");
//     }
//   });

//   // LED status endpoint
//   server.on("/led_status", HTTP_GET, []() {
//     int ledState = digitalRead(ledPin);
//     if (ledState == HIGH) {
//       server.send(200, "text/plain", "ON");
//     } else {
//       server.send(200, "text/plain", "OFF");
//     }
//   });

//   // ওয়েব সার্ভার চালু করুন
//   server.begin();
// }

// void loop() {
//   server.handleClient();  // ওয়েব রিকুয়েস্ট প্রসেস করা
// }
