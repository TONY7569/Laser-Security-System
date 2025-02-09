#include <WiFi.h>
#include <WebServer.h>

#define LDR_PIN 34   // LDR sensor on GPIO34 (Analog)
#define BUZZER 26    // Buzzer on GPIO26
#define LED 27       // LED on GPIO27

const char* ssid = "Your_WiFi_SSID";       // Your WiFi name
const char* password = "Your_WiFi_Password"; // Your WiFi password

WebServer server(80);  // Web server on port 80

int threshold = 600;   // Adjust based on ambient light conditions

void handleRoot() {
    int ldrValue = analogRead(LDR_PIN);
    String message = "Laser Status: " + String(ldrValue) + "<br>";

    if (ldrValue < threshold) {
        message += "🚨 Intruder Detected! 🚨";
    } else {
        message += "✅ Secure Area";
    }

    server.send(200, "text/html", message);
}

void setup() {
    Serial.begin(115200);
    
    pinMode(BUZZER, OUTPUT);
    pinMode(LED, OUTPUT);
    
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println("\nConnected!");
    Serial.println(WiFi.localIP());

    server.on("/", handleRoot);
    server.begin();
}

void loop() {
    int ldrValue = analogRead(LDR_PIN);
    Serial.println(ldrValue);

    if (ldrValue < threshold) {  // If beam is broken
        digitalWrite(BUZZER, HIGH);
        digitalWrite(LED, HIGH);
        Serial.println("🚨 Intruder detected!");
    } else {
        digitalWrite(BUZZER, LOW);
        digitalWrite(LED, LOW);
    }

    server.handleClient();
    delay(100);
}
