#include <Arduino.h>
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
//#include <ESPAsyncTCP.h>
//#include <ESPAsyncWebServer.h>
#include "WiFiManager.h"          //https://github.com/tzapu/WiFiManager

#include <NTPClient.h>
//#include <EEPROM.h>
#include <Timezone.h> 
#include <FastLED.h>
#include <ArduinoOTA.h>
const char* WiFi_hostname = "My-watch";


#define LED_PIN     2
#define NUM_LEDS    110

CRGB leds[NUM_LEDS];
int test_mode = 0;
int brightness = 100; // Initial brightness
int red = 100;
int green = 100;
int blue = 100;
ESP8266WebServer server(80); //Server on port 80

TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120};     // Central European Summer Time
TimeChangeRule CET = {"CET ", Last, Sun, Oct, 3, 60};       // Central European Standard Time
Timezone CE(CEST, CET);

unsigned int localPort = 2390;  
IPAddress timeServerIP; // time.nist.gov NTP server address
 
const char* ntpServerName = "pool.ntp.org";
//uint8_t brightness, red, green, blue;
String LocalTime ="";
String page = "";
WiFiUDP udp;
NTPClient timeClient(udp, ntpServerName);//, 0, 60000);

int hour_low = 55;
int hour_high = 109;
int one_low = 63;
int one_high = 65;
int two_low = 55;
int two_high = 57;
int three_low = 66;
int three_high = 68;
int four_low = 73;
int four_high = 76;
int five_low = 85;
int five_high = 87;
int six_low = 77;
int six_high = 79;
int seven_low = 88;
int seven_high = 90;
int eight_low = 91;
int eight_high = 94;
int nine_low = 96;
int nine_high = 98;
int ten_low = 107;
int ten_high = 109;
int eleven_low = 103;
int eleven_high = 106;
int twelve_low = 99;
int twelve_high = 102;


int min_low = 11;
int min_high = 54;
int half_low = 51;
int half_high = 54;
int in_low = 35;
int in_high = 35;
int over_low = 44;
int over_high = 47;
int fivemin_low = 19;
int fivemin_high = 21;
int tenmin_low = 14;
int tenmin_high = 15;
int fifteenmin_low = 22;
int fifteenmin_high = 26;
int twentymin_low = 39;
int twentymin_high = 43;

int header_low = 0;
int header_high = 10;
int clock_low = 0;
int clock_high = 6;
int is_low = 8;
int is_high = 9;

void closeBulbs(int low, int high){
  for (int i =low; i<= high; i++){
    leds[i] = CRGB (0,0,0);
  }
}

void lightBulbs(int low,int high){
  for (int i =low; i<= high; i++){
    leds[i] = CRGB (red,green,blue);
  }
}

void lightMinutes(int minutes){
  Serial.print(":");
  Serial.println(minutes);
  closeBulbs(min_low,min_high);
  
  switch (minutes){
    case 5:
      lightBulbs(fivemin_low,fivemin_high);
      lightBulbs(over_low,over_high);
      break;
    case 10:
      lightBulbs(tenmin_low,tenmin_high);
      lightBulbs(over_low,over_high);
      break;
    case 15:
      lightBulbs(fifteenmin_low,fifteenmin_high);
      lightBulbs(over_low,over_high);
      break;
    case 20:
      lightBulbs(twentymin_low,twentymin_high);
      lightBulbs(over_low,over_high);
      break;
    case 25:
      lightBulbs(fivemin_low,fivemin_high);
      lightBulbs(in_low,in_high);
      lightBulbs(half_low,half_high);
      break;
    case 30:
      lightBulbs(half_low,half_high);
      break;
    case 35:
      lightBulbs(fivemin_low,fivemin_high);
      lightBulbs(over_low,over_high);
      lightBulbs(half_low,half_high);
      break;
    case 40:
      lightBulbs(twentymin_low,twentymin_high);
      lightBulbs(in_low,in_high);
      break;
    case 45:
      lightBulbs(fifteenmin_low,fifteenmin_high);
      lightBulbs(in_low,in_high);
      break;
    case 50:
      lightBulbs(tenmin_low,tenmin_high);
      lightBulbs(in_low,in_high);
      break;
    case 55:
      lightBulbs(fivemin_low,fivemin_high);
      lightBulbs(in_low,in_high);
  }
  
}
void lightHour(int our){
  Serial.print("Lightning: ");
  Serial.print(our);
  closeBulbs(hour_low,hour_high);
  
  switch (our){
    
    
    case 1:
      lightBulbs(one_low,one_high);
      break;
    case 2:
      lightBulbs(two_low,two_high);
      break;
    case 3:
      lightBulbs(three_low,three_high);
      break;
    case 4:
      lightBulbs(four_low,four_high);
      break;
    case 5:
      lightBulbs(five_low,five_high);
      break;
    case 6:
      lightBulbs(six_low,six_high);
      break;
    case 7:
      lightBulbs(seven_low,seven_high);
      break;
    case 8:
      lightBulbs(eight_low,eight_high);
      break;
    case 9:
      lightBulbs(nine_low,nine_high);
      break;
    case 10:
      lightBulbs(ten_low,ten_high);
      break;
    case 11:
      lightBulbs(eleven_low,eleven_high);
      break;
    case 12:
      lightBulbs(twelve_low,twelve_high);
      break;
  }
  
}
void lightClock(Timezone tz, time_t utc ){
  Serial.println("lightning clock");
  closeBulbs(header_low,header_high);
  lightBulbs(clock_low,clock_high);
  lightBulbs(is_low,is_high);
  
  //char m[4];    // temporary storage for month string (DateStrings.cpp uses shared buffer)
  TimeChangeRule *tcr;        // pointer to the time change rule, use to get the TZ abbrev
  time_t t = tz.toLocal(utc, &tcr);
  //strcpy(m, monthShortStr(month(t)));
  if (minute(t)>=25){
    lightHour(1+hour(t)%12);
  }
  else{
    lightHour(hour(t)%12);
  }
  lightMinutes(minute(t)/5 *5);
  delay(1000);
  FastLED.show();
  Serial.println("Fastled Show");
}

void testingBulbs(){
  lightBulbs(0,109);
  delay(1000);
  FastLED.show();
}
// given a Timezone object, UTC and a string description, convert and print local time with time zone
void printDateTime(Timezone tz, time_t utc, const char *descr)
{
    char buf[40];
    char m[4];    // temporary storage for month string (DateStrings.cpp uses shared buffer)
    TimeChangeRule *tcr;        // pointer to the time change rule, use to get the TZ abbrev

    time_t t = tz.toLocal(utc, &tcr);
    
    strcpy(m, monthShortStr(month(t)));
    sprintf(buf, "%.2d:%.2d:%.2d %s %.2d %s %d %s",
        hour(t), minute(t), second(t), dayShortStr(weekday(t)), day(t), m, year(t), tcr -> abbrev);
    LocalTime = buf;
    Serial.print(buf);
    Serial.print(' ');
    Serial.println(descr);
}


void handleRoot() {
  
  page = String("<!DOCTYPE html>")+
  "<html>"+
"<head>"+
"<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"+
"<style>"+
".slidecontainer {"+
"  width: 75%;"+
"}"+
".slider {"+
"  -webkit-appearance: none;"+
"  width: 100%;"+
"  height: 25px;"+
"  background: #d3d3d3;"+
"  outline: none;"+
"  opacity: 0.7;"+
"  -webkit-transition: .2s;"+
"  transition: opacity .2s;"+
"}"+
""+
".slider:hover {"+
"  opacity: 1;"+
"}"+
""+
".slider::-webkit-slider-thumb {"+
"  -webkit-appearance: none;"+
"  appearance: none;"+
"  width: 25px;"+
"  height: 25px;"+
"  background: #4CAF50;"+
"  cursor: pointer;"+
"}"+
".slider::-moz-range-thumb {"+
"  width: 25px;"+
"  height: 25px;"+
"  background: #4CAF50;"+
"  cursor: pointer;"+
"}"+
"</style>"+
"</head>"+
"<body>"+
  
  "<h4>"+String(LocalTime)+"</h4></br><h4>"+
  String(brightness)+"</h4></br>"+
  "<form action=\"/LED\" method=\"POST\">Brighness:<br><input type=\"number\" name=\"bright\" value="+brightness+"> "+
  "<input type=\"submit\"></form>"+
  "<form action=\"/TESTMODE\" method=\"POST\">TestMode:<br><input type=\"number\" name=\"testmode\" value="+test_mode+"> "+
  "<input type=\"submit\"></form>"+
  "<div class=\"slidecontainer\">"+
  "<form action=\"/CHANGE_COLOR\" method=\"POST\">"+
  "Red: <input type=\"range\" name=\"red\" min=\"0\" max=\"255\" value="+red+" class=\"slider\" id=\"redInput\" oninput=\"redOutput.value = redInput.value\">"+
  "<output name=\"ageOutputName\" id=\"redOutput\">"+red+"</output><p>"+
  "Green: <input type=\"range\" name=\"green\" min=\"0\" max=\"255\" value="+green+" class=\"slider\" id=\"greenInput\" oninput=\"greenOutput.value = greenInput.value\">"+
  "<output name=\"ageOutputName\" id=\"greenOutput\">"+green+"</output><p>"+
  "Blue: <input type=\"range\" name=\"blue\" min=\"0\" max=\"255\" value="+blue+" class=\"slider\" id=\"blueInput\" oninput=\"blueOutput.value = blueInput.value\">"+
  "<output name=\"ageOutputName\" id=\"blueOutput\">"+blue+"</output><p>"+
  "<input type=\"submit\"></form>"+
  "</div>"+
  "</body>";
 //String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", page); //Send web page
}
void testMode(){
  test_mode = server.arg("testmode").toInt();
  if (test_mode == 1){
    Serial.println("Starting test mode");
  }
  
  
  server.sendHeader("Location","/");        // Add a header to respond with a new location for the browser to go to the home page again
  server.send(303);                         // Send it back to the browser with an HTTP status 303 (See Other) to redirect
}

void handleLED() {                          
  brightness=server.arg("bright").toInt();      // Change the state of the LED
  server.sendHeader("Location","/");        // Add a header to respond with a new location for the browser to go to the home page again
  server.send(303);                         // Send it back to the browser with an HTTP status 303 (See Other) to redirect
}
void changeColor() {
  red = server.arg("red").toInt();
  green = server.arg("green").toInt();
  blue = server.arg("blue").toInt();
  //eepromVar1.redeeprom = red;
  //eepromVar1.greeneeprom = green;
  //eepromVar1.blueeeprom = blue;
  //
  //EEPROM.put(0, red);
  //EEPROM.put(4, green);
  //EEPROM.put(8, blue);
  //EEPROM.commit();

  server.sendHeader("Location","/");
  server.send(303);
}

void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
}

void setup() {
  Serial.begin(115200);
  //EEPROM.begin(12);
  //EEPROM.begin(sizeof(MyEEPROMStruct));
  

  // write the data to EEPROM - ignoring anything that might be there already (re-flash is guaranteed)
  
  // put your setup code here, to run once:
  
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(brightness);
  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  //reset settings - for testing
  //wifiManager.resetSettings();
  WiFi.hostname(WiFi_hostname);
  //set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
  wifiManager.setAPCallback(configModeCallback);
  wifiManager.autoConnect("KLOCKA");
  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  if(!wifiManager.autoConnect()) {
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
   // ESP.reset();
    delay(1000);
  } 

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");
  timeClient.begin();
  server.on("/", handleRoot);      //Which routine to handle at root location
  server.on("/LED", HTTP_POST, handleLED);
  server.on("/TESTMODE",HTTP_POST,testMode);
  server.on("/CHANGE_COLOR",HTTP_POST,changeColor);
  server.begin();    
  
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}



void loop() {
  int j;
  //EEPROM.get(0,j);
  //Serial.println(j);
  //EEPROM.get(4,j);
  //Serial.println(j);
  //EEPROM.get(8,j);
  //Serial.println(j);
  //Serial.println(eepromVar2.redeeprom);
  //Serial.println(eepromVar2.greeneeprom);
  //Serial.println(eepromVar2.blueeeprom);
  ArduinoOTA.handle();
  Serial.println(brightness);
  Serial.println(red);
  Serial.println(green);
  Serial.println(blue);
  server.handleClient();   
  timeClient.update();
  printDateTime(CE,timeClient.getEpochTime(),"Stockholm2");
  if (test_mode == 1){
    testingBulbs();
  }
  else {
    lightClock(CE,timeClient.getEpochTime());
  }
  
  
  Serial.println("Loop finished");  
}
