int analogPin = 0; //워터센서 analog port 0 연결 선언
int led = 13; // LED 디지털 13번 포트 연결 선언
int val = 0; // 전류변화값 변수선언

void setup() {
  pinMode(led,OUTPUT); //디지털 13번 포트를 LED OUTPUT으로 설정
  Serial.begin(9600); //시리얼 모니터 설정
}

void loop() {
  // put your main code here, to run repeatedly:
  val = analogRead(analogPin);
  if(val > 100)
  {
    digitalWrite(led,HIGH);
  }
  else
  {
    digitalWrite(led,LOW);
  }

  Serial.println(val);
  delay(500);
}
