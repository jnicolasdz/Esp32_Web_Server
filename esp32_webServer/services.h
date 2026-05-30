
#ifndef SERVICES_H

#define SERVICES_H

#include <cstdint>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <WebServer.h>

void stringToMac(const String& mac, byte* macAddress) {
  int index = 0;
  for (int i = 0; i < 6; i++) {
    macAddress[i] = strtoul(mac.c_str() + index, nullptr, 16);
    index += 3; 
  }
}

void createMagicPacket(uint8_t* magicPacket, const byte* macAddress) {

  int index = 0;

  for(int i=0; i < 6; i++){
    magicPacket[index++] = 0xFF;
  }

  for(int i=0; i < 16; i++) {
    for(int j = 0; j < 6; j++) {
      magicPacket[index++] = macAddress[j];
    }
  }
}

void sendUDP(const void* data, size_t size, uint16_t puerto, IPAddress dest) {
    WiFiUDP udp;
    udp.beginPacket(dest, puerto);
    udp.write((const uint8_t*) data, size);
    udp.endPacket();
}

void sendTCP(String mensaje, IPAddress dest, uint16_t puerto, String ruta = "/") {

  WiFiClient client;
  if (client.connect(dest, puerto)) {
    client.println("POST " + ruta + " HTTP/1.1"); 
    client.println("Host: " + String(dest));
    client.println("Content-Length: " + String(mensaje.length()));
    client.println("Connection: close");
    client.println();
    client.print(mensaje);
    client.stop();
  } else {
    Serial.println("Connection failed to " + dest.toString());
  }
}

#endif 