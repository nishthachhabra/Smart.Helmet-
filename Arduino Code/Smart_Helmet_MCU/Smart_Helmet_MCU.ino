#define freeFall D0
#define alcohol D1

#include <ESP8266WiFi.h>
#include "HTTPSRedirect.h"
#include "DebugMacros.h"

// Fill ssid and password with your network credentials
const char* ssid = "Chhabras";
const char* password = "9999496977";

const char* host = "glyphographic-silk.000webhostapp.com";

const int httpsPort = 443;

//url to call the php file saved on server
String url1=String("/messageaccident.php");
String url2=String("/emailaccident.php");
String url3=String("/messagealcohol.php");
String url4=String("/emailalcohol.php");

HTTPSRedirect* client = nullptr;
// used to store the values of free stack and heap
// before the HTTPSRedirect object is instantiated
// so that they can be written to Google sheets
// upon instantiation

void setup() {
  // put your setup code here, to run once:
  pinMode(freeFall, INPUT);
  pinMode(alcohol, INPUT);

  Serial.begin(115200);

  Serial.print("Connecting to wifi: ");
  Serial.println(ssid);
  // flush() is needed to print the above (connecting...) message reliably,
  // in case the wireless connection doesn't go through
  Serial.flush();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Use HTTPSRedirect class to create a new TLS connection
  client = new HTTPSRedirect(httpsPort);
  client->setPrintResponseBody(true);
  client->setContentTypeHeader("application/json");

  Serial.print("Connecting to ");
  Serial.println(host);

  // Try to connect for a maximum of 5 times
  bool flag = false;
  for (int i = 0; i < 5; i++) {
    int retval = client->connect(host, httpsPort);
    if (retval == 1) {
      flag = true;
      break;
    }
    else
      Serial.println("Connection failed. Retrying...");
  }

  if (!flag) {
    Serial.print("Could not connect to server: ");
    Serial.println(host);
    Serial.println("Exiting...");
    return;
  }

  Serial.println("Certificate mis-match");

}

int freeFallState = 1;
int alcoholState = 1;

void loop() {
  Serial.println(digitalRead(freeFall));
  Serial.println(digitalRead(alcohol));
  // put your main code here, to run repeatedly:
  if(digitalRead(freeFall)==HIGH){
    if(freeFallState == 1){
     Serial.println("freeFall");
     Serial.println("\nSend SMS");
     Serial.println("=========================");
   client->GET(url1,host);
     Serial.println("\nSend EMAIL");
     Serial.println("=========================");
     client->GET(url2,host);
     freeFallState = 0;
     delay(15000);
  }}
  else{
    freeFallState = 1;
  }
  if(digitalRead(alcohol)==HIGH){
    if(alcoholState == 1){
     Serial.println("alcohol");
     Serial.println("\nSend SMS");
     Serial.println("=========================");
     client->GET(url1,host);
     Serial.println("\nSend EMAIL");
     Serial.println("=========================");
     client->GET(url2,host);
     alcoholState = 0;
     delay(15000);
  }}
  else{
    alcoholState = 1;
  }
  
}
