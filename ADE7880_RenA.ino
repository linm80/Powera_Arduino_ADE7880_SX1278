#include "ADE7880.h"
#include <LoRa_STM32.h>

int counter = 0;

ADE7880 myADE(PB12);

void setup()
{
  Serial.begin(115200);
  // while (!Serial)
  //   ;

  Serial.println("init ADE7880");
  myADE.begin();
  Serial.println("end of init");

  Serial.println("init Sx1278");
  if (!LoRa.begin(433E6))
  {
    Serial.println("Starting LoRa failed!");
    while (1)
      ;
  }
  Serial.println("end of init");
}

void loop()
{
  adePrintValues();
  // loraSend();
  // loraRecieve();
}

void loraSend()
{
  Serial.print("Sending packet: h ");
  Serial.println(counter);
  LoRa.beginPacket();

  LoRa.print(counter);
  LoRa.endPacket();
  counter++;
  delay(400);
}
void loraRecieve()
{
  int packetSize = LoRa.parsePacket();
  if (packetSize)
  {
    Serial.print("Received packet '");
    while (LoRa.available())
    {
      Serial.print((char)LoRa.read());
    }
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
}
void adePrintValues()
{
  Serial.print("AIRMS`: ");
  Serial.print(myADE.read32bits(AIRMS) * ILSB);
  Serial.print("  -  ");

  Serial.print("AVRMS`: ");
  Serial.print(myADE.read32bits(AVRMS) * VLSB);
  Serial.print("  -  ");

  Serial.print("PF`: ");
  Serial.print(myADE.read16bits(APF) * PFLSB);
  Serial.print("  -  ");

  Serial.print("AWATT`: ");
  Serial.print((myADE.read24bits(AWATT)));
  Serial.println("    ");

  delay(100);
}
