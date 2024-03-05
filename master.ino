#include <AddicoreRFID.h>
#include <SPI.h>
#include <SoftwareSerial.h>



// RED LED를 디지털 11번 핀에 연결합니다.
const int RED_PIN = 2;
// GREEN LED를 디지털 10번 핀에 연결합니다.
const int GREEN_PIN = 3;
// BLUE LED를 디지털 9번 핀에 연결합니다.
const int BLUE_PIN = 4;

int check=0;
int M_SW=8;
int RXpin = 6;
int TXpin = 7;
int SETpin = 5;
int Chanel = 5;
SoftwareSerial HC11(RXpin,TXpin);
#define  uchar unsigned char
#define uint  unsigned int

//4 bytes tag serial number, the first 5 bytes for the checksum byte
uchar serNumA[5];

uchar fifobytes;
uchar fifoValue;

AddicoreRFID myRFID; // create AddicoreRFID object to control the RFID module

/////////////////////////////////////////////////////////////////////
//set the pins
/////////////////////////////////////////////////////////////////////
const int chipSelectPin = 10;
const int NRSTPD = 5;

//Maximum length of the array
#define MAX_LEN 16

void setup() {
  // 2번 핀을 출력으로 설정 합니다.
  pinMode(RED_PIN, OUTPUT);    
  // 3번 핀을 출력으로 설정 합니다.
  pinMode(GREEN_PIN, OUTPUT); 
  // 4번 핀을 출력으로 설정 합니다.
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(M_SW,INPUT_PULLUP);
  digitalWrite(RED_PIN,HIGH);
  Serial.begin(9600);                        // RFID reader SOUT pin connected to Serial RX pin at 9600bps 
 
  // start the SPI library:
  SPI.begin();
  
  pinMode(chipSelectPin,OUTPUT);              // Set digital pin 10 as OUTPUT to connect it to the RFID /ENABLE pin 
  digitalWrite(chipSelectPin, LOW);         // Activate the RFID reader
  pinMode(NRSTPD,OUTPUT);                     // Set digital pin 10 , Not Reset and Power-down
  digitalWrite(NRSTPD, HIGH);

  myRFID.AddicoreRFID_Init();  


  HC11.begin(9600);
  char CHANEL[4];
  sprintf(CHANEL,"%03d",Chanel);
//  pinMode(LEDpin,OUTPUT);
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

void loop(){
  uchar i, tmp, checksum1;
  uchar status;
  uchar str[MAX_LEN];
  uchar RC_size;
  uchar blockAddr;  //Selection operation block address 0 to 63
  String mynum = "";

  str[1] = 0x4400;
  //Find tags, return tag type
  status = myRFID.AddicoreRFID_Request(PICC_REQIDL, str); 
  if (status == MI_OK){
    Serial.println("RFID tag detected");
    Serial.print(str[0],BIN);
    Serial.print(" , ");
    Serial.print(str[1],BIN);
    Serial.println(" ");
  }

  //Anti-collision, return tag serial number 4 bytes
  status = myRFID.AddicoreRFID_Anticoll(str);
  if (status == MI_OK){
    checksum1 = str[0] ^ str[1] ^ str[2] ^ str[3];
    Serial.println("The tag's number is  : ");
    //Serial.print(2);
    Serial.print(str[0]);
    Serial.print(" , ");
    Serial.print(str[1],HEX);
    Serial.print(" , ");
    Serial.print(str[2],HEX);
    Serial.print(" , ");
    Serial.print(str[3],HEX);
    Serial.print(" , ");
    Serial.print(str[4],HEX);
    Serial.print(" , ");
    Serial.println(checksum1,HEX);
           
    // Should really check all pairs, but for now we'll just use the first
    if(str[0] == 54 || str[0] == 47 ||str[0] == 16 ||str[0] == 68 ){                      //You can change this to the first byte of your tag by finding the card's ID through the Serial Monitor
      HC11.print("OFF\r\n");
      check=1;
      Serial.print("Hello World!\n");
      digitalWrite(RED_PIN,LOW);
      digitalWrite(GREEN_PIN,HIGH);
      delay(1000);
      digitalWrite(GREEN_PIN,LOW);
      digitalWrite(RED_PIN,HIGH);
      
    }
    
    
    else{
      HC11.print("ON\r\n");
      check=0;
      digitalWrite(RED_PIN,LOW);
      delay(100);
      digitalWrite(RED_PIN,HIGH);
      delay(100);
      digitalWrite(RED_PIN,LOW);
      delay(100);
      digitalWrite(RED_PIN,HIGH);
      delay(100);
      digitalWrite(RED_PIN,LOW);
      delay(100);
      digitalWrite(RED_PIN,HIGH);
      delay(100);
      digitalWrite(RED_PIN,LOW);
      delay(100);
      digitalWrite(RED_PIN,HIGH);
      delay(100);
      digitalWrite(RED_PIN,LOW);
      delay(100);
      digitalWrite(RED_PIN,HIGH);
      delay(100);
    }
    Serial.println();
    delay(1000);
  }
  Serial.println(digitalRead(M_SW));
  if(check==1){
    while(digitalRead(M_SW)==HIGH);
    delay(100);
    while(digitalRead(M_SW)==LOW);
    delay(100);
    check = 0;
  }
  
  if(digitalRead(M_SW)==LOW)
  {
    if(check==0){
      HC11.print("ON\r\n");
    }
    else
    {
      HC11.print("OFF\r\n");
      check=0;
    }
    
  }
  myRFID.AddicoreRFID_Halt();      //Command tag into hibernation              

}
