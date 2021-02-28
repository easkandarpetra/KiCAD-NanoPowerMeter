#include<ModbusMaster.h>
#include<SoftwareSerial.h>
#include <Wire.h>

SoftwareSerial mySerial(3,4); //Rx: Pin 3, Tx: Pin 4

uint16_t m_startAddress[10] = {0x0000 , 0x0006, 0x000C, 0x001E, 0x0012, 0x0018, 0x0046, 0x0156};
uint16_t m_length = 0x00001;
uint8_t result;
uint8_t ret;
int sta = 0;
int DE = 8;
union
{
  uint16_t u[2];
  float f;
}
meterdata;

/////////////////////////////// Data Send to Cognisense //////////////////////////////////

char outBuffer[30];

String volt = String(meterdata.f);
String current = String(meterdata.f);
String power = String(meterdata.f);
String powerFactor = String(meterdata.f);
String frequency = String(meterdata.f);
String apparentPower = String(meterdata.f);
String reactivePower = String(meterdata.f);
String totalKwh = String(meterdata.f);


ModbusMaster node;

void enableTx()  //TX
{
  delay(2);  
  digitalWrite(DE, HIGH);
}

void disableTx()  //RX
{
  delay(2);
  digitalWrite(DE, LOW);
}

void requestEvent() {

 // Put the data inside Wire.write()
 
    Wire.write("230, 0.1, 12, 0.5, 22");
    
}

void setup()
{
  pinMode(DE, OUTPUT);
  digitalWrite(DE, HIGH);
  node.preTransmission(enableTx);
  node.postTransmission(disableTx);
  Serial.begin(9600);
  mySerial.begin(9600);
  node.begin(111, mySerial);
  Serial.println("CS I2C slaves Testing...");
  delay(2000);
  
  // I2C Slave address
  Wire.begin( 0x5B );
  Wire.onRequest(requestEvent);
  

}

void loop()
{
  Wire.beginTransmission(0x5B);
  //Wire.send(0);
  Wire.requestFrom(requestEvent, 1);
  Wire.endTransmission();


    
  result = node.readInputRegisters(m_startAddress[sta], m_length);
  if (result == node.ku8MBSuccess)
  {
      if (sta == 0)
        {
        meterdata.u[1] = node.getResponseBuffer(0);
        meterdata.u[0] = node.getResponseBuffer(1);
        volt = String(meterdata.f);
        Serial.print(volt + ",");
        }
      else if (sta == 1)
        {
        meterdata.u[1] = node.getResponseBuffer(0);
        meterdata.u[0] = node.getResponseBuffer(1);
        current = String(meterdata.f);
        Serial.print(current + ",");
        }
      else if (sta == 2) 
       {
        meterdata.u[1] = node.getResponseBuffer(0);
        meterdata.u[0] = node.getResponseBuffer(1);
       power = String(meterdata.f);
       Serial.print(power + ",");
       }
      else if (sta == 3) 
       {
        meterdata.u[1] = node.getResponseBuffer(0);
        meterdata.u[0] = node.getResponseBuffer(1);
        powerFactor = String(meterdata.f);
        Serial.print(powerFactor + ",");
       }
       else if (sta == 4) 
       {
        meterdata.u[1] = node.getResponseBuffer(0);
        meterdata.u[0] = node.getResponseBuffer(1);
        apparentPower = String(meterdata.f);
        Serial.print(apparentPower + ",");
       }
       else if (sta == 5) 
       {
        meterdata.u[1] = node.getResponseBuffer(0);
        meterdata.u[0] = node.getResponseBuffer(1);
        reactivePower = String(meterdata.f);
        Serial.print(reactivePower + ",");
       }
       else if (sta == 6) 
       {
        meterdata.u[1] = node.getResponseBuffer(0);
        meterdata.u[0] = node.getResponseBuffer(1);
        frequency = String(meterdata.f);
        Serial.print(frequency + ",");
        
       }
       else if (sta == 7)
       {
        meterdata.u[1] = node.getResponseBuffer(0);
        meterdata.u[0] = node.getResponseBuffer(1);
        totalKwh = String(meterdata.f);
        Serial.print(totalKwh);
        Serial.println("");
       } 
  }

      else 
      {
      Serial.print("ERROR");
     
      }

  sta++;
  if (sta == 8) sta = 0;
  delay(1000);

}
