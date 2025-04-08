#pragma once

//--------------------------------------------------------------------------------------------
// This file contains the definitions of the types used in the communication 
// between the ESP32 and the Arduino.
//
// THESE TYPES MUST BE EXACTLY THE SAME ON BOTH THE ARDUINO AND THE ESP32.
//--------------------------------------------------------------------------------------------

#include <cstddef>
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

enum PacketLabel : uint8_t {
    NULL_LABEL = 0x00,
    PARAM_WHITE_RED_LIMIT = 0x01,
    PARAM_WHITE_GREEN_LIMIT = 0x02,
    PARAM_WHITE_BLUE_LIMIT = 0x03,
    PARAM_PERCENT_TO_STOP = 0x04,
    PARAM_STOP_GREEN_TOLERANCE = 0x05,
    PARAM_STOP_BLUE_TOLERANCE = 0x06,
    PARAM_STOPBOX_TL_X = 0x07,
    PARAM_STOPBOX_TL_Y = 0x08,
    PARAM_STOPBOX_BR_X = 0x09,
    PARAM_STOPBOX_BR_Y = 0x0A,
    PARAM_WHITE_CENTER_POS = 0x0B
};

static constexpr uint32_t SYNC_BYTES = 0xAA55AA55;

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
    PacketLabel label;
    int16_t data;
};

#pragma pack(pop)