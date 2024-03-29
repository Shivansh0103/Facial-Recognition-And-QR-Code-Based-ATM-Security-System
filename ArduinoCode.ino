
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Arduino_JSON.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "Keypad.h"

//FOR KEYPAD
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns

static const uint8_t D0   = 16;
static const uint8_t D1   = 5;
static const uint8_t D2   = 4;
static const uint8_t D3   = 0;
static const uint8_t D4   = 2;
static const uint8_t D5   = 14;
static const uint8_t D6   = 12;
static const uint8_t D7   = 13;
static const uint8_t D8   = 15;
static const uint8_t D9   = 3;

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {D0, D3, D4, D5};
byte colPins[COLS] = {D6, D7, D8, D9};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

//for wifi
const char* ssid = "RR";
const char* password = "ravi9999";

//Your Domain name with URL path or IP address with path
const char* serverName = "http://djcollection.pythonanywhere.com/cam/1/";

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 9000;

String sensorReadings;
float sensorReadingsArr[3];

void setup() {
  lcd.begin();
  lcd.backlight();
  lcd.print("starting machine");
  Serial.begin(115200);
  delay(1000);
  lcd.clear();

  WiFi.begin(ssid, password);
  lcd.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Setup completed");
}

String httpGETRequest(const char* serverName) {
    WiFiClient client;
    HTTPClient http;

    // Your IP address with path or Domain name with URL path
    http.begin(client, serverName);

    int httpResponseCode = http.GET();

    String payload = "{}";

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      payload = http.getString();
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();

    return payload;
  }
int pass=0,flag=0;
void func1(){
  char key = keypad.getKey();

  if (key) {
    if(key=='A'){
      pass=0;
      goto abc;
    }
    Serial.println(key);
    lcd.clear();
    pass=10*pass+((int)key-48);
    lcd.print(pass);
    goto last;
 abc:   lcd.print("deleted");
        delay(500);
        lcd.clear();
 last:  delay(500);
  }
  if(pass>999 and pass<9999)
    flag=1;
  Serial.println("one time");
}
void loop() {
  
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.print("Welcome");
  delay(3000);
  lcd.clear();
  lcd.print("Enter Userid");
  delay(1000);
  lcd.clear();

    while(flag==0)
    func1();
  Serial.print(pass);
  delay(1000);
    if (pass == 1745) {
       //person 1;
    }
    else if (pass == 1690) {
      //person 2;
    }
    // Send an HTTP POST request depending on timerDelay
    if ((millis() - lastTime) > timerDelay) {
      //Check WiFi connection status
      if (WiFi.status() == WL_CONNECTED) {
        lcd.print("wifi connected");
        WiFiClient client;
        HTTPClient http;
              http.begin(client, serverName);
              http.addHeader("Content-Type", "application/json");
              //get id from pass;
              int httpResponseCode=0;
              httpResponseCode =http.PUT("{\"cam_access\":\"1\"}");
              Serial.print("HTTP Response code: ");
              Serial.println(httpResponseCode);
              delay(1000);
              Serial.print("camera request done");
              
        sensorReadings = httpGETRequest(serverName);
        Serial.println(sensorReadings);
        JSONVar myObject = JSON.parse(sensorReadings);

        // JSON.typeof(jsonVar) can be used to get the type of the var
        if (JSON.typeof(myObject) == "undefined") {
          Serial.println("Parsing input failed!");
          return;
        }

        Serial.print("JSON object = ");
        Serial.println(myObject);
        Serial.println(myObject["identified_face"]);

        // Serial.println(myObject["f_name"]);
        //Serial.println(myObject[0]["balance"]);

      }
      else {
        Serial.println("WiFi Disconnected");
      }
      lastTime = millis();
    }

  }
