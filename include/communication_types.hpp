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

enum EspCommand : uint8_t {
    CMD_REQUEST_IMAGE = 1,
    CMD_SET_PARAM = 2
};

enum EspPacketType : uint8_t {
    IMAGE_PACKET = 0x55,
    BASIC_PACKET = 0xAA
};

static constexpr uint8_t SYNC_BYTE = 0xDB;


#pragma pack(push, 1)   // Remove padding from structs for consistent memory layout

struct EspToPicoPacket
{
    EspPacketType packetType;
    int8_t whiteDist;
    bool stopDetected;
};


struct EspToPicoPacketImage
{
    EspPacketType packetType;
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