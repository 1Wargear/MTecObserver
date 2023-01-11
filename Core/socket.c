#include "socket.h"

socket_driver_t* drivers;
uint8_t driverCount;

void registerSocketDrivers(socket_driver_t *socketDrivers, uint8_t size)
{
    drivers = socketDrivers;
    driverCount = size;
}

HANDLE_t createSocket(socket_type_t socketType, HANDLE_t options)
{
    for (int i = 0; i < driverCount; i++)
    {
        if(drivers[i].socketType == socketType)
        {
            return drivers[i].createInstance(options);
        }
    }
    
    return NULL_HANDLE;
}