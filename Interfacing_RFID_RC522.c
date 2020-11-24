
#include <SPI.h> // SPI Library
#include <MFRC522.h> // Library for RC522 module

#define RST_PIN         22          // Configurable, see typical pin layout above
#define SS_1_PIN        21         // SDA Pin for Reader 1
#define SS_2_PIN        15          //  SDA Pin for Reader 2

#define NR_OF_READERS   1 // Total number of Readers

byte ssPins[] = {SS_1_PIN, SS_2_PIN};// Storing the SDA pins of both reader so that we can call them 1 by 1

MFRC522 mfrc522[NR_OF_READERS];   // Create MFRC522 instance.

void setup() {

  Serial.begin(9600); // Initialize serial communications with the PC
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)

  SPI.begin();        // Init SPI bus

  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) // Loop to initialize all RFID readers 1 by 1
  {
    mfrc522[reader].PCD_Init(ssPins[reader], RST_PIN); // Init each MFRC522 card
    Serial.print(F("Reader "));
    Serial.print(reader); //Print Reader number
    Serial.print(F(": "));
    mfrc522[reader].PCD_DumpVersionToSerial()//Printing Firmware version of RFID reader
  }
}


void loop() 
{

  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) //Loop to check readers 1 by 1
  {
    // Look for new cards

    if (mfrc522[reader].PICC_IsNewCardPresent() && mfrc522[reader].PICC_ReadCardSerial()) //If card is Present and Data is coming
    {
      Serial.print(F("Reader "));
      Serial.print(reader); //Print Reader number
      // Show some details of the PICC (that is: the tag/card)
      Serial.print(F(": Card UID:"));
      dump_byte_array(mfrc522[reader].uid.uidByte, mfrc522[reader].uid.size); //Print Unique ID code of Card
      Serial.println();
      Serial.print(F("PICC type: "));
      MFRC522::PICC_Type piccType = mfrc522[reader].PICC_GetType(mfrc522[reader].uid.sak); //Get the Type of Card Sacnned
      Serial.println(mfrc522[reader].PICC_GetTypeName(piccType)); //Print the Type of Card Scanned

      // Halt PICC
      mfrc522[reader].PICC_HaltA(); //Pause the reader 
      // Stop encryption on PCD
      mfrc522[reader].PCD_StopCrypto1();
    } //if (mfrc522[reader].PICC_IsNewC
  } //for(uint8_t reader
}


void dump_byte_array(byte *buffer, byte bufferSize) //Function to print the incoming bytes
{
  for (byte i = 0; i < bufferSize; i++) 
  {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}
