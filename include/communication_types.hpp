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
    CMD_REQUEST_IMAGE = 0x01,
    CMD_SET_PARAM = 0x02
};

enum PacketType : uint8_t {
    ESP_PACKET = 0x01,
    COMMAND_PACKET = 0x02,
    ACK_PACKET = 0x03
};

static constexpr uint32_t SYNC_BYTES = 0xFEDCBA98;

#pragma pack(push, 1)   // Remove padding from structs for consistent memory layout

struct EspToPicoPacket
{
    PacketType packetType = ESP_PACKET;
    int8_t whiteDist;
    bool stopDetected;
    bool imageIncluded;
};

struct PicoToEspPacket
{
    PacketType packetType = COMMAND_PACKET;
    EspCommand command;
    char label[6];
    int16_t data;
};

struct AckPacket
{
    PacketType packetType = ACK_PACKET;
    bool ack;
    char label[6];
};

#pragma pack(pop)