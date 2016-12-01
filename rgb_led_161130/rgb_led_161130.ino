#include <Wire.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char *ssid = "Eat_eat";
const char *password = "annie1222";

ESP8266WebServer server(80);

char led_status[200];
char rBuf[50];
char gBuf[50];
char bBuf[50];
String webSite;

#define LEDR 5
#define LEDG 4
#define LEDB 14

void turnoff(){
  digitalWrite(LEDR, LOW);
  digitalWrite(LEDG, LOW);
  digitalWrite(LEDB, LOW);
}

void buildWeb(){
  webSite += "<html>";
  webSite+="<head>\n";
  webSite+="<meta charset='UTF-8'/>\n";
  webSite+="<title>Form Control RGB_LED</title>\n";
  webSite+="<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/1.11.3/jquery.min.js\"></script>";
  webSite+="<script>";
  webSite+="$(document).ready(function(){";
  webSite+="$(\"[type = range]\").change(function(){";
  webSite+="var redval = $(\"#red\").val();";
  webSite+="$(\"#red-text\").text(redval);";
  webSite+="var greenval = $(\"#green\").val();";
  webSite+="$(\"#green-text\").text(greenval);";
  webSite+="var blueval = $(\"#blue\").val();";
  webSite+="$(\"#blue-text\").text(blueval);";
  webSite+="});";
  webSite+="});";
  webSite+="</script>";
  webSite+="</head>\n";
  webSite += "<body>";
  webSite += "<h1>RGB LED Control form</h1>";
  webSite += "<form action=\"/form1\">";
  webSite += "<p>Red Color : <span id = \"red-text\">0</span></p>";
  webSite += "<input type=\"range\" id=\"red\" name=\"R-led\" min=\"0\" max=\"255\" value=\"0\">";
  webSite += "<p>Green Color : <span id = \"green-text\">0</span></p>";
  webSite += "<input type=\"range\" id=\"green\" name=\"G-led\" min=\"0\" max=\"255\" value=\"0\">";
  webSite += "<p>Blue Color : <span id = \"blue-text\">0</span></p>";
  webSite += "<input type=\"range\" id=\"blue\" name=\"B-led\" min=\"0\" max=\"255\" value=\"0\" ><br><br>";
  webSite += "<input type=\"submit\" value=\"submit\"></form>";
  webSite += "</body></html>";

}

void handleRoot(){
  //send back client a HTML file
  server.send (200, "text/html", webSite); 
}

void ColorPicker(){
  int red_color = server.arg("R-led").toInt();
  int green_color = server.arg("G-led").toInt();
  int blue_color = server.arg("B-led").toInt();
  
  analogWrite(LEDR,red_color);
  analogWrite(LEDG,green_color);
  analogWrite(LEDB,blue_color);

  server.send(200,"text/html",webSite);
}

void handleNotFound(){
  server.send ( 404, "text/html", "404 Error!" );  
}

void setup(){
  Serial.begin(115200);
  // put your setup code here, to run once:
  turnoff();
  buildWeb();
  WiFi.begin(ssid, password); 
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  server.on("/",handleRoot);
  server.on("/form1",ColorPicker);
  server.onNotFound(handleNotFound);
  
  server.begin();
  Serial.println(WiFi.localIP());  
}

void loop(){
  // put your main code here, to run repeatedly:
  server.handleClient();  
}
