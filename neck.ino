#include <SoftwareSerial.h>



//#define HC11_A

#define HC11_B

//#define HC11_TEST

/*

 * 사용하는 보드가 LED 제어 신호를 보내는 보드면

 * //#define HC11_A 를 

 * #define HC11_A 로 변경합니다.

 * 그 반대의 경우 

 * //#define HC11_B를 

 * #define HC11_B 로 변경합니다.

 */

int RXpin = 11;

int TXpin = 12;

int SETpin = 10;

int Chanel = 5;

int LEDpin = 6;





SoftwareSerial HC11(RXpin,TXpin);



void setup() {

  Serial.begin(9600);

  HC11.begin(9600);

  char CHANEL[4];

  sprintf(CHANEL,"%03d",Chanel);

  pinMode(LEDpin,OUTPUT);

  //CC41모듈의 채널을 설정합니다.

  pinMode(SETpin,OUTPUT);

  digitalWrite(SETpin,LOW);

  delay(500);

  HC11.print("AT+C");

  HC11.print(CHANEL);

  HC11.print("\r\n");

  delay(1000);

  while(HC11.available()>0){

    Serial.write(HC11.read());

  }

  digitalWrite(SETpin,HIGH);

}



void loop() {

  

  #ifdef HC11_A

  //여기에 A파트 HC11모듈의 소스코드가 올라갑니다.  

  delay(500);

  HC11.print("ON\r\n");

  delay(500);

  HC11.print("OFF\r\n");

  #endif

  

  #ifdef HC11_B

  //여기에 B파트 HC11모듈의 소스코드가 올라갑니다.

  static String DATA = "";

  while(HC11.available() > 0){

    DATA += (char)HC11.read();
    
  }

  if(DATA.startsWith("ON")){
    
    digitalWrite(LEDpin,HIGH);
    Serial.write("check1");

  }

  if(DATA.startsWith("OFF")){
    Serial.write("Check2");
    analogWrite(LEDpin,0);

  }

  if(DATA.endsWith("\r\n")){

    DATA = "";

  }

  #endif

  

  #ifdef HC11_TEST

  //HC-11 테스트용 코드

  while(Serial.available() > 0){

    HC11.write(Serial.read());

  }

  while(HC11.available() > 0){

    Serial.write(HC11.read());

  }

  #endif

  

}