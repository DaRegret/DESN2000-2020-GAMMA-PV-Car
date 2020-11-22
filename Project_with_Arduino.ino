int V_digital=0;
float V_analog=0;
int I_digital=0;
float I_analog=0;
float Power=0;
int period=200;
float coe=1.025;
int c1=5;
float measurement = 0;


const int lvl1=2;
const int lvl2=4;
const int lvl3=8;


const int getcurrent=7;
const int getvoltage=12;
const int getpower=13;

void setup()
{
  pinMode(A0,INPUT); 
  pinMode(A1,INPUT); //measuring input current   //by sensor, 1V/A
  pinMode(A2,INPUT); //measuring input voltage  //by voltage divider
  pinMode(lvl1,OUTPUT);//lvl 1
  pinMode(lvl2,OUTPUT);//lvl 2
  pinMode(lvl3,OUTPUT);//lvl 3

  pinMode(getcurrent,INPUT); //communication with ESP8266
  pinMode(getvoltage,INPUT);
  pinMode(getpower,INPUT);


  digitalWrite(lvl1,LOW);
  digitalWrite(lvl2,LOW);
  digitalWrite(lvl3,LOW);


  
  Serial.begin (9600);
}

void loop()
{  
 {//gathering information.
  I_digital=analogRead(A1);
  V_digital=analogRead(A2);
  I_analog=I_digital*5.0*coe/1023;
  V_analog=V_digital*5.0*coe/1023;
  Power=I_analog*V_analog;
 }

  //led indication for current generation
  if (I_analog<0.2){
    digitalWrite(lvl1,HIGH);
    //Serial.println("Mode 1");
  }else if(I_analog>=0.2 && I_analog<=0.3){
    digitalWrite(lvl1,HIGH);
    digitalWrite(lvl2,HIGH);
    //Serial.println("Mode 2");
  }else{
    digitalWrite(lvl1,HIGH);
    digitalWrite(lvl2,HIGH);
    digitalWrite(lvl3,HIGH);
    //Serial.println("Mode 3");
  }

//  
  {//display on serial for debug
//    Serial.println(V_digital);  
//      Serial.println(I_digital);    
//  Serial.print("Voltage is ");     
//  Serial.println(V_analog);      
//  Serial.print("Current is ");
//  Serial.println(I_analog); 
//  Serial.print("Power is ");                         
//  Serial.println(Power);  
  }  

  //test UART for delivery
  
  
  //delivery to esp8266
  if (digitalRead(getcurrent)==HIGH){
    Serial.println(I_analog);
    //Serial.println("Current delivered.");
  }else if (digitalRead(getvoltage)==HIGH){
    Serial.println(V_analog);
    //Serial.println("Voltage delivered.");
  }else if (digitalRead(getpower)==HIGH){
    Serial.println(Power);
    //Serial.println("Power delivered.");
  }
  //typical delay
  delay(period);   
}
