#include <Arduino.h>
#include <NimBLEDevice.h>

#include "./log_macros.h"
#include "./bleretro32.h"
#include "./xbox.h"
#define SCAN_TIME 10

NimBLEScan *scanner;
NimBLEAdvertisedDevice *foundDevice;
pad_definition_t *supported_pads = nullptr;
size_t number_of_supported_pads = 0;
pad_definition_t *found_pad;
CnnStatus cnn_status = CnnStatus::Idle;
ControllerDataSerialReport ControllerDataSerialReportStatus = ControllerDataSerialReportEnable;
class AdvertisedDeviceCallbacks : public NimBLEAdvertisedDeviceCallbacks
{
    void onResult(NimBLEAdvertisedDevice *advertisedDevice)
    {
        BLERETRO_LOGF("Found device: %s\n", advertisedDevice->toString().c_str());
        auto deviceName = advertisedDevice->getName();
        for (size_t i = 0; i < number_of_supported_pads; i++)
        {
            if (deviceName == supported_pads[i].name)
            {

                BLERETRO_LOGF("** Found '%s'\n", supported_pads[i].name);
                foundDevice = advertisedDevice;
                found_pad = &supported_pads[i];
                cnn_status = CnnStatus::DeviceFound;
                scanner->stop();
            }
        }
    }
};

static xbox_controller_data_t old_status;

void BLERetro32_Setup(pad_definition_t *pad_list, size_t count)
{
    supported_pads = pad_list;
    number_of_supported_pads = count;

    BLERETRO_MSGF("Setting up Bluetooth\n");
    NimBLEDevice::setScanFilterMode(CONFIG_BTDM_SCAN_DUPL_TYPE_DEVICE);
    NimBLEDevice::setScanDuplicateCacheSize(200);
    NimBLEDevice::init("BLERetro32");
    NimBLEDevice::setOwnAddrType(BLE_OWN_ADDR_PUBLIC);
    NimBLEDevice::setSecurityAuth(true, false, false);

    BLERETRO_LOGF("Setting up scanner\n");
    scanner = NimBLEDevice::getScan();
    scanner->setAdvertisedDeviceCallbacks(new AdvertisedDeviceCallbacks());
    scanner->setActiveScan(true);
    scanner->setInterval(1000);
    scanner->setWindow(900);

    BLERETRO_LOGF("Setting up GPIO\n");
}

void ScanCompleteCB(NimBLEScanResults results)
{
    BLERETRO_LOGF("** Scan ended");
    if (cnn_status == CnnStatus::Scanning)
    {
        cnn_status = CnnStatus::Idle;
        BLERETRO_LOGF(" ...restarting\n");
    }
    else
    {
        BLERETRO_LOGF("\n");
    }
}

CnnStatus ScanAndConnect()
{
    BLERETRO_MSGF("Starting scanner\n");
    scanner->clearResults();
    scanner->clearDuplicateCache();
    scanner->setDuplicateFilter(true);

    scanner->start(SCAN_TIME, &ScanCompleteCB, false);
    return CnnStatus::Scanning;
}

class ClientCallbacks : public NimBLEClientCallbacks
{
public:
    ClientCallbacks()
    {
    }

    void onConnect(NimBLEClient *pClient)
    {
        BLERETRO_LOGF("** onConnect\n");
    };

    void onDisconnect(NimBLEClient *pClient)
    {
        BLERETRO_LOGF("** onDisconnect\n");
        BLERETRO_LOGF("Disconnected\n");
        cnn_status = CnnStatus::Idle;
    };

    uint32_t onPassKeyRequest()
    {
        BLERETRO_LOGF("** onPassKeyRequest\n");
        return 0;
    };

    bool onConfirmPIN(uint32_t pass_key)
    {
        BLERETRO_LOGF("** onConfirmPIN\n");
        return true;
    };
};

void CharacteristicNofifyCB(NimBLERemoteCharacteristic *characteristic, uint8_t *data, size_t length, bool is_notify)
{
    if (length == sizeof(xbox_controller_data_t))
    {
        auto status = *((xbox_controller_data_t *)data);
        if (memcmp(&old_status, &status, sizeof(xbox_controller_data_t)))
        {
            XBOX_JoyconLocation xboxjoyconLdata = XBOX_InterpretJoycon(status, LEFTJOYCON);
            XBOX_JoyconLocation xboxjoyconRdata = XBOX_InterpretJoycon(status, RIGHTJOYCON);
            if (ControllerDataSerialReportStatus == ControllerDataSerialReportEnable)
            {
                BLERETRO_LOGF("        Data %d-> ", millis());
                BLERETRO_LOGF("isNotify: %d\n", is_notify);

                if (status.btnA)
                    BLERETRO_LOGF("btnA: %d ", status.btnA);
                if (status.btnX)
                    BLERETRO_LOGF("btnX: %d ", status.btnX);
                if (status.btnB)
                    BLERETRO_LOGF("btnB: %d ", status.btnB);
                if (status.btnY)
                    BLERETRO_LOGF("btnY: %d ", status.btnY);
                // ABXY
                if (DPAD_U <= status.dpad && status.dpad <= DPAD_UL)
                {
                    switch (status.dpad)
                    {
                    case DPAD_U:
                        BLERETRO_LOGF("dpad: up ");
                        break;
                    case DPAD_D:
                        BLERETRO_LOGF("dpad: down ");
                        break;
                    case DPAD_L:
                        BLERETRO_LOGF("dpad: left ");
                        break;
                    case DPAD_R:
                        BLERETRO_LOGF("dpad: right ");
                        break;
                    case DPAD_DL:
                        BLERETRO_LOGF("dpad: down left ");
                        break;
                    case DPAD_DR:
                        BLERETRO_LOGF("dpad: down right ");
                        break;
                    case DPAD_UL:
                        BLERETRO_LOGF("dpad: up left ");
                        break;
                    case DPAD_UR:
                        BLERETRO_LOGF("dpad: up right ");
                        break;
                    }
                }
                // dpad
                if (status.btnL)
                    BLERETRO_LOGF("LB: %d ", status.btnL);
                if (status.btnR)
                    BLERETRO_LOGF("RB: %d ", status.btnR);
                // shoulder button

                BLERETRO_LOGF("LT: %d ", status.trg_l);
                BLERETRO_LOGF("RT: %d ", status.trg_r);
                // trigger
                BLERETRO_LOGF("\n");

                BLERETRO_LOGF("joy_l(%d,%d),(%.2lf,%.2lf)\n", status.joy_l_h, status.joy_l_v, xboxjoyconLdata.r, xboxjoyconLdata.angle);
                BLERETRO_LOGF("joy_r(%d,%d),(%.2lf,%.2lf)\n", status.joy_r_h, status.joy_r_v, xboxjoyconRdata.r, xboxjoyconRdata.angle);
                // joycon
            }
            old_status = status;
        }
    }
}

void ConnectToHID(NimBLEAdvertisedDevice *device)
{
    Serial.printf("Connecting\n");
    auto client = NimBLEDevice::createClient(foundDevice->getAddress());
    client->setClientCallbacks(new ClientCallbacks(), true);
    client->setConnectTimeout(5); // Por defecto son 30 segundos
    cnn_status = CnnStatus::Connecting;
    int retryCount = 9;
    do
    {
        BLERETRO_LOGF("Connecting %d\n", retryCount);
        client->connect(true);
        delay(500);
        retryCount--;
    } while (!client->isConnected() && retryCount > 0);

    auto services = client->getServices(true);
    static auto hidServiceUUID = NimBLEUUID("1812");
    for (auto service : *services)
    {
        auto serviceUUID = service->getUUID();
        if (serviceUUID.equals(hidServiceUUID))
        {
            BLERETRO_LOGF("-> Found HID\n");
            auto characteristics = service->getCharacteristics(true);
            for (auto characteristic : *characteristics)
            {
                BLERETRO_LOGF("---> Found characteristic %s\n", characteristic->toString());
                if (characteristic->canRead())
                {
                    BLERETRO_LOGF("-----> Can read...\n");
                    int retryCount = 3;
                    auto str = characteristic->readValue();
                    while (str.size() == 0 && retryCount > 0)
                    {
                        retryCount--;
                        str = characteristic->readValue();
                    }
                    BLERETRO_LOGF("-----> ...'%s'\n", str);
                }
                if (characteristic->canNotify())
                {
                    BLERETRO_LOGF("-----> Can notify. Suscribing.\n");
                    characteristic->subscribe(true, CharacteristicNofifyCB, true);
                    cnn_status = CnnStatus::Connected;
                    BLERETRO_LOGF("Connected.\n");
                }
            }
        }
    }
    if (cnn_status != CnnStatus::Connected)
    {
        NimBLEDevice::deleteClient(client);
        BLERETRO_LOGF("Can't connect.\n");
        cnn_status = CnnStatus::Idle;
    }
}

CnnStatus BLERetro32_Loop()
{
    if (cnn_status == CnnStatus::Idle)
    {
        cnn_status = ScanAndConnect();
    }
    else if (cnn_status == CnnStatus::DeviceFound)
    {
        if (foundDevice != nullptr)
        {
            ConnectToHID(foundDevice);
        }
        else
        {
            cnn_status = CnnStatus::Idle;
        }
    }
    else if (cnn_status == CnnStatus::Connected)
    {
    }

    return cnn_status;
}

xbox_controller_data_t ReturnControllerData(void)
{
    return old_status;
}

void SetControllerSerialReportStatus(ControllerDataSerialReport input)
{
    ControllerDataSerialReportStatus = input;
}