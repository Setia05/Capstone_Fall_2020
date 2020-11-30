#include "ThingSpeak.h" //Official library for Thingspeak
#include <WiFi.h>// Library to connect to WiFi

char ssid[] = "Pixel";   // your network SSID (name)
char pass[] = "elcamino";   // your network password
WiFiClient  client;

unsigned long myChannelNumber = 1245528;
const char * myWriteAPIKey = "4W4LWYYCZBFN22DB";
//Thingspeak credential

int slots = -1; //Variable to store number of slots coming from beaglebone

void setup() {
  Serial.begin(9600);  //Initialize serial

  WiFi.mode(WIFI_STA);  
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {

  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){ //If Wifi not connected then reconnect
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass); 
      Serial.print(".");
      delay(5000);    
    }
    Serial.println("\nConnected.");
  }

  if(Serial.available()) // If any data coming from Serial - Beaglebone
  {
    slots = Serial.parseInt(); //Read the data as an Integer
  }

if(slots != -1)
{
  //Serial.print("Uploading Data: "); 
  //Serial.println(slots); //Print Statements
 
  // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
  // pieces of information in a channel.  Here, we write to field 1.
  int x = ThingSpeak.writeField(myChannelNumber, 1, slots, myWriteAPIKey); //Upload the data on Thingspeak and store status in x
  if(x == 200){ //If x is 200 means data got uploaded successfully
    ;
    //Serial.println("Channel update successful.");
  }
  else{ //Print if any error status code
   
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
 

 
  delay(20000); // Wait 20 seconds to update the channel again
}
delay(100); //Small delay
slots = -1; //make value as -1 again so that it is ready to take new values from Beaglebone
}
