 
 
#include <OneWire.h> // library(라이브러리) 추가

const int Temp_SensingPin = 7; //Temperature Sensing Pin : 7(Digital I/O)
// Const int - 함수 지정 및 변동x(즉, 함수 고정)
// 7번 Pin으로 데이터를 받을 것이다. 


OneWire ds(Temp_SensingPin);   //Basic library - ds(7)
// 7번 Pin으로 데이터를 받을 것이다. 
// OneWire ds(Temp_SensingPin); = ds(7); 


void setup(void) {
  Serial.begin(9600);       

  
  pinMode(Temp_SensingPin, OUTPUT); //Pin, Only Sensing Read Setup

}
   
 
void loop(void) {
 
  float temperature = getTemp(); //getTemp() 에서 온도 값을 불러 옵니다. 
  Serial.println(temperature); //자체적으로 온도 프린트 해주기
    
  delay(1000); //딜레이
  
}


//*************** 핵심 함수 ***************//
//*****************************************//
float getTemp(){
  //returns the temperature from one DS18S20 in DEG Celsius
  byte data[12];
  byte addr[8];
  if ( !ds.search(addr)) {
      //no more sensors on chain, reset search
      ds.reset_search();
      return -1000;
  }
  if ( OneWire::crc8( addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return -1000;
  }
  if ( addr[0] != 0x10 && addr[0] != 0x28) {
      Serial.print("Device is not recognized");
      return -1000;
  }
  ds.reset();
  ds.select(addr);
  ds.write(0x44,1); // start conversion, with parasite power on at the end
  byte present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE); // Read Scratchpad
  
  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = ds.read();
  }

  ds.reset_search();
  
  byte MSB = data[1];
  byte LSB = data[0];
  float tempRead = ((MSB << 8) | LSB); //using two's compliment
  float TemperatureSum = tempRead / 16;
  if(TemperatureSum>20.0){
    
  }
  return TemperatureSum; //해당 값을 반환한다. - 즉 해당 값이 출력으로 나온다. 
}
//*************** 핵심 함수 ***************// 
//*****************************************//
