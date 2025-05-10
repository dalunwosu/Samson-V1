// #include <Arduino.h>
// #include <../include/radio.hpp>

// struct __attribute__((packed)) Packet{
//     int32_t lat;
//     int32_t lon;
//     uint16_t pressure;
// };

// LoRraModule lora(Serial1);

// static Packet pkt1;
// // Creating uint8_t type ptr to the first byte of this struct
// uint8_t* pkt_ptr = reinterpret_cast<uint8_t*>(&pkt1);

// void setup()
// {
//     delay(2000);
//     Serial.begin(9600);
//     lora.startup(sizeof(Packet), 21,22,23,normal);
//     pkt1.lat = 10123;
//     pkt1.lon = 10456;
//     pkt1.pressure = 10789;

// }

// void loop(){
//     // Avionics Usage:
//     Serial.println(lora.SndMessage(pkt_ptr));
//     delay(1000);
// }