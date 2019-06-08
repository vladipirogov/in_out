#include "Arduino.h"
#include <BH1750.h>
#include "lighting.h"
#include "serialdecorator.h"
#include <ELClient.h>
#include <ELClientCmd.h>
#include <ELClientMqtt.h>
#include <EEPROM.h>
#include "hashmap.h"   

#define outPin 8
#define inESPPin1 2
#define inESPPin2 4
#define pushBtnPin 3

//Setting map
Hashmap<int> setting;

//Connection status
bool connected;

// Initialize a connection to esp-link using the normal hardware serial port both for
// SLIP and for debug messages.
ELClient esp(&Serial, &Serial);

// Initialize CMD client (for GetTime)
ELClientCmd cmd(&esp);

// Initialize the MQTT client
ELClientMqtt mqtt(&esp);

BH1750 lightMeter;

Lighting lighting;

SerialDecorator decorator(&lighting);
uint16_t lux = 0;
int outLite;

int compare(char a[],char b[]){
           for(int i=0;a[i]!='\0';i++){
               if(a[i]!=b[i])
                   return 0;
           }
           return 1;
       }

unsigned int print(char* key, int value) {
  int *address = setting.get(key);
  EEPROM.put(*address, value);
  return 0;
}

// Callback made from esp-link to notify of wifi status changes
// Here we just print something out for grins
void wifiCb(void* response) {
  ELClientResponse *res = (ELClientResponse*)response;
  if (res->argc() == 1) {
    uint8_t status;
    res->popArg(&status, 1);

    if(status == STATION_GOT_IP) {
      Serial.println("WIFI CONNECTED");
    } else {
      Serial.print("WIFI NOT READY: ");
      Serial.println(status);
    }
  }
}


// Callback when MQTT is connected
void mqttConnected(void* response) {
  Serial.println("MQTT connected!");
  mqtt.subscribe("home/commandtopic");
  connected = true;
}

// Callback when MQTT is disconnected
void mqttDisconnected(void* response) {
  Serial.println("MQTT disconnected");
  connected = false;
}

// Callback when an MQTT message arrives for one of our subscriptions
void mqttData(void* response) {
  ELClientResponse *res = (ELClientResponse *)response;
  String topic = res->popString();

  String serial_str = res->popString();
  Serial.println(serial_str); 
  
  if (serial_str != "") {
    char buf[serial_str.length() + 1]; 
    serial_str.toCharArray(buf, serial_str.length() + 1);
    char *array[serial_str.length() + 2];
    int i = 0;
    int parameter = 0;
    array[i] = strtok(buf,"=");
    while(array[i]!=NULL) {
      array[++i] = strtok(NULL,"=");
    }
    char* name = array[0] ;
    int value = atoi(array[1]);
    parameter = value;
    if(compare(name, "lightmeter") == 1) {
      if(value == 0) {
        parameter = outLite;
      }
      if(value == 1) {
        parameter = lux;
      }
    }
     int printee = decorator.parseSerialCommand(name, parameter, print);
     Serial.println(printee);
     char buf1[12];
     itoa(printee, buf1, 10);
     mqtt.publish("home/feedbacktopic", buf1);
    String serial_str = "";
  }
}

void mqttPublished(void* response) {
  //Serial.println("MQTT published");
}


void setup(void)
{
  int mintrs = 0;
  int addr_mintrs = 0;
  int maxtrs = 0;
  int addr_maxtrs = 2;

  setting.put("mintrs", &addr_mintrs);
  setting.put("maxtrs", &addr_maxtrs);

  EEPROM.get(addr_mintrs, mintrs);
  EEPROM.get(addr_maxtrs, maxtrs);

  lighting.setMaxLight(maxtrs);
  lighting.setMinLight(mintrs);
  lighting.setDelay(delay);
  lighting.setTime(250);

  pinMode(outPin, OUTPUT);
  pinMode(inESPPin1, INPUT);
  pinMode(inESPPin2, INPUT);
  pinMode(pushBtnPin, INPUT);
  
  Serial.begin(9600);
  lightMeter.begin();
    Serial.println("EL-Client starting!");

  // Sync-up with esp-link, this is required at the start of any sketch and initializes the
  // callbacks to the wifi status change callback. The callback gets called with the initial
  // status right after Sync() below completes.
  esp.wifiCb.attach(wifiCb); // wifi status change callback, optional (delete if not desired)
  bool ok;
  do {
    ok = esp.Sync();      // sync up with esp-link, blocks for up to 2 seconds
    if (!ok) Serial.println("EL-Client sync failed!");
  } while(!ok);
  Serial.println("EL-Client synced!");

  // Set-up callbacks for events and initialize with es-link.
  mqtt.connectedCb.attach(mqttConnected);
  mqtt.disconnectedCb.attach(mqttDisconnected);
  mqtt.publishedCb.attach(mqttPublished);
  mqtt.dataCb.attach(mqttData);
  mqtt.setup();

  //Serial.println("ARDUINO: setup mqtt lwt");
  //mqtt.lwt("/lwt", "offline", 0, 0); //or mqtt.lwt("/lwt", "offline");

  Serial.println("EL-MQTT ready");
}

void loop(void)
{
  esp.Process();
  uint16_t lux = lightMeter.readLightLevel();
  delay(500);

  lighting.property.wifiIn = 0;//digitalRead(inESPPin1);    
  lighting.property.wifiInOff = 0;//digitalRead(inESPPin2); 
  lighting.property.btnStat = digitalRead(pushBtnPin);  

  outLite = decorator.outStateCtrl(lux); //read sensor


  if (outLite == HIGH)
  {
    digitalWrite(outPin, HIGH); // sets the output  value
  }
  else
  {
    digitalWrite(outPin, LOW); // sets the output  value
  }
}