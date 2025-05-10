// For Flight Software:


// 1. Aux Pin: PIN that needs to user defined
// IDLE - Sends a HIGH if the buffer is free(either reciever or transmitting)
// BUSY - Sends a LOW if it's busy

// 2. M0 PIN : User defined LoRa configuration PINS

// 3. M1 PIN : User defined LoRa configuration PINS

// 4. Packet/Buffer Size: Size of the user defined data packet

#include <Arduino.h>
#include <HardwareSerial.h>
#include <Wire.h>
#include <string.h>
enum modeType {wakeup, sleepy, normal, configure};
struct __attribute__((packed)) TestPacket{
    int32_t lat;
    int32_t lon;
    uint16_t pressure;
};

//Communicates via UART
class LoRraModule{
    public:
        // Constructor
        LoRraModule(HardwareSerial &serial): SerialAT(serial) {}
        // Configure this if Radio Not working:
        HardwareSerial &SerialAT;
        char buffer[256];
        size_t pktSize;
        int Aux;
        std::string receivedString;
        bool receiveMessage(uint8_t *ptr);
        bool SndMessage(uint8_t *ptr);
        bool startup(int pkt_size, int Aux, int M0, int M1, modeType modeChosen); // Constructor Function
        TestPacket pkt;

    private:
        int x = 1;
};

bool LoRraModule::receiveMessage(uint8_t *ptr)
{
        if (digitalRead(this->Aux) == LOW)
        {
            Serial.println("Buffer was full, will try next cycle");
            return false;
        }
        else{
            this->SerialAT.readBytes(ptr, this->pktSize);
            return true;
        }
}

bool LoRraModule::SndMessage(uint8_t *ptr){
    if (digitalRead(this->Aux)==LOW){
        Serial.println("Buffer was full, will try next cycle");
        this->SerialAT.write(ptr, this->pktSize);
        return false;
    }

    else{
        this->SerialAT.write(ptr, this->pktSize);
        return true;
    }
}
    
bool LoRraModule::startup(int pkt_size, int Aux,int M0,int M1,modeType modeChosen){
    this->pktSize = pkt_size;
    Serial.println("Setting the pinmodes");
    pinMode(M0, OUTPUT);
    pinMode(M1,OUTPUT);
    pinMode(Aux,INPUT);
    while (digitalRead(Aux)!=HIGH)
    {
        ;
    }
    Serial.println("Aux HIGH detected!");
    try{
        this->SerialAT.begin(9600, SERIAL_8N1, 16, 17);
        this->Aux=Aux;
        if(modeChosen==wakeup){
            //Setting that you can set sleep and wake oscillations by some rate
            digitalWrite(M0,HIGH);
            digitalWrite(M1,LOW);         
        }
        else if(modeChosen==sleepy){
            digitalWrite(M0,HIGH);
            digitalWrite(M1,HIGH);
            //Low-power sleep state where the module consumes minimal power. 
            // It typically cannot transmit or receive in this mode and ignores UART input until woken up by changing the M0/M1 pins.
        }

        else if(modeChosen==normal){
            digitalWrite(M0,LOW);
            digitalWrite(M1,LOW);
        }

        else{
            //Configuration code, allows access to internal registers to control parameters of radio
            //In competition this may need to be actually set, but automatic is fine for now
            digitalWrite(M0,LOW);
            digitalWrite(M1,HIGH);
            //model Factory default parameter value: C0 00 00 62 00 12
            //Module model, frequency, address, channel, air speed, baud rate, Serial format, pwr
            //E22-900T33S, 868.125MHz, 0x0000, 0x12, 2.4kbps, 9600, 8N1, 33dbm
        }
        this->pkt.lat = 123;
        this->pkt.lon = 456;
        this->pkt.pressure = 789;
        this->SerialAT.write((uint8_t *)&this->pkt, sizeof(pkt));
        delay(4000);
        return true;
    }
    
    catch(...){
        return false;
    }


}
