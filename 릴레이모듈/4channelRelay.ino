int relay1 = 2; //릴레이1 핀설정
int relay2 = 3; //릴레이2 핀설정
int relay3 = 4; //릴레이3 핀설정
int relay4 = 5; //릴레이4 핀설정
 
void setup ()
{
  pinMode (relay1, OUTPUT); // relay1 -> 배수
  pinMode (relay2, OUTPUT); // relay2 -> 급수
  pinMode (relay3, OUTPUT); // relay3 -> 히터
  pinMode (relay4, OUTPUT); // relay4 -> 조명
 

  digitalWrite (relay1, HIGH);
  digitalWrite (relay2, HIGH);
  digitalWrite (relay3, HIGH);
  digitalWrite (relay4, HIGH);
}
void loop ()
{

  digitalWrite (relay1, LOW); // 릴레이 ON
  digitalWrite (relay2, LOW); // 릴레이 ON
  digitalWrite (relay3, LOW); // 릴레이 ON
  digitalWrite (relay4, LOW); // 릴레이 ON
  delay(1000);
  
  digitalWrite (relay1, HIGH); // 릴레이 OFF
  digitalWrite (relay2, HIGH); // 릴레이 OFF
  digitalWrite (relay3, HIGH); // 릴레이 OFF
  digitalWrite (relay4, HIGH); // 릴레이 OFF
  delay (1000);    

  //조건없이 릴레이 작동여부 확인
}

