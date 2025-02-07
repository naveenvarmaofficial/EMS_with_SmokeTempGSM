#include <SoftwareSerial.h>
#include <dht.h>
#include <LiquidCrystal.h>
#define dht_apin A3
#define mq2_pin A4


int i;
int Smokelvl;

SoftwareSerial mySerial(6, 7); //SIM800L Tx & Rx is connected to Arduino #3 & #2

dht DHT;
const int rs = 12, en = 11, d4 = 2, d5 = 3, d6 = 4, d7 = 5;
 LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  Serial.println("Initializing GSM..."); 
  delay(1000);

  mySerial.println( "AT" ); //Once the handshake test is successful, it will back to OK
  updateSerial();
  mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  mySerial.println("AT+CMGS=\"+91 phn number\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
  updateSerial();
  mySerial.print("Setup Completed!! Ready To Use."); //text content
  updateSerial();
  mySerial.write(26);
  updateSerial();
  mySerial.println("AT+CMGF=1"); 
  updateSerial();
  mySerial.println("AT+=1,2,0,0,0");
  updateSerial();
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("Intilizing");
  lcd.setCursor(0,1);
  lcd.print("    :)    ");
  pinMode(A3,INPUT);
  delay(500);
  Serial.println("\n....Temperature and Humidity Sensor....");
  Serial.println("*********************");
  delay(500);
  Serial.println("....Smoke Sensor is Ready...");
  Serial.println("*********************");
  Serial.print("Initializing");
  for(i=0;i<3;i++)
   {
    Serial.print(".");  
    delay(500);
    }
  Serial.println("");
  
  delay(500);
  
  inti();
}

void loop()
{
  String SMS_text;
  delay(500);
while(mySerial.available()) 
{
SMS_text=mySerial.readString();

int S=SMS_text.indexOf("Status");
if (S>10)
{
  Right();
  }
}
  String cmd = "";
 Smokelvl=analogRead(mq2_pin);
 DHT.read11(dht_apin);
 if(Serial.available() > 0)
 {
  cmd = Serial.readString();
  if((cmd == "status"))
  {
    Right();
    }
  else 
  {
    intrupW();
    }
  }

if (Smokelvl>300) 
{
 smklvl();
}
 else if(DHT.temperature > 30)
{
 templvl();
}
lcd.setCursor(0,0);
lcd.print("Humidity:");
lcd.print(DHT.humidity);
lcd.setCursor(0,1);
lcd.print("Temperature:");
lcd.print(DHT.temperature);

Serial.flush();
}

void inti() 
{
  DHT.read11(dht_apin);
  Serial.println("====================");
  Serial.print("Initially The Humidity percentage is ");
  Serial.print(int(DHT.humidity));
  Serial.println(" %");
  Serial.print("Initially The Temperature is ");
  Serial.print(int(DHT.temperature));
  Serial.println(" c");
  Serial.println("====================");
  
  delay(1000);
 
}

void Right()

{

Serial.println("Request Received");
Serial.println("Sending SMS");
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Request Received");
lcd.setCursor(0,1);
lcd.print("Sending SMS");
mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
updateSerial();
mySerial.println("AT+CMGS=\"+91 phn number\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
updateSerial();
DHT.read11(dht_apin);
String Output="The Humidity percentage is "+String(int(DHT.humidity))+" %"+"\nThe Temperature is "+String(int(DHT.temperature))+" c";
mySerial.print(Output); //text content
updateSerial();
mySerial.write(26);
delay(5000);
lcd.clear();
mySerial.println("AT+CMGF=1"); 
mySerial.println("AT+CNMI=1,2,0,0,0");
}

void intrupW()

{
lcd.clear();
Serial.println("INVALID Command");
lcd.setCursor(0,0);
lcd.print("INVALID Command");

delay(5000);
lcd.clear();
}


void smklvl()
{
lcd.clear();  
Serial.println("Smoke Level is above the Limit");
Smokelvl = analogRead(mq2_pin);
Serial.print(Smokelvl);
Serial.println("smoke is Detected");
lcd.setCursor(0,0);
lcd.print("Smoke Detected");
lcd.setCursor(0,1);
lcd.print("Open Windows");

lcd.clear();
mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
updateSerial();
mySerial.println("AT+CMGS=\"+91 phn number\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
updateSerial();
mySerial.print("Smoke Level is Above The Limit."); //text content
updateSerial();
mySerial.write(26);
delay(30000);
mySerial.println("AT+CMGF=1"); 
mySerial.println("AT+CNMI=1,2,0,0,0");

}

void templvl()

{
lcd.clear();
lcd.setCursor(0,0);
DHT.read11(dht_apin);
Serial.print("Temperature Level Exceeded the Limit ");
Serial.print(DHT.temperature);
Serial.println(" Degree C");
lcd.print("Temperature");
lcd.setCursor(0,1); 
lcd.print("Exceeded");
delay(5000);
lcd.clear();
mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
updateSerial();
mySerial.println("AT+CMGS=\"+91 phn number\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
updateSerial();
mySerial.print("Temperature level is Too High"); //text content
updateSerial();
mySerial.write(26);
mySerial.println("AT+CMGF=1"); 
mySerial.println("AT+CNMI=1,2,0,0,0");
delay(30000);

}

void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}
