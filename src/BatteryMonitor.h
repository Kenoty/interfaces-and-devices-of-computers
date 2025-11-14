#pragma once

#include <windows.h>
#include <powrprof.h>
#include <string>
#include <batclass.h>
#include <setupapi.h>
#include <vector>

class BatteryMonitor {
public:
    BatteryMonitor();
    ~BatteryMonitor();
    bool updateStatus();

    std::string getPowerStatusString() const;
    int getBatteryPercent() const { return status.BatteryLifePercent; }
    DWORD getBatteryLifeTime() const { return status.BatteryLifeTime; }
    DWORD getBatteryFullLifeTime() const { return status.BatteryFullLifeTime; }
    bool getBatterySavingMode() const;

    bool isOnACPower() const { return status.ACLineStatus == 1; }
    bool isBatterySaverOn() const { return (status.SystemStatusFlag & 1) == 1; }

    bool initializeAdvancedBatteryInfo();
    std::vector<std::string> getBatteryType() const;

    static bool enterSleep();
    static bool enterHibernate();

private:
    SYSTEM_POWER_STATUS status;
    std::vector<BATTERY_INFORMATION> batteries;
    HANDLE hBattery;
};
