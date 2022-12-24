#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <FreeRTOS.h>
const char EOPmarker = '=';  //This is the end of packet marker
char serialbuf[8];           //This gives
#include <EEPROM.h>
//#include <TimerOne.h>
#include <TM1637TinyDisplay6.h>
#include <string.h> // we'll need this for subString
#define MAX_STRING_LEN 20  // like 3 lines above, change as needed.
#define DIO 23             // pin 6
#define CLK 19             // pin 5

TM1637TinyDisplay6 display(CLK, DIO);
unsigned int a;
unsigned int b;
unsigned int c;
unsigned int d;
unsigned int e;
unsigned long millis();
unsigned long f;
int weight;
int counter;
unsigned long previousMillis = 0;  //will store last time LED was blinked
const long period = 100;           // period at which to blink in ms

char *subStr(char *input_string, char *separator, int segment_number) {
  char *act, *sub, *ptr;
  static char copy[MAX_STRING_LEN];
  int i;
  strcpy(copy, input_string);
  for (i = 1, act = copy; i <= segment_number; i++, act = NULL) {
    sub = strtok_r(act, separator, &ptr);
    if (sub == NULL) break;
  }
  return sub;
}

const char *SSID = "Student Wifi";
const char *PWD = "gaadapassword";

WebServer server(9950);


StaticJsonDocument<250> jsonDocument;
char buffer[250];



void setup_routing() {
  server.on("/weight", getWeight);
  server.on("/counter", getCounter);
  server.on("/data", getData);

  server.begin();
}

void create_json(char *tag, float value, char *unit) {
  jsonDocument.clear();
  jsonDocument["type"] = tag;
  jsonDocument["value"] = value;
  jsonDocument["unit"] = unit;
  serializeJson(jsonDocument, buffer);
}

void add_json_object(char *tag, float value, char *unit) {
  JsonObject obj = jsonDocument.createNestedObject();
  obj["type"] = tag;
  obj["value"] = value;
  obj["unit"] = unit;
}

void read_sensor_data(void *parameter) {

  for (;;) {
    readSerialC();
    weight = ((d*1000) + (e*1000) + (c*100) + (b*10) + a);
    // read serial data loop
    counter += 1;
    vTaskDelay(1000 / portTICK_PERIOD_MS);  // delay untuk baca data
  }
}

void update_display(void *parameter) {

  for (;;) {
    // update display loop
    uint8_t dots = 0b00100000;
    display.showNumberDec(weight, dots);
    vTaskDelay(1000 / portTICK_PERIOD_MS);  // delay buat update
  }
}

void getWeight() {
  Serial.println("Get weight");
  create_json("weight", weight, "kg");
  server.send(200, "application/json", buffer);
}

void getCounter() {
  Serial.println("Get counter");
  create_json("counter", counter, "item");
  server.send(200, "application/json", buffer);
}



void getData() {
  Serial.println("Get All Data");
  jsonDocument.clear();
  add_json_object("weight", weight, "kg");
  add_json_object("counter", counter, "item");
  serializeJson(jsonDocument, buffer);
  server.send(200, "application/json", buffer);
}



void setup_task() {
  xTaskCreate(
    read_sensor_data,
    "Read sensor data",
    1000,
    NULL,
    1,
    NULL);
  xTaskCreate(
    update_display,
    "Update display",
    128,
    NULL,
    2,
    NULL);
}

void setup() {
  Serial.begin(115200);
  display.setBrightness(BRIGHT_HIGH);
  display.clear();
  Serial2.begin(1200);  // serial 1 digital pin (tx 18,rx 19)
  Serial.print("Connecting to Wi-Fi");

  WiFi.begin(SSID, PWD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

WiFi.softAP(SSID,PWD);
  Serial.print("Connected! IP Address: ");
  Serial.println(WiFi.localIP());
  setup_task();
  setup_routing();
}

void loop() {
  server.handleClient();
}
void readSerialC() {               // 3 spaces 1 dots between values
  if (Serial2.available() > 0) {   //makes sure something is ready to be read
    static int bufpos = 0;         //starts the buffer back at the first position in the incoming serial.read
    char inchar = Serial2.read();  //assigns one byte (as serial.read()'s only input one byte at a time
    if (inchar != EOPmarker) {     //if the incoming character is not the byte that is the incoming package ender
      serialbuf[bufpos] = inchar;  //the buffer position in the array get assigned to the current read
      bufpos++;                    //once that has happend the buffer advances, doing this over and over again until the end of package marker is read.
    } else {                       //once the end of package marker has been read
      serialbuf[bufpos] = '\0';    //restart the buff// end of string
      bufpos = 0;                  //restart the position of the buff


      int p = atoi(subStr(serialbuf, "", 1));
      millis();
      int x = atoi(subStr(serialbuf, "", 1));
      millis();
      int y = atoi(subStr(serialbuf, "", 1));
      millis();
      int z = atoi(subStr(serialbuf, ".", 2));
      a = (p % 1000 / 100);
      b = (x % 100 / 10);
      c = (y % 10);
      d = (z % 100);
      e = (z / 100);

      // Serial.print(a);
      //Serial.print(b);
      //Serial.println(c);
      Serial.println(d);
      // Serial.println(e);
      //Serial.println(z);
      Serial.println(serialbuf);
    }
  }
}
