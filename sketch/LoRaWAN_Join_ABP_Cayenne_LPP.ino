/*  Simple ABP join for a TTN Japan with Cayenne LPP Format
 *  
 *  In setup() below please replace the argument to LoRaWAN.begin()
 *  with your appropriate region specific band:
 *  
 *  AS923 AU915 EU868 IN865 KR920 US915
 *  
 *  AU915/US915 networks have 64+8 channels. Typical gateways support only
 *  8 (9) channels. Hence it's a good idea to pick the proper channel
 *  subset via select via LoRaWAN.setSubBand(),
 *  
 *  EU868/IN865 have duty cycle restrictions. For debugging it makes sense
 *  to disable those via setDutyCycle(false);
 *  
 *  For an external antenna one should set the proper antenna gain
 *  (default is 2.0) via setAntennaGain().
 *  
 *  Please edit the keys below as they are just debugging samples.
 *  
 *  This example code is in the public domain.
 */
 
#include "LoRaRadio.h"
#include "LoRaWAN.h"

#define RSSI_THRESHOLD -82
#define SENSE_TIME 6
#define onboardLED 13

const char *devAddr = "00000000";
const char *nwkSKey = "00000000000000000000000000000000";
const char *appSKey = "00000000000000000000000000000000";

void setup(void)
{
    pinMode(onboardLED, OUTPUT);
    digitalWrite(onboardLED, LOW);
    
    Serial.begin(9600);
    delay(1000);
    
    LoRaRadio.begin(923200000);
    LoRaRadio.setLnaBoost(true);
    LoRaWAN.begin(AS923);
    LoRaWAN.setADR(false);
    LoRaWAN.setDataRate(2);
    LoRaWAN.setTxPower(13.0f);
    
    LoRaWAN.joinABP(devAddr, nwkSKey, appSKey);
    if (Serial == 1) Serial.println("JOIN( )");
}

void loop(void)
{
    if (LoRaWAN.joined() && !LoRaWAN.busy())
    {
      if (Serial == 1)
      {
        Serial.print("TRANSMIT( ");
        Serial.print("TimeOnAir: ");
        Serial.print(LoRaWAN.getTimeOnAir());
        Serial.print(", NextTxTime: ");
        Serial.print(LoRaWAN.getNextTxTime());
        Serial.print(", MaxPayloadSize: ");
        Serial.print(LoRaWAN.getMaxPayloadSize());
        Serial.print(", DR: ");
        Serial.print(LoRaWAN.getDataRate());
        Serial.print(", TxPower: ");
        Serial.print(LoRaWAN.getTxPower(), 1);
        Serial.print("dbm, UpLinkCounter: ");
        Serial.print(LoRaWAN.getUpLinkCounter());
        Serial.print(", DownLinkCounter: ");
        Serial.print(LoRaWAN.getDownLinkCounter());
        Serial.println(" )");
      }
      
      // Carrier Sensing
      while (!LoRaRadio.sense(RSSI_THRESHOLD, SENSE_TIME)) {
        if (Serial == 1) Serial.println("RF Channel Busy: Waiting...");
        delay(50);
      }
      
      LoRaWAN.beginPacket();
      // Channel
      LoRaWAN.write(0x01); // 1byte Ch.1
      // Data Type
      LoRaWAN.write(0x88); // 1byte GPS
      // Location
      // Summit of Mt.Fuji WGS84
      LoRaWAN.write(0x05); // 3bytes LAT 35.3607
      LoRaWAN.write(0x65);
      LoRaWAN.write(0x47);
      LoRaWAN.write(0x15); // 3bytes LON 138.7274
      LoRaWAN.write(0x2B);
      LoRaWAN.write(0x0A);
      LoRaWAN.write(0x05); // 3bytes ALT 3772
      LoRaWAN.write(0xc1);
      LoRaWAN.write(0x70);
      LoRaWAN.endPacket();
    }
    
    digitalWrite(onboardLED, HIGH);
    delay(1000);
    digitalWrite(onboardLED, LOW);
    delay(10000);
}
