#include <WiFi.h>
#include <WiFiUdp.h>
#include <WebServer.h>
#include <Arduino.h>

#include "services.h"
#include "view.h"

const char* ssid = "Hidden_Network";
const char* password = "pass12345";

const IPAddress broadcast = IPAddress(192, 168, 4, 255);
const int puerto = 8080;
const int LED_PIN = 2;

WebServer server(80);

void setup() {
    pinMode(LED_PIN, OUTPUT);
    // inicialización de comunicación serial
    Serial.begin(115200);

    // Configuración red 
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password);

    // declaración de endpoints
    server.on("/led", turnLed); // sample: /led?state=on or off
    server.on("/mp", getMagicPacket); // sample: /mp?mp=52:17:8A:22:81:8D
    server.on("/sendMP", sendMagicPacket); // sample: /sendMP?mp=52:17:8A:22:81:8D
    server.on("/printf", printMessage);  // sample: /printf?ip=192.168.4.2&msg=HolaMundo
    server.on("/suspendPC", suspendPC); // sample: /shutdownPC?ip=192.168.4.2
    server.on("/", handleRoot);

    server.begin();
    Serial.println("HTTP server started");
}

void loop() {
    server.handleClient();
}

// Routers

void getMagicPacket() {

  byte magicPacket[102];

  if (!server.hasArg("mp")) {
    server.send(400, "text/plain", "Missing MAC address : ?mp=<MAC Address>");
    return;
  }

  byte macAddress[6]; 
  String mac = server.arg("mp");
  stringToMac(mac, macAddress);

  createMagicPacket(magicPacket, macAddress);
  String out = "";
  
  for(int i = 0; i < 102; i++){
    out+= String(magicPacket[i], HEX);
  }

  server.send(200, "text/plain", out);
}

void sendMagicPacket() {

  byte magicPacket[102];

  if (!server.hasArg("mp")) {
    server.send(400, "text/plain", "Missing MAC address : ?mp=<MAC Address>");
    return;
  }

  byte macAddress[6]; 
  String mac = server.arg("mp");
  stringToMac(mac, macAddress);
  createMagicPacket(magicPacket, macAddress);
  sendUDP(magicPacket, 102, 7, broadcast);
  server.send(200, "text/plain", "Magic packet sent");
}

void printMessage() {

  if (!server.hasArg("msg") || !server.hasArg("ip")) {
    server.send(400, "text/plain", "Missing argument : ?msg=<menssage> or ip=<ip>");
    return;
  }
  
  String menssage = server.arg("msg");
  String ipStr = server.arg("ip");
  IPAddress ip;
  ip.fromString(ipStr);
  sendTCP(menssage, ip, puerto, "/print");
  server.send(200, "text/plain", ipStr + " : " + menssage);
}

void suspendPC() {

  if (!server.hasArg("ip")) {
    server.send(400, "text/plain", "Missing argument : ?ip=<ip>");
    return;
  }
  
  String ipStr = server.arg("ip");
  IPAddress ip;
  ip.fromString(ipStr);
  sendTCP("", ip, puerto, "/suspend");
  server.send(200, "text/plain", "Suspend order sent to" + ipStr + " on execution at 1 minute");
}

void handleRoot() {
  server.send(200, "text/html", INDEX_HTML);
}

void turnLed() {
  if(!server.hasArg("state")) {
    server.send(400, "text/plain", "Missing argument : ?state=on or off");
    return;
  }
  bool state = server.arg("state") == "on";
  led(LED_PIN, state);
  server.send(200, "text/plain", "turning on/off led");
}