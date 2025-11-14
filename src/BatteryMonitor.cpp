#include "BatteryMonitor.h"
#include <powrprof.h>
#include <devguid.h>

#pragma comment(lib, "powrprof.lib")
#pragma comment(lib, "setupapi.lib")


BatteryMonitor::BatteryMonitor(): hBattery(INVALID_HANDLE_VALUE) {
    ZeroMemory(&status, sizeof(SYSTEM_POWER_STATUS));
    initializeAdvancedBatteryInfo();
}

BatteryMonitor::~BatteryMonitor() {
    if (hBattery != INVALID_HANDLE_VALUE) {
        CloseHandle(hBattery);
    }
}

bool BatteryMonitor::updateStatus() {
    return GetSystemPowerStatus(&status);
}

std::string BatteryMonitor::getPowerStatusString() const {
    switch (status.ACLineStatus) {
    case 0: return "от аккумулятора";
    case 1: return "от сети";
    case 255: return "статус неизвестен";
    default: return "неопределенный статус";
    }
}

bool BatteryMonitor::getBatterySavingMode() const {
    return status.SystemStatusFlag;
}

bool BatteryMonitor::initializeAdvancedBatteryInfo() {
    HDEVINFO hdev = SetupDiGetClassDevs(&GUID_DEVCLASS_BATTERY, 0, 0, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

    if (hdev == INVALID_HANDLE_VALUE) return false;

    bool result = false;
    SP_DEVICE_INTERFACE_DATA did = {0};
    did.cbSize = sizeof(did);

    for (DWORD i = 0; SetupDiEnumDeviceInterfaces(hdev, 0, &GUID_DEVCLASS_BATTERY, i, &did); i++) {
        DWORD requiredSize = 0;
        SetupDiGetDeviceInterfaceDetail(hdev, &did, 0, 0, &requiredSize, 0);

        PSP_DEVICE_INTERFACE_DETAIL_DATA pdidd = (PSP_DEVICE_INTERFACE_DETAIL_DATA)LocalAlloc(LPTR, requiredSize);
        if (!pdidd) continue;

        pdidd->cbSize = sizeof(*pdidd);
        if (SetupDiGetDeviceInterfaceDetail(hdev, &did, pdidd, requiredSize, &requiredSize, 0)) {
            hBattery = CreateFile(pdidd->DevicePath, GENERIC_READ | GENERIC_WRITE,
                                  FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

            if (hBattery != INVALID_HANDLE_VALUE) {
                BATTERY_QUERY_INFORMATION bqi = {0};
                DWORD bytesReturned = 0;

                if (DeviceIoControl(hBattery, IOCTL_BATTERY_QUERY_TAG, NULL, 0,
                                    &bqi.BatteryTag, sizeof(bqi.BatteryTag), &bytesReturned, NULL) && bqi.BatteryTag) {

                    BATTERY_INFORMATION bi = {0};
                    bqi.InformationLevel = BatteryInformation;

                    if (DeviceIoControl(hBattery, IOCTL_BATTERY_QUERY_INFORMATION, &bqi, sizeof(bqi),
                                        &bi, sizeof(bi), &bytesReturned, NULL)) {
                        batteries.push_back(bi);
                        result = true;
                    }
                }
            }
        }
        LocalFree(pdidd);
    }
    SetupDiDestroyDeviceInfoList(hdev);
    return result;
}

std::vector<std::string> BatteryMonitor::getBatteryType() const {
    std::vector<std::string> result;

    for(int i = 0; i < batteries.size(); i++) {
        char temp[5] = {0};
        memcpy(temp, &(batteries[i].Chemistry), 4);

        result.push_back(std::string(temp));
    }

    return result;
}

bool BatteryMonitor::enterSleep() {
    return SetSuspendState(FALSE, TRUE, FALSE) != FALSE;
}

bool BatteryMonitor::enterHibernate() {
    SYSTEM_POWER_CAPABILITIES powerCaps;
    if (GetPwrCapabilities(&powerCaps) && powerCaps.SystemS4) {
        return SetSuspendState(TRUE, TRUE, FALSE) != FALSE;
    }
    return false;
}
