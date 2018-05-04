#include <SPI.h>
#include <math.h>
const byte slaveSelectPin = 13;
const float dac_low = -6.63, dac_high=6.64;
const byte bufsize=10;
char buf[bufsize];

void setup() {
  Serial.begin(115200);
  pinMode(slaveSelectPin, OUTPUT);
  digitalWrite(slaveSelectPin,HIGH);
  SPI.begin();
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE1));
  delay(100);
  dac_send(map_dac(0));
}

void loop() {
 if (Serial.available() > 0) {
    Serial.readBytesUntil('\n', buf, bufsize);
    String text=buf;
    unsigned int value=0;
    Serial.print("data sent:");
    if(text.indexOf('v')>-1){
      float data=text.toFloat();
      value=map_dac(data);
      Serial.print(data);
      Serial.print("||");
    }
    else{
      value=text.toInt();
    }
    dac_send(value);    
    Serial.println(value);
    for(byte i=0;i<bufsize;i++)
      buf[i]='\0';
  } 
}
void dac_send(unsigned int data){//send 12 bytes to dac
  digitalWrite(slaveSelectPin,LOW);  
  SPI.transfer16(data<<2);  
  digitalWrite(slaveSelectPin,HIGH);  
}

unsigned int map_dac(float x)
{
  return (unsigned int)round(((x - dac_low) * 4095 / (dac_high-dac_low)));
}
