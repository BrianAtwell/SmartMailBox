#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

#include "BinaryConverter.h"
#include "UpdateCaller.h"
#include "BoolWatch.h"
#include "chartjs.html_hex.h"

const char *ssid = "WifiName";
const char *password = "password";

WebServer server(80);

#define HORIZONTAL_HIGH_LIMIT 4095
#define HORIZONTAL_LOW_LIMIT  3500
#define VERTICAL_HIGH_LIMIT   4095
#define VERTICAL_LOW_LIMIT    3500

#define UPDATE_INTERVAL  500

// GPIO13 Output
const int VerticalIRLEDPin = 13;
// GPIO16 Output
const int HorizontalIRLEDPin = 16;
// GPIO36 (ADC1_CH0)
const int VerticalIRPin = 36;
// GPIO34 (ADC1_CH6)
const int HorizontalIRPin = 34;
// GPIO32
const int DoorSwitchPin = 17;

int HorizontalValue=0;
int VerticalValue=0;
bool DoorSwitchValue=0;

bool hasChanged=false;

int count =0;

BoolWatch DoorSwitchWatcher(&DoorSwitchValue);
BinaryConverter HortizontalConverter(&HorizontalValue, HORIZONTAL_HIGH_LIMIT, HORIZONTAL_LOW_LIMIT);
BinaryConverter VerticalConverter(&VerticalValue, VERTICAL_HIGH_LIMIT, VERTICAL_LOW_LIMIT);

UpdateCaller SensorUpdater(UPDATE_INTERVAL);

void handleRoot() {
  server.send(200, "text/html", chartjs_html);
  for(int i=0; i<50; i++)
  {
    Serial.print(chartjs_html[i]);
  }
  Serial.println("");
  delay(1000);
}

void handleMailboxDataUpdate() {
  char temp[500];
  
  snprintf(temp, 500,"{\
  \"Current\":{\
    \"time\": %d,\
    \"timestr\": null,\
    \"HorizontalIRAnalog\": %d,\
    \"VerticalIRAnalog\": %d,\
    \"HorizontalIRState\": %d,\
    \"VerticalIRState\": %d,\
    \"DoorState\": %d\
  },\
  \"Changes\":\
    [{\
    \"time\": 1126,\
    \"timestr\": null,\
    \"HorizontalIRAnalog\": 4095,\
    \"VerticalIRAnalog\": 4095,\
    \"HorizontalIRState\": 0,\
    \"VerticalIRState\": 0,\
    \"DoorState\": 0\
    }]\
}",
  millis(), HorizontalValue, VerticalValue, HortizontalConverter.getState(),
  VerticalConverter.getState(), DoorSwitchValue);

  server.send(200, "text/html", temp);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(10);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  pinMode(HorizontalIRLEDPin, OUTPUT);
  pinMode(VerticalIRLEDPin, OUTPUT);
  pinMode(HorizontalIRPin, INPUT);
  pinMode(VerticalIRPin, INPUT);
  pinMode(DoorSwitchPin, INPUT);

  // Uncomment below two comments to test program storage space usage
  //Serial.print(chart_2_8_0_hex);
  //Serial.print(jquery_3_5_1_hex);

  digitalWrite(HorizontalIRLEDPin, LOW);
  digitalWrite(VerticalIRLEDPin, LOW);

  Serial.print("");
  Serial.println("SmartMailBox Test");

  HortizontalConverter.setChangeHandler([&] (void) {
      hasChanged=true;
  });
  VerticalConverter.setChangeHandler([&] (void) {
      hasChanged=true; 
  });
  DoorSwitchWatcher.setChangeHandler([&] (void) {
      hasChanged=true; 
  });

  SensorUpdater.SetUpdateHandler([&] (void) {

      //digitalWrite(HorizontalIRLEDPin, HIGH);
      //delay(10);
      //HorizontalValue = analogRead(HorizontalIRPin);
      //digitalWrite(HorizontalIRLEDPin, LOW);

      digitalWrite(VerticalIRLEDPin, HIGH);
      delay(50);
      VerticalValue = analogRead(VerticalIRPin);
      digitalWrite(VerticalIRLEDPin, LOW);
      
      digitalWrite(HorizontalIRLEDPin, HIGH);
      delay(10);
      HorizontalValue = analogRead(HorizontalIRPin);
      digitalWrite(HorizontalIRLEDPin, LOW);

      /*
      if(count%4 == 0)
      {
      digitalWrite(VerticalIRLEDPin, HIGH);
      VerticalValue = analogRead(VerticalIRPin);
      digitalWrite(VerticalIRLEDPin, LOW);
      }
      else if(count%4 == 1)
      {
        digitalWrite(VerticalIRLEDPin, HIGH);
        VerticalValue = analogRead(VerticalIRPin);
      }
      else if(count%4 == 2)
      {
        digitalWrite(VerticalIRLEDPin, LOW);
        VerticalValue = analogRead(VerticalIRPin);
        digitalWrite(VerticalIRLEDPin, HIGH);
      }
      else
      {
        VerticalValue = analogRead(VerticalIRPin);
      }
      count++;
      */

      
      DoorSwitchValue = digitalRead(DoorSwitchPin);
      HortizontalConverter.update();
      VerticalConverter.update();
      DoorSwitchWatcher.update();

      Serial.print("Hortizontal Value: ");
      Serial.println(HorizontalValue);
      Serial.print("Hortizontal State: ");
      Serial.println(HortizontalConverter.getState());

      Serial.print("Vertical Value: ");
      Serial.println(VerticalValue);
      Serial.print("Vertical State: ");
      Serial.println(VerticalConverter.getState());

      Serial.print("Door Switch Value: ");
      Serial.println(DoorSwitchValue);
      
      if(hasChanged)
      {
        //Serial.print("Count: ");
        //Serial.println(count%4);

        /*
        Serial.println("Change Detected!!!");
        Serial.print("Hortizontal Value: ");
        Serial.println(HorizontalValue);
        Serial.print("Hortizontal State: ");
        Serial.println(HortizontalConverter.getState());

        Serial.print("Vertical Value: ");
        Serial.println(VerticalValue);
        Serial.print("Vertical State: ");
        Serial.println(VerticalConverter.getState());

        Serial.print("Door Switch Value: ");
        Serial.println(DoorSwitchValue);
        */

        
        hasChanged=false;
      }
  });

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/smartmail-data.json", handleMailboxDataUpdate);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");

}

void loop() {
  // put your main code here, to run repeatedly:
  SensorUpdater.update();
  server.handleClient();
}
