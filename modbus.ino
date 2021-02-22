#include<ModbusMaster.h>
#include<SoftwareSerial.h>
#include <Wire.h>

SoftwareSerial mySerial(3,4); //Rx: Pin 3, Tx: Pin 4

uint16_t m_startAddress[12] = {0x0000 , 0x0006, 0x000C, 0x001E, 0x0012, 0x0018, 0x0046, 0x0034, 0x003E, 0x0102, 0x00E0, 0x0156};
uint16_t m_length = 0x00001;
uint8_t result;
uint8_t ret;
int sta = 0;
int DE = 7;
union
{
  uint16_t u[2];
  float f;
}
meterdata;

/////////////////////////////// Data Send to Cognisense //////////////////////////////////

char outBuffer[30];

String volt = String(240);
String current = String(2.5);
String power = String(120);
String powerFactor = String(0.9);
String frequency = String(50);
String output = volt + "," + current + "," + power + "," + powerFactor + "," + frequency;

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

    Wire.write("240, 2.5, 120, 0.9, 50");
    Serial.println(output);
    
}

void setup()
{
  pinMode(DE, OUTPUT);
  digitalWrite(DE, HIGH);
  node.preTransmission(enableTx);
  node.postTransmission(disableTx);
  Serial.begin(9600);
  node.begin(111, mySerial);
  Serial.println("CS I2C slaves Testing");
  
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

  while(Wire.available()){
    
  result = node.readInputRegisters(m_startAddress[sta], m_length);
  Serial.println(result);
  if (result == node.ku8MBSuccess)
  {
      if (sta == 0)
        {
        meterdata.u[1] = node.getResponseBuffer(0);
        meterdata.u[0] = node.getResponseBuffer(1);
        Serial.print("Voltage: ");
        Serial.print(meterdata.f);
        Serial.println("V");
       // Serial.println(meterdata.u[1]);
        }
      
      else if (sta == 1)
        {
        meterdata.u[1] = node.getResponseBuffer(0);
        meterdata.u[0] = node.getResponseBuffer(1);
        Serial.print("Current: ");
        Serial.print(meterdata.f);
        Serial.println("A");
        }
      else if (sta == 2) 
       {
        meterdata.u[1] = node.getResponseBuffer(0);
        meterdata.u[0] = node.getResponseBuffer(1);
        Serial.print("Power: ");
        Serial.print(meterdata.f);
        Serial.println("W");
       }
      else if (sta == 3) 
       {
        meterdata.u[1] = node.getResponseBuffer(0);
        meterdata.u[0] = node.getResponseBuffer(1);
        Serial.print("Power Factor: ");
        Serial.print(meterdata.f);
        Serial.println("");
       }
       else if (sta == 4) 
       {
        meterdata.u[1] = node.getResponseBuffer(0);
        meterdata.u[0] = node.getResponseBuffer(1);
        Serial.print("Apparent Power: ");
        Serial.print(meterdata.f);
        Serial.println("VA");
       }
       else if (sta == 5) 
       {
        meterdata.u[1] = node.getResponseBuffer(0);
        meterdata.u[0] = node.getResponseBuffer(1);
        Serial.print("Reactive Power: ");
        Serial.print(meterdata.f);
        Serial.println("VAR");
       }
       else if (sta == 6) 
       {
        meterdata.u[1] = node.getResponseBuffer(0);
        meterdata.u[0] = node.getResponseBuffer(1);
        Serial.print("Frequency of supply voltage: ");
        Serial.print(meterdata.f);
        Serial.println("Hz");
       }
       else if (sta == 7) 
       {
        meterdata.u[1] = node.getResponseBuffer(0);
        meterdata.u[0] = node.getResponseBuffer(1);
        Serial.print("Total system power: ");
        Serial.print(meterdata.f);
        Serial.println("W");
       }
       else if (sta == 8)
       {
        meterdata.u[1] = node.getResponseBuffer(0);
        meterdata.u[0] = node.getResponseBuffer(1);
        Serial.print("Total system Power Factor: ");
        Serial.print(meterdata.f);
        Serial.println("");
       }
       else if (sta == 9)
       {
        meterdata.u[1] = node.getResponseBuffer(0);
        meterdata.u[0] = node.getResponseBuffer(1);
        Serial.print("Current Demand: ");
        Serial.print(meterdata.f);
        Serial.println("A");
       }
       else if (sta == 10)
       {
        meterdata.u[1] = node.getResponseBuffer(0);
        meterdata.u[0] = node.getResponseBuffer(1);
        Serial.print("Neutral Current: ");
        Serial.print(meterdata.f);
        Serial.println("A");
       }
       else if (sta == 11)
       {
        meterdata.u[1] = node.getResponseBuffer(0);
        meterdata.u[0] = node.getResponseBuffer(1);
        Serial.print("Total kWh: ");
        Serial.print(meterdata.f);
        Serial.println("kWh");
       }
  }

      else 
      {
      Serial.print("ERROR");
     // Serial.println(result, HEX);
      }

  sta++;
  if (sta == 12) sta = 0;
  delay(1000);
}
}
