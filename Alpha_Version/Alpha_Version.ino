/*
##Xively WiFi Sensor Tutorial##
This sketch is designed to take sensors (from photocell) and upload the values to Xively
at consistant intervals. At the same time it gets a setable value from Xively to adjust the brigthness
of an LED. This sketch is reusable and can be adapted for use with many different sensors.
Derived from Xively Ardino Sensor Client by Sam Mulube.
 
By Calum Barnes 3-4-2013
BSD 3-Clause License - [http://opensource.org/licenses/BSD-3-Clause]
Copyright (c) 2013 Calum Barnes
*/
#include <SPI.h>
#include <Ethernet.h>
#include <HttpClient.h>
#include <Xively.h>
 
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0E, 0x93, 0xA1 }; 
// Your Xively key to let you upload data
char xivelyKey[] = "Pkr4vEXNzuPdGCuPMy6nb4LMLYjyMsH24bymRUpFyGX7PC4Z";
//your xively feed ID
#define xivelyFeed 1423904035
//datastreams
char sensorID[] = "light";
char ledID[] = "led";
 
// Analog pin which we're monitoring (0 and 1 are used by the Ethernet shield)
#define sensorPin A2
//led connected pin
#define ledPin 9
 
// Define the strings for our datastream IDs
XivelyDatastream datastreams[] = {
  XivelyDatastream(sensorID, strlen(sensorID), DATASTREAM_FLOAT),
  XivelyDatastream(ledID, strlen(ledID), DATASTREAM_FLOAT),
};
// Finally, wrap the datastreams into a feed
XivelyFeed feed(xivelyFeed, datastreams, 2 /* number of datastreams */);
 
EthernetClient client;
XivelyClient xivelyclient(client);
 
//void printWifiStatus() {
//  // print the SSID of the network you're attached to:
//  Serial.print("SSID: ");
//  Serial.println(WiFi.SSID());
// 
//  // print your WiFi shield's IP address:
//  IPAddress ip = WiFi.localIP();
//  Serial.print("IP Address: ");
//  Serial.println(ip);
// 
//  // print the received signal strength:
//  long rssi = WiFi.RSSI();
//  Serial.print("signal strength (RSSI):");
//  Serial.print(rssi);
//  Serial.println(" dBm \n");
//}
void setup() {
  // put your setup code here, to run once:
  delay(1000);
  Ethernet.begin(mac);
  Serial.begin(9600);
  //pin setup
  pinMode(sensorPin, INPUT);
  pinMode(ledPin, OUTPUT);
 
  Serial.println("Starting single datastream upload to Xively...");
  Serial.println();
 
  // attempt to connect to Wifi network:
//  while ( status != WL_CONNECTED) {
//    Serial.print("Attempting to connect to SSID: ");
//    Serial.println(ssid);
//    status = WiFi.begin(ssid, keyIndex, pass);
//    // wait 10 seconds for connection:
//    delay(10000);
//  }
  Serial.println("Connected to Ethernet");
//printWifiStatus();
}
 
void loop() {
  //adjust LED level. set from Xively
  int getReturn = xivelyclient.get(feed, xivelyKey);    //get data from xively
  if(getReturn > 0){
    Serial.println("LED Datastream");
    Serial.println(feed[1]);
  }else Serial.println("HTTP Error");
 
  //write value to LED - change brightness
  int level = feed[1].getFloat();
  if(level < 0){
    level = 0;
  }else if(level > 255){
    level = 255;
  }
  //actually write the value
  digitalWrite(ledPin, level);
 
///////////////////////////////////////////////////////
  //read sensor values
  int sensorValue = analogRead(sensorPin);
  datastreams[0].setFloat(sensorValue);
 
  //print the sensor valye
  Serial.print("Read sensor value ");
  Serial.println(datastreams[0].getFloat());
 
  //send value to xively
  Serial.println("Uploading it to Xively");
  int ret = xivelyclient.put(feed, xivelyKey);
  //return message
  Serial.print("xivelyclient.put returned ");
  Serial.println(ret);
  Serial.println("");
 
  //delay between calls
  delay(15000);
}

