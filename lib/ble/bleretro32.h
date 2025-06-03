#ifndef BLERETRO32_H
#define BLERETRO32_H

#include <stdlib.h>
#include "xbox.h"
enum class CnnStatus : uint8_t
{
    Idle,
    Scanning,
    DeviceFound,
    Connecting,
    Connected
};


struct pad_definition_t
{
    const char *name;
};

enum ControllerDataSerialReport{
    ControllerDataSerialReportEnable,
    ControllerDataSerialReportDisable
};

void BLERetro32_Setup(pad_definition_t *pad_list, size_t count);
CnnStatus BLERetro32_Loop();
xbox_controller_data_t ReturnControllerData(void);
void SetControllerSerialReportStatus(ControllerDataSerialReport input);
#endif
