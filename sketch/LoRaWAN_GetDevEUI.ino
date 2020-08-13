/* Simple getDevEUI
   This example code is in the public domain.
*/

#include "LoRaWAN.h"

char buffer[32];

void setup(void)
{
  Serial.begin(9600);
  while(!Serial){}
  delay(30000);
  
  LoRaWAN.begin(AS923);
  Serial.println("devEUI request");
  LoRaWAN.getDevEui(buffer, 18);
  Serial.println(buffer);
}

void loop(void){
}
