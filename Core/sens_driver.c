#include "sens_driver.h"

int sensorDriverCount;
sens_driver_t *sensorDrivers;

void registerSensorDrivers(sens_driver_t *drivers, int length)
{
    sensorDriverCount = length;
    sensorDrivers = drivers;
}