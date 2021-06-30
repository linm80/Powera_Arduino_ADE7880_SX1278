#include <SPI.h>

#include "ADE7880.h"

#include <math.h>

SPIClass SPI_2(2);

ADE7880::ADE7880(int _CS)
{
  CS = _CS;
}

void ADE7880::begin()
{
  pinMode(CS, OUTPUT);
  digitalWrite(CS, HIGH);

  SPI_2.setDataMode(SPI_MODE2);
  SPI_2.setClockDivider(SPI_CLOCK_DIV32);
  SPI_2.setBitOrder(MSBFIRST);
  // SPI_2.setModule(2);
  SPI_2.begin();
  delay(10);
  enbaleSPI();
  write8(WTHR, 0x03);
  write8(VARTHR, 0x03);
  write8(VATHR, 0x03);
  write24(VLEVEL, 0x38000);
  write16(CF1DEN, CF1DENFACTOR);
  write8(0xE7FE, 0xAD);
  write8(0xE7E3, 0x80);
  write8(CONFIG2, 0x00);
  write16(CFMODE, 0x00A0);

  write16(RUNREG, 0x0001);

  // write16(LINECYC, 0x64);
  enable();
}

void ADE7880::enbaleSPI()
{
  write8(0xEBFF, 0x00);
  write8(0xEBFF, 0x00);
  write8(0xEBFF, 0x00);
}

void ADE7880::enable()
{
  digitalWrite(CS, LOW);
  //Serial.println("en");
}

void ADE7880::disable()
{
  digitalWrite(CS, HIGH);
  //Serial.println("dis");
}

unsigned char ADE7880::read8bits(unsigned int reg)
{

  unsigned char result;
  char regMSB, regLSB;
  regLSB = (unsigned char)(reg & 0xFF);
  regMSB = (unsigned char)(reg >> 8);
  enable();
  SPI_2.transfer(0x01);
  SPI_2.transfer(regMSB);
  SPI_2.transfer(regLSB);
  result = SPI_2.transfer(0x00);
  disable();
  delayMicroseconds(50);
  return (unsigned char)result;
}
unsigned long ADE7880::read32bits(unsigned int reg)
{

  unsigned long result;
  unsigned char resultLeastByte, resultSecondByte, resultThirdByte, resultMostByte;
  char regMSB, regLSB;
  regLSB = (unsigned char)(reg & 0xFF);
  regMSB = (unsigned char)(reg >> 8);
  enable();
  SPI_2.transfer(0x01);
  SPI_2.transfer(regMSB);
  SPI_2.transfer(regLSB);
  resultMostByte = SPI_2.transfer(0x00);
  resultThirdByte = SPI_2.transfer(0x00);
  resultSecondByte = SPI_2.transfer(0x00);
  resultLeastByte = SPI_2.transfer(0x00);
  disable();
  delayMicroseconds(50);
  result = (unsigned long)resultMostByte << 24 | (unsigned long)resultThirdByte << 16 | (unsigned long)resultSecondByte << 8 | (unsigned long)resultLeastByte;
  return result;
}
unsigned int ADE7880::read16bits(unsigned int reg)
{

  unsigned int result;
  unsigned char resultLeastByte, resultMostByte;
  char regMSB, regLSB;
  regLSB = (unsigned char)(reg & 0xFF);
  regMSB = (unsigned char)(reg >> 8);
  enable();
  SPI_2.transfer(0x01);
  SPI_2.transfer(regMSB);
  SPI_2.transfer(regLSB);
  resultMostByte = SPI_2.transfer(0x00);
  resultLeastByte = SPI_2.transfer(0x00);
  disable();
  delayMicroseconds(50);
  result = (unsigned int)resultMostByte << 8 | (unsigned int)resultLeastByte;
  return result;
}
unsigned long ADE7880::read24bits(unsigned int reg)
{

  unsigned long result;
  unsigned char resultLeastByte, resultMiddleByte, resultMostByte;
  char regMSB, regLSB;
  regLSB = (unsigned char)(reg & 0xFF);
  regMSB = (unsigned char)(reg >> 8);
  enable();
  SPI_2.transfer(0x01);
  SPI_2.transfer(regMSB);
  SPI_2.transfer(regLSB);
  resultMostByte = SPI_2.transfer(0x00);
  resultMiddleByte = SPI_2.transfer(0x00);
  resultLeastByte = SPI_2.transfer(0x00);
  disable();
  delayMicroseconds(50);
  result = (unsigned long)resultMostByte << 16 | (unsigned long)resultMiddleByte << 8 | (unsigned long)resultLeastByte;
  return result;
}
void ADE7880::write8(unsigned int reg, unsigned char data)
{
  char regMSB, regLSB;
  regLSB = (unsigned char)(reg & 0xFF);
  regMSB = (unsigned char)(reg >> 8);
  enable();
  SPI_2.transfer(0x00);
  SPI_2.transfer(regMSB);
  SPI_2.transfer(regLSB);
  SPI_2.transfer(data);
  disable();
  delayMicroseconds(50);
}
void ADE7880::write16(unsigned int reg, unsigned int data)
{
  char dataMSB, dataLSB;
  char regMSB, regLSB;
  regLSB = (unsigned char)(reg & 0xFF);
  regMSB = (unsigned char)(reg >> 8);
  dataLSB = (unsigned char)(data & 0xFF);
  dataMSB = (unsigned char)(data >> 8);
  enable();
  SPI_2.transfer(0x00);
  SPI_2.transfer(regMSB);
  SPI_2.transfer(regLSB);
  SPI_2.transfer(dataMSB);
  SPI_2.transfer(dataLSB);
  disable();
  delayMicroseconds(50);
}
void ADE7880::write24(unsigned int reg, unsigned long data)
{
  char dataMSB, dataMiddleB, dataLSB;
  char regMSB, regLSB;
  regLSB = (unsigned char)(reg & 0xFF);
  regMSB = (unsigned char)(reg >> 8);

  dataLSB = (unsigned char)(data & 0xFF);
  dataMiddleB = (unsigned char)(data >> 8);
  dataMSB = (unsigned char)(data >> 16);
  enable();
  SPI_2.transfer(0x00);
  SPI_2.transfer(regMSB);
  SPI_2.transfer(regLSB);
  SPI_2.transfer(dataMSB);
  SPI_2.transfer(dataMiddleB);
  SPI_2.transfer(dataLSB);
  disable();
  delayMicroseconds(50);
}
void ADE7880::write32(unsigned int reg, unsigned long data)
{

  char dataMSB, dataMMiddleB, dataLMiddleB, dataLSB;
  char regMSB, regLSB;
  regLSB = (unsigned char)(reg & 0xFF);
  regMSB = (unsigned char)(reg >> 8);

  dataLSB = (unsigned char)(data & 0xFF);
  dataLMiddleB = (unsigned char)(data >> 8);
  dataMMiddleB = (unsigned char)(data >> 16);
  dataMSB = (unsigned char)(data >> 24);
  enable();
  delayMicroseconds(50);
  SPI_2.transfer(0x00);
  SPI_2.transfer(regMSB);
  SPI_2.transfer(regLSB);
  SPI_2.transfer(dataMSB);
  SPI_2.transfer(dataMMiddleB);
  SPI_2.transfer(dataLMiddleB);
  SPI_2.transfer(dataLSB);
  disable();
  delayMicroseconds(50);
}
