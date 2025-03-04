#pragma once

//--------------------------------------------------------------------------------------------
// This file contains the definitions of the types used in the communication 
// between the ESP32 and the Arduino.
//
// THESE TYPES MUST BE EXACTLY THE SAME ON BOTH THE ARDUINO AND THE ESP32.
//--------------------------------------------------------------------------------------------

#include <cstdint>
#include <cstring>
#include <string>

enum EspCommand : char {
    CMD_REQUEST_IMAGE = 1,
    CMD_SET_PARAM = 2
};

enum EspPacketType : char {
    IMAGE_PACKET = 1,
    BASIC_PACKET = 2
};


#pragma pack(push, 1)   // Remove padding from structs for consistent memory layout

struct EspToPicoPacket
{
    static constexpr EspPacketType packetType = BASIC_PACKET;
    int8_t whiteDist;
    bool stopDetected;
};


struct EspToPicoPacketImage
{
    static constexpr EspPacketType packetType = IMAGE_PACKET;
    int8_t whiteDist;
    bool stopDetected;
    uint16_t image[96][96];
};

struct PicoToEspPacket
{
    EspCommand command;
    char label[6];
    int16_t data;
};

struct AckPacket
{
    bool ack;
    char label[6];
};
#pragma pack(pop)