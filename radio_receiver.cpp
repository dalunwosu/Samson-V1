#include <string.h>
#include <Arduino.h>
#include <cstring>
#include <../include/radio.hpp>


struct __attribute__((packed)) Packet{
    int32_t lat;
    int32_t lon;
    uint16_t pressure;
};

LoRraModule lora(Serial2);
static Packet pkt1;
// Creating uint8_t type ptr to the first byte of this struct
uint8_t* pkt_ptr = reinterpret_cast<uint8_t*>(&pkt1);
int32_t lat;
int32_t lon;
uint16_t pressure;

void setup(){
    Serial.begin(9600);
    lora.startup(sizeof(Packet), 21,22,23,normal);
}

void loop(){
    Serial.println(" -- Reciever Test -- ");

    // Ground Station Usage:
    lora.receiveMessage(pkt_ptr);
    lat = pkt1.lat;
    lon = pkt1.lon;
    pressure = pkt1.pressure;
    Serial.println("Latitude"); Serial.print(lat);
    Serial.println("Longitude"); Serial.print(lon);
    Serial.println("Pressure"); Serial.print(pressure);
    delay(1000);
}