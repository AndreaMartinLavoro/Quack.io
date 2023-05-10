#include <WiFi.h>
#include <HTTPClient.h>
#include <SPIFFS.h>
#include <string>


/* ============================================================================================= */


const char* _ssid = "WIFI SSID";
const char* _password = "WIFI PASSWORD";
const char* _serverUrl = "API URL";


/* ============================================================================================= */


int initWiFi(char *ssid, char *password) {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi... ");
    do {
      Serial.print(".");
      delay(500);
    } while (WiFi.status() != WL_CONNECTED);
    Serial.println(" Done!");
  }
  return WiFi.status();
}



void initSPIFFS() {
  if (!SPIFFS.begin(true)) {
    Serial.println("Failed to mount SPIFFS");
    return;
  }
  Serial.println("SPIFFS mounted successfully");
}


/* ============================================================================================= */


bool saveFile(const char* fileName, std::string data) {
  File file = SPIFFS.open(fileName, FILE_WRITE);
  if (!file) {
    return false;
  }
  file.write(reinterpret_cast<const uint8_t*>(data.c_str()), sizeof(data));
  file.close();
  return true;
}



std::string readFile(const char* fileName) {
  std::string data = "";
  File file = SPIFFS.open(fileName, "r");
  if (!file) {
    Serial.println("Failed to open file for reading");
    return "";
  }
  do {
    data += static_cast<char>(file.read());
  } while(file.available());
  file.close();
  return data;
}


/* ============================================================================================= */


int HTTPrequest(std::string url, String base64) {
  HTTPClient http;
  http.begin(url.c_str());
  http.addHeader("Content-Type", "application/octet-stream");
  String payload = "file=data:audio/mp3;base64," + base64;
  int httpResponseCode = http.POST(payload);
  http.end();
  return httpResponseCode;
}


/* ============================================================================================= */


void setup() {
  Serial.begin(115200);
  initWiFi(const_cast<char*>(_ssid), const_cast<char*>(_password));
  initSPIFFS();

  saveFile("/audio_file.mp3", "abcdefghilmnopqyaaa");

  std::string data = readFile("/audio_file.mp3");
  Serial.printf("SPIFFS Saved Data: %s\n", data.c_str());

  int http = HTTPrequest(_serverUrl, data.c_str());
  Serial.printf("HTTP Response: %d\n", http);
}





void loop() {

}
