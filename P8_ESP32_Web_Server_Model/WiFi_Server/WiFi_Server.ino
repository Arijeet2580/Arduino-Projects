#include <WiFi.h>
// Replace with your network credentials
const char* ssid = "redmi 9i";
const char* password = "kamehameha";
// Set web server port number to 80
WiFiServer server(80);
// Variable to store the HTTP request
String header;
String ledState = "off";
// Assign output variables to GPIO pins
const int led = 2;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(led, OUTPUT);
  // Set outputs to LOW
  digitalWrite(led, LOW);
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println();
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  WiFiClient client= server.available(); //Used to Listen to Incoming Clients
  if(client){          //Opens when a Client Connects
    Serial.println("New Client: ");
    String currLine=""; //Stores all incoming Data from the Client
    while(client.connected()){//1 
      if(client.available()){//2
        char c= client.read();
        Serial.write(c);
        header+=c;

        if(c=='\n'){//3 //If the incoming data is a newLine character in a row,Thats the end of the HTTP request
      
          if(!currLine.length()){//4
            //The following Lines is the appropriate Response for any given Client command given by the client Server
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection:close");
            client.println();
            /*
            */
            if(header.indexOf("GET /26/on") >=0){ 
                Serial.println("Turning On led in GPIO 26 pin");
                ledState="ON";
                digitalWrite(led,HIGH);
            }
            else if(header.indexOf("GET /26/off") >=0){
                Serial.println("Turning Off led in GPIO 26 pin");
                ledState="OFF";
                digitalWrite(led,LOW);              
            }

            //HTML CSS Code for ESP32
            client.println("<!DOCTYPE html><html>");
            client.println(" <head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"> ");                      
            client.println("<title>Station Mode Console</title>");
            client.println("<style>html{font-family:Helvetica;display:inline-block;margin:0px auto;text-align:center;}");
            client.println(".button{background-color:#4CAF50;border:none;color:white;padding:16px 40px;text-decoration:none;font-size:30px;margin:2px;cursor:pointer;}");
            client.println(".button2{background-color:#555555;}</style></head>");
            //4CAF50: FRUIT SALAD GREEN #555555 GREY COLOR
            client.println("<body><h1>ESP32 Station Mode Server</h1>");
            //Following line used to display the State of Led connected to the GPIO pin 26
            client.println("<p>LED state "+ledState+" </p>");
            if(ledState=="off" || ledState=="OFF"){
                client.println("<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>");                
            }
            else{
              client.println("<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
   
            client.println("</body></html>");
            //HTTP response ends with another blank line
            client.println();
            //Break out of the while loop
            break;        
          } // 4 if(!currLine.length()){ ends
          else{ // if you got a newline, then clear currLine
            currLine="";
          }
        } //3 if(c=='\n')ends
        else if(c!='\r'){ // if you got anything else but a carriage return character,
          currLine+=c; //Add it to the end of the currLine
        }
      }//2 if(client.available()) ends
    }//1 while(client.connected()){ ends
    
    //Clear the header variable
    header="";
    //Close the  connection 
    client.stop();
    Serial.println("Client Disconnected");
    Serial.println();
  }
}
