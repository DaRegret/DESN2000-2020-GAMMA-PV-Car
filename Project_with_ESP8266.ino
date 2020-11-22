
// Load WiFi Library
#include <ESP8266WiFi.h>
#include <string.h>

// Network credentials
const char* ssid     = "All HD"; // WiFi name
const char* password = "hdhdhdhd";       // WiFi password

// Set web server port number to 80
WiFiServer server(80);
// Variable to store the HTTP request
String header;
// Auxiliar variables to store the current output state
String outputD1State = "off";
String outputD4State = "off";
String outputD2State = "off";
String outputD3State = "off";
String u = "nothing received";
float f=0.0;
float coal=0.0;
float insolation=0.0;

//variables of our choice
int counter = 0;
float measurement=0; // analogread stored here

// Assign output variables to GPIO pins
const int outputD1 = 5;
const int outputD2 = 4;
const int outputD3 = 0;
const int outputD4 = 2;
const int inputD5 = 14;
const int inputD6 = 12;
const int inputD7 = 13;
const int inputD8 = 15;


// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

int responsetime=0; //wait for 0 seconds for the arduino to deliver the correct value to the D1 Mini

void setup() {
  Serial.begin(9600); //gotta use 9600 instead of 115200 for uart connection
  // Initialize the input/output variables as input/outputs
  pinMode(outputD1, OUTPUT);
  pinMode(outputD2, OUTPUT);
  pinMode(outputD3, OUTPUT);
  pinMode(outputD4, OUTPUT);
  pinMode(inputD5, INPUT);
  pinMode(inputD6, INPUT);
  pinMode(inputD7, INPUT);
  pinMode(inputD8, INPUT);
  pinMode(inputA0, INPUT);
  
  // Set outputs to LOW
  digitalWrite(outputD1, LOW);
  digitalWrite(outputD2, LOW);
  digitalWrite(outputD3, LOW);
  digitalWrite(outputD4, LOW);

  // Set your Static IP address
  IPAddress local_IP(192, 168, 137, 189);
  // Set your Gateway IP address
  IPAddress gateway(192, 168, 1, 1);

  IPAddress subnet(255, 255, 0, 0);
  IPAddress primaryDNS(8, 8, 8, 8);   //optional
  IPAddress secondaryDNS(8, 8, 4, 4); //optional

  // Configures static IP address
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
    Serial.println("Yo did this even happen?");
  }

  //Serial.println("Now we start!");
  // Connect to Wi-Fi network with SSID and password
  //Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
  }
  // Print local IP address and start web server
  //Serial.println("");
  Serial.println("WiFi connected!");
  Serial.println("IP address: ");
  //Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();         
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
      //  Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
//use UART to receive reading (trial), read from arduino and print on website
    u = "nothing received";

    if(Serial.available()){
      f = Serial.parseFloat();
    }
//Serial.println(f);
Serial.println("I got f now!");
     
//pins
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /5/on") >= 0) {
              Serial.println("GPIO 5 on");
              outputD1State = "on";
              outputD4State = "off";
              outputD2State = "off";
              outputD3State = "off";
              digitalWrite(outputD1, HIGH);
              digitalWrite(outputD4, LOW);
              digitalWrite(outputD2, LOW);
              digitalWrite(outputD3, LOW);
              while(Serial.available()){
                f=Serial.parseFloat();
              }
            } else if (header.indexOf("GET /5/off") >= 0) {
              Serial.println("GPIO 5 off");
              outputD1State = "off";
              digitalWrite(outputD1, LOW);
            } 
              
              else if (header.indexOf("GET /2/on") >= 0) {
              //Serial.println("GPIO 2 on");
              outputD4State = "on";
              outputD1State = "off";
              outputD2State = "off";
              outputD3State = "off";
              digitalWrite(outputD4, HIGH);
              digitalWrite(outputD1, LOW);
              digitalWrite(outputD2, LOW);
              digitalWrite(outputD3, LOW);
              delay(responsetime);
            } else if (header.indexOf("GET /2/off") >= 0) {
              Serial.println("GPIO 2 off");
              outputD4State = "off";
              digitalWrite(outputD4, LOW);
            } else if (header.indexOf("GET /4/on") >= 0) {
              Serial.println("GPIO 4 on");
              outputD2State = "on";
              outputD1State = "off";
              outputD3State = "off";
              outputD4State = "off";
              digitalWrite(outputD2, HIGH);
              digitalWrite(outputD1, LOW);
              digitalWrite(outputD4, LOW);
              digitalWrite(outputD3, LOW);
              delay(responsetime);
            } else if (header.indexOf("GET /4/off") >= 0) {
              Serial.println("GPIO 4 off");
              outputD2State = "off";
              digitalWrite(outputD2, LOW);
            } else if (header.indexOf("GET /0/on") >= 0) {
              //Serial.println("GPIO 0 on");
              outputD3State = "on";
              outputD1State = "off";
              outputD2State = "off";
              outputD4State = "off";
              digitalWrite(outputD3, HIGH);
              digitalWrite(outputD1, LOW);
              digitalWrite(outputD2, LOW);
              digitalWrite(outputD4, LOW);
              delay(responsetime);
            } else if (header.indexOf("GET /0/off") >= 0) {
              Serial.println("GPIO 0 off");
              outputD3State = "off";
              digitalWrite(outputD3, LOW);
            }





            
//website        
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            
            //refresh every certain period
            client.println("<meta http-equiv=\"refresh\" content=\"4\">");
            
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;"); //text of buttons// global
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}"); //effect on cursor when hover around
            client.println(".button2 {background-color: #eb34eb;  border: none; color: orange; padding: 16px 40px;}</style></head>");// button specfic
            
            // Web Page Heading
            client.println("<body><h1>ESP8266 Web Server</h1>");
            client.println("<h1>HoHo's Bizzare Adventure</h1>");
            counter++;
            client.print("<p><strong>Counter: </strong>");
            client.println(counter);
            client.print("<strong>UART transimitting: </strong>");
            client.println(f);
            client.print("</p>");
            Serial.println("I have placed f onto the paragraph now!");
  
  //webiste button
              
            // Display current state, and ON/OFF buttons for GPIO 5, Pin D1  
            client.println("<p>GPIO 5,D1 - State " + outputD1State + "</p>");
            // If the outputD1State is off, it displays the ON button       
            if (outputD1State=="off") {
              client.println("<p><a href=\"/5/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/5/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 

//D2,3, and 4 are used to temporarily display current when they are on!               
            // Display current state, and ON/OFF buttons for GPIO 2, Pin D4
            client.println("<p>GPIO 2,D4 - State " + outputD4State + " measuring Power" + "<img src=\"https://i.pinimg.com/originals/38/a1/91/38a191eb7f544dd42d1b986118afa552.png\" alt=\"lightbulb\" style=\"width:50px;height:50px;\">" + "</p>");
            // If the outputD4State is off, it displays the ON button       
            if (outputD4State=="off") {
              client.println("<p><a href=\"/2/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              coal=f*0.957;
              client.println("<p>You are saving "); 
              client.println(coal);
              client.println(" grams of coal per hour.</p>");
              client.println("<p><a href=\"/2/off\"><button class=\"button button2\">");
              client.print(f);
              client.println("W</button></a></p>");
            }

            // Display current state, and ON/OFF buttons for GPIO 4, Pin D2  
            client.println("<p>GPIO 4,D2 - State " + outputD2State + " measuring Voltage" + "</p>");
            // If the outputD2State is off, it displays the ON button       
            if (outputD2State=="off") {
              client.println("<p><a href=\"/4/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/4/off\"><button class=\"button button2\">");
              client.print(f);
              client.println("V</button></a></p>");
            }

            // Display current state, and ON/OFF buttons for GPIO 0, Pin D3 
            client.println("<p>GPIO 0,D3 - State " + outputD3State + " measuring Current" + "</p>");
            // If the outputD3State is off, it displays the ON button       
            if (outputD3State=="off") {
              client.println("<p><a href=\"/0/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              insolation=f/2.4*1000;
              client.println("<p>The insolation is "); 
              client.println(insolation);
              client.println(" .</p>");
              client.print("<p><a href=\"/0/off\"><button class=\"button button2\">");
              client.print(f);
              client.println("A</button></a></p>");
              //Serial.println("I've printed f on button now!"); //for debugging
            }
              
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected");
    Serial.println("");
  }
  if (Serial.available()){
    f=Serial.parseFloat();
  }
}
