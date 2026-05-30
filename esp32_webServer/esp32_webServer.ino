#include <WiFi.h>
#include <WiFiUdp.h>
#include <WebServer.h>
#include <Arduino.h>

#include "services.h"

const char* ssid = "Hidden_Network";
const char* password = "pass12345";

const IPAddress dest = IPAddress(192, 168, 4, 2);
const int puerto = 8080;
const byte macAddress[6] = {0x52,0x17,0x8A,0x22,0x81,0x8D};

WebServer server(80);

void setup() {

    // inicialización de comunicación serial
    Serial.begin(115200);

    // Configuración red 
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password);

    // declaración de endpoints
    server.on("/mp", getMagicPacket); // sample: /mp?mp=52:17:8A:22:81:8D
    server.on("/sendMP", sendMagicPacket); // sample: /sendMP?mp=52:17:8A:22:81:8D
    server.on("/printf", printMensaje);  // sample: /printf?msg=HolaMundo

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

  String mac = server.arg("mp");
  byte macAddress[6];
  int index = 0;
  for (int i = 0; i < 6; i++) {
    macAddress[i] = strtoul(mac.c_str() + index, nullptr, 16);
    index += 3; // saltar "XX:"
  }
  
  createMagicPacket(magicPacket, macAddress);
  String out = "";
  
  for(int i = 0; i < 102; i++){
    out+= String(magicPacket[i], HEX);
  }

  server.send(200, "text/plain", out);
}

void sendMagicPacket() {

  byte magicPacket[102];
  createMagicPacket(magicPacket, macAddress);
  sendUDP(magicPacket, 102, 7, dest);
  server.send(200, "text/plain", "Magic packet sent");
}

void printMensaje() {

  if (!server.hasArg("msg")) {
    server.send(400, "text/plain", "Missing argument : ?msg=<mensaje>");
    return;
  }
  
  String mensaje = server.arg("msg");
  sendTCP(mensaje, dest, puerto);
  server.send(200, "text/plain", "Printing into client : " + mensaje);
}