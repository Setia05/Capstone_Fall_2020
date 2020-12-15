
#include <SPI.h>
#include <MFRC522.h>

#include "ThingSpeak.h"
#include <WiFi.h>

#define DEBUG 0

char ssid[] = "	qwert";   // your network SSID (name) 
char pass[] = "123456789";   // your network password
WiFiClient  client;

unsigned long myChannelNumber = 123456;
const char * myWriteAPIKey = "zxcvbnm";

long oldmillis;

int slots = -1;


#define RST_PIN         22          // Configurable, see typical pin layout above
#define SS_1_PIN        21         // Configurable, take a unused pin, only HIGH/LOW required, must be different to SS 2
#define SS_2_PIN        5          // Configurable, take a unused pin, only HIGH/LOW required, must be different to SS 1

#define NR_OF_READERS   2

byte ssPins[] = {SS_1_PIN, SS_2_PIN};

MFRC522 mfrc522[NR_OF_READERS];   // Create MFRC522 instance.

String CardValue;
int ReaderNum;


void setup() {

  Serial.begin(9600); // Initialize serial communications with the PC
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)

  SPI.begin();        // Init SPI bus

  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++)
  {
    mfrc522[reader].PCD_Init(ssPins[reader], RST_PIN); // Init each MFRC522 card
    if(DEBUG) Serial.print(F("Reader "));
    if(DEBUG) Serial.print(reader);
    if(DEBUG) Serial.print(F(": "));
    if(DEBUG) mfrc522[reader].PCD_DumpVersionToSerial();
  }

  
  WiFi.mode(WIFI_STA);  // 
  ThingSpeak.begin(client);  // Initialize ThingSpeak

  oldmillis = millis();
  
}


void loop() 
{



  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) 
  {
    // Look for new cards

    if (mfrc522[reader].PICC_IsNewCardPresent() && mfrc522[reader].PICC_ReadCardSerial()) 
    {
//      Serial.print(F("Reader "));
//      Serial.print(reader);

      ReaderNum = reader;
      // Show some details of the PICC (that is: the tag/card)
//      Serial.print(F(": Card UID:"));
      dump_byte_array(mfrc522[reader].uid.uidByte, mfrc522[reader].uid.size);
      Serial.println();
//      Serial.print(F("PICC type: "));
      MFRC522::PICC_Type piccType = mfrc522[reader].PICC_GetType(mfrc522[reader].uid.sak);
//      Serial.println(mfrc522[reader].PICC_GetTypeName(piccType));

      // Halt PICC
      mfrc522[reader].PICC_HaltA();
      // Stop encryption on PCD
      mfrc522[reader].PCD_StopCrypto1();
    }
 
  } 


    
  if(Serial.available()) // If any data is coming from Beaglebone
  {
    slots = Serial.parseInt();
    Serial.println(slots);
    
  }


  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }
  

if(slots != -1 and (millis()-oldmillis)>16000) // Uploading to Thingspeak
{



   oldmillis = millis();

  Serial.print("Uploading Data: ");
  Serial.println(slots);
  
  // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
  // pieces of information in a channel.  Here, we write to field 1.
  int x = ThingSpeak.writeField(myChannelNumber, 1, slots, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }

  slots = -1;
 }
delay(10);
 

  
}


void dump_byte_array(byte *buffer, byte bufferSize) 
{
  for (byte i = 0; i < bufferSize; i++) 
  {

    CardValue += (String)buffer[i];
  }
  //Serial.print("Reader: ");
  if(ReaderNum == 0)   Serial.print("A");
  if(ReaderNum == 1)   Serial.print("B");
  //Serial.print(" | Card: ");
  Serial.println(CardValue);
  CardValue = "\0";
}



