#include <Servo.h>

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



Servo servo; 

void setup() {
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

}

void loop() {
    digitalWrite (relay1, LOW);
    delay(1000);
    digitalWrite (relay1, HIGH);

    delay(1000);
    
    digitalWrite (relay2, LOW);
    delay(1000);
    digitalWrite (relay2, HIGH);

    delay(1000);
    
    digitalWrite (relay3, LOW);
    delay(1000);
    digitalWrite (relay3, HIGH);

    delay(1000);
    
    digitalWrite (relay4, LOW);
    delay(1000);
    digitalWrite (relay4, HIGH);

    delay(1000);

    digitalWrite (relay5, LOW);
    delay(1000);
    digitalWrite (relay5, HIGH);

    delay(1000);
    
    digitalWrite (relay6, LOW);
    delay(1000);
    digitalWrite (relay6, HIGH);

    delay(1000);

    for(angle = 0; angle < 180; angle++) 
    { 
     servo.write(angle); 
     delay(20); 
    } 

    delay(1000);
    

    
    
}
