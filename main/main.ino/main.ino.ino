#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>
#include <Servo.h>


#include <OneWire.h>

#define PH_sensor A0 
#define Level_Senser A1
#define ArrayLength 40
#define Offset 0.00

#define REQ_BUF_SZ 60

int relay1 = 2; //digital I/O 2 _ 릴레이1 핀설정 -> 배수
int relay2 = 3; //digital I/O 3 _ 릴레이2 핀설정 -> 급수
int relay3 = 4; //digital I/O 4 _ 릴레이3 핀설정 -> 히터
int relay4 = 5; //digital I/O 5 _ 릴레이4 핀설정 -> 조명
int relay5 = 6; //digital I/O 6 _ 릴레이5 핀설정 -> PH
int relay6 = 7; //digital I/O 7 _ 릴레이6 핀설정 -> 먹이급여기
int Temp_SensingPin = 8; //digital I/O 8 _ 온도 센서
int servoPin = 9;
int angle = 0;
int pHArray[40];
int pHarrayIndex=0;

boolean feeding_flag = false;
boolean ph_flag = false;
boolean heater_flag = false;
boolean light_flag = false;
boolean supply_flag = false;
boolean drain_flag = false;




//float now_temp = getTemp(); 

float receive_temp; 
float pHValue, voltage;

OneWire ds(Temp_SensingPin);
Servo servo;

byte mac[] = { 0x00,0xAA,0xBB,0xCC,0xDE,0x02 };  
IPAddress ip(192,168, 0, 177);
EthernetServer server(80); 
File webFile;
char HTTP_req[REQ_BUF_SZ]={0};
char req_index = 0;


void setup()
{  

    
    pinMode (relay1, OUTPUT); 
    pinMode (relay2, OUTPUT);
    pinMode (relay3, OUTPUT); 
    pinMode (relay4, OUTPUT); 
    pinMode (relay5, OUTPUT);
    pinMode (relay6, OUTPUT);
    pinMode (Temp_SensingPin, OUTPUT);

    digitalWrite (relay1, HIGH);
    digitalWrite (relay2, HIGH);
    digitalWrite (relay3, HIGH);
    digitalWrite (relay4, HIGH);
    digitalWrite (relay5, HIGH);
    digitalWrite (relay6, HIGH);

    servo.attach(servoPin);
   
    Serial.begin(9600);      
    
    Serial.println(F("Initializing SD card..."));
    if (!SD.begin(4)) {
        Serial.println(F("ERROR - SD card initialization failed!"));
        return;    
    }
    Serial.println(F("SUCCESS - SD card initialized."));
    if (!SD.exists("index.htm")) { 
        Serial.println(F("ERROR - Can't find index.htm file!"));
        return;  
    }
    Serial.println(F("SUCCESS - Found index.htm file."));

    //Serial.print(Ethernet.localIP());
    
    Ethernet.begin(mac, ip);  
   

    server.begin();     

}

void loop()
{

    EthernetClient client = server.available(); // try to get client

    if (client) {  
        boolean currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) {   
                char c = client.read(); 

                if(req_index < (REQ_BUF_SZ -1)){
                  HTTP_req[req_index] = c;
                  req_index++;
                }
                
                if (c == '\n' && currentLineIsBlank)
                {
                    client.println(F("HTTP/1.1 200 OK"));
                    if(StrContains(HTTP_req, "ajax_inputs"))
                    {
                      client.println(F("Content-Type: text/html"));
                      client.println(F("Connection: keep-alive"));
                      client.println();
                      
                      setModules();
                      
                      XML_response(client);
                    }
                    else 
                    {
                      client.println(F("Content-Type: text/html"));
                      client.println(F("Connection: keep-alive"));
                      client.println();
                      webFile = SD.open("index.htm");       
                      if (webFile) 
                      {
                        while(webFile.available()) {
                            client.write(webFile.read()); 
                        }
                        webFile.close();
                      }
                    }
                    Serial.print(HTTP_req);
                    req_index = 0;
                    StrClear(HTTP_req, REQ_BUF_SZ);
                    break;
                }
                if (c == '\n') {
                    currentLineIsBlank = true;
                } 
                else if (c != '\r') { 
                    currentLineIsBlank = false;
                }
            } //end if (client.available())
        } //end while (client.connected())
        delay(1);      
        client.stop(); 
    }
//----------------------------------------------------------

    pHArray[pHarrayIndex++]=analogRead(PH_sensor);
    if(pHarrayIndex==ArrayLength){
      pHarrayIndex=0;
    }
    //voltage = avergearray(pHArray, ArrayLength)*5.0/1024;
    pHValue=3.5 * voltage + Offset;
}

float getTemp(){

  byte data[12];
  byte addr[8];
  if ( !ds.search(addr)) {
      //no more sensors on chain, reset search
      ds.reset_search();
      return -1000;
  }
  if ( OneWire::crc8( addr, 7) != addr[7]) {
      Serial.println(F("CRC is not valid!"));
      return -1000;
  }
  if ( addr[0] != 0x10 && addr[0] != 0x28) {
      Serial.print(F("Device is not recognized"));
      return -1000;
  }
  ds.reset();
  ds.select(addr);
  ds.write(0x44,1);
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
  
  
  return TemperatureSum; 
}

void setHeater(void)
{
  if(StrContains(HTTP_req, "heater_on=1")){
    heater_flag=1;
    digitalWrite(relay3, LOW);
  }
}
void setModules(void)
{
  if(StrContains(HTTP_req, "light=1")){
    light_flag=1;
    digitalWrite(relay4, LOW);
  }
  else if(StrContains(HTTP_req, "light=0")){
    light_flag=0;
    digitalWrite(relay4, HIGH);
  }
//-------------light module-----------------

  if(StrContains(HTTP_req, "supply_water=1")){
    supply_flag=1;
    digitalWrite(relay2, LOW);
  }
  else if(StrContains(HTTP_req, "supply_water=0")){
    supply_flag=0;
    digitalWrite(relay2, HIGH);
  }

  //--------------Supply_water module--------------

  if(StrContains(HTTP_req, "drain_water=1")){
    drain_flag=1;
    digitalWrite(relay1, LOW);
    
  }
  else if(StrContains(HTTP_req, "drain_water=0")){
    drain_flag=0;
    digitalWrite(relay1, HIGH);
  }

  //--------------Drain_water module----------------
  
  
  if(StrContains(HTTP_req, "heater_on=1")){
    heater_flag=1;
    digitalWrite(relay3, LOW);
    Serial.print("heater!");
  }
  else if(StrContains(HTTP_req, "heater_on=0")){
    heater_flag=0;
    digitalWrite(relay3, HIGH);
    Serial.print("heater!");
  }
  //-------------heater module---------------- 

  if(StrContains(HTTP_req, "PH_down=1")){
    if(pHValue<=6.1){
   
    }
    else {
      ph_flag=1;
      digitalWrite(relay5, LOW);
    }
  }
  // --------------PH module-------------------

  if(StrContains(HTTP_req, "feeding=1")){
    for(angle=0; angle < 90; angle++)
    {
      servo.write(angle);
      delay(15); 
    }
  }

  // -------------feeding module------------------
}



void XML_response(EthernetClient cl)
{
  int analog_val;
  int count;

  cl.print("<?xml version = \"1.0\" ?>");
  cl.print("<inputs>");

  //--------------- light module
  cl.print("<light>");
  if(light_flag){
    cl.print("checked");
  }
  else{
    cl.print("unchecked");
  }
  cl.print("</light>");

  //-----------------supply_water module
  cl.print("<supply_water>");
  if(supply_flag){
    cl.print("checked");
  }
  else{
    cl.print("unchecked");
  }

  cl.print("</supply_water>");

  //------------------drain_water module
  cl.print("<drain_water>");
  if(drain_flag){
    cl.print("checked");
  }
  else{
    cl.print("unchecked");
  }
  cl.print("</drain_water>");

  //------------------feeding module
  cl.print("<feeding>");
  if(feeding_flag){
    cl.print("on");
  }
  else{
    cl.print("off");
  }
  cl.print("</feeding>");

  //-----------------heater module
  cl.print("<heater>");
  if(heater_flag){
    cl.print("on");
  }
  else{
    cl.print("off");
  }
  cl.print("</heater>");

  //-----------------PH module
  cl.print("<PH>");
  if(ph_flag){
    cl.print("on");
  }
  else{
    cl.print("off");
  }
  cl.print("</PH>");
  cl.print("</inputs>");
  
}

void StrClear(char *str, char length)
{
  for(int i=0; i<length; i++){
    str[i] = 0;
  }
}

char StrContains(char *str, const char *sfind)
{
  char found = 0;
  char index = 0;
  char len;

  len = strlen(str);

  if(strlen(sfind)>len){
    return 0;
  }

  while(index<len){
    if(str[index] == sfind[found]){
      found++;
      if(strlen(sfind) == found){
        return 1;
      }
    }
    else{
      found = 0;
    }
    index++;
  }

  return 0;
}



