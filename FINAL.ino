#include <Wire.h>



#include<DS3231.h>
#include <Wire.h>
#include <config.h>

// ds3231 ko lagi


#include <HTTPClientESP32Ex.h>
#include "FirebaseESP32.h"
#include <WiFi.h>
#include <WiFiClient.h>


#include <ESP32_Servo.h>

#define FIREBASE_HOST "dav-webapp.firebaseio.com" //Do not include https:// in FIREBASE_HOST
#define FIREBASE_AUTH "IbhQVmWMlc38tNPmUEsWXLkfklIz6H1Kb8JTMvKz"

FirebaseData firebaseData;
DS3231 temprsensor;
const char* ssid = "DAVProjectKoLagi";
const char* password = "DAV@12345";
//wifi AP setup
#define in1 13 //13 19 21 23
#define in2 19
#define in3 32
#define in4 23
#define pin 25
//motor driver setup
int distance; //ultrasonic ko distance
Servo servo;
 int modeVal, commandVal;
int servoPin  = 2; //servo initialized
float tempr;
void servoWrite(int degree)   {  servo.write(degree);   delay(1000);  }



void setup(void) {
  Serial.begin(115200); //serial begin
  Wire.begin();
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password); //wifi turned on


  
  servo.attach(servoPin); //servo attached
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true); 
  
 


  Serial.println("Firebase Started");



  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

}

void loop(void) {

 tempr = get_Temp(); //got tempr

 firebaseCommunicate(tempr,54,getUltraVal());
//0 automatic
  
 if(modeVal == 12){
  
    
  servo.write(90);
  initialPhase();
  if(getUltraVal() > 100) {  forward(); delay(1000); initialPhase(); delay(200); }else{

      servo.write(0);
      delay(500);
      if(getUltraVal() > 100) { servo.write(90); right(); delay(350); initialPhase();  delay(200); }else{
        
        servo.write(180);
        delay(500);
        if(getUltraVal() > 100) { servo.write(90); left(); delay(350); initialPhase();  delay(200);}else{
          
           //gadi paxadi
          }
        
        }
    
    
    }
  
  }


  if(modeVal == 13) {
    
    
  switch(commandVal){


    case 0:
      initialPhase();
      delay(200);
      break;

    case 1:
      forward();
      delay(1000);
      initialPhase();
      delay(200);
      break;

    case 2:
      backward();
      delay(1000);
      initialPhase();
      delay(200);
      break;

    case 3:
      left();
      delay(300);
      initialPhase();
      delay(200);
      break;

   case 4:
      right();
      delay(300);
      initialPhase();
      delay(200);
      break;

      

    
    default:
    initialPhase();
    break;
    
    
    
    
    }
    
    
    }

}


void forward() {
  Serial.println("RUNNING FORWARD");
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW) ;
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);


}
void backward() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH) ;
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}
void right() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW) ;
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);



}
void left() {

  Serial.println("RunNING LEFT");
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW) ;
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);





}

void initialPhase() {

  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW) ;
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);

}

float get_Temp(){ return temprsensor.getTemperature(); }

int getUltraVal()
{

  int x;

  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  delayMicroseconds(2);
  digitalWrite(pin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pin, LOW);

  pinMode(pin, INPUT);
  x = pulseIn(pin, HIGH);
  return ( x / (29 * 2));




}


void firebaseCommunicate(float tempr, float humidity, int obstacle){
 
  Firebase.setInt(firebaseData,"temp",tempr);
  
  Firebase.setInt(firebaseData,"humidity",humidity);
  
  Firebase.setInt(firebaseData,"obstacle",obstacle);

  modeVal = Firebase.getInt(firebaseData,"mode");
  commandVal = Firebase.getInt(firebaseData,"command");
  Firebase.setInt(firebaseData,"command",0);
Serial.println(tempr);
  
  
  }
