#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <GxEPD2_BW.h>
#include <Fonts/FreeMonoBold9pt7b.h> // Bold for headers
#include <Fonts/FreeMono9pt7b.h>

// --- SETTINGS ---
const char* ssid     = "chillHouse";
const char* password = "chillHouse3000";

// Your static ngrok domain from the Dashboard
String serverUrl = "https://your-unique-name.ngrok-free.app/api/"; 

// Current logged-in user to display (This could be changed by a button)
String currentSummoner = "YourName"; 

// DISPLAY SETUP (GDEY0154D67 1.54")
GxEPD2_BW<GxEPD2_154_GDEY0154D67, GxEPD2_154_GDEY0154D67::HEIGHT> display(GxEPD2_154_GDEY0154D67(2, 3, 1, 0));

void setup() {
  Serial.begin(115200);
  SPI.begin(6, -1, 7, 2); 
  display.init(115200, true, 2, false);
  display.setRotation(1); // Landscape is usually better for stats

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.println("\nWiFi Connected!");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    fetchAndDisplayLoL(currentSummoner);
  }
  delay(60000); // Update every minute to save battery and display life
}

void fetchAndDisplayLoL(String summoner) {
  HTTPClient http;
  // Endpoint matches the Python backend we discussed
  String fullUrl = serverUrl + "profile/" + summoner;
  
  http.begin(fullUrl);
  int httpCode = http.GET();

  if (httpCode == 200) {
    String payload = http.getString();
    DynamicJsonDocument doc(2048);
    deserializeJson(doc, payload);

    display.setFullWindow();
    display.firstPage();
    do {
      display.fillScreen(GxEPD_WHITE);
      display.setTextColor(GxEPD_BLACK);
      
      // Header: Summoner Name
      display.setFont(&FreeMonoBold9pt7b);
      display.setCursor(5, 20);
      display.print(doc["nick"].as<const char*>());
      
      display.drawFastHLine(0, 25, 200, GxEPD_BLACK);

      // Body: Stats
      display.setFont(&FreeMono9pt7b);
      display.setCursor(5, 50);
      display.printf("Level: %d", doc["level"].as<int>());
      
      display.setCursor(5, 80);
      display.printf("Rank: %s %s", doc["tier"].as<const char*>(), doc["rank"].as<const char*>());
      
      display.setCursor(5, 110);
      display.printf("LP: %d", doc["lp"].as<int>());

      // Visual Rank Bar (Optional)
      int lp = doc["lp"].as<int>();
      display.drawRect(5, 130, 100, 10, GxEPD_BLACK);
      display.fillRect(5, 130, lp, 10, GxEPD_BLACK); // Simple 0-100 LP bar

    } while (display.nextPage());
  }
  http.end();
}
