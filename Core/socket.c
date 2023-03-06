#include "socket.h"

socket_driver_t* sock_drivers;
uint8_t sock_driverCount;

void registerSocketDrivers(socket_driver_t *socketDrivers, uint8_t size)
{
    sock_drivers = socketDrivers;
    sock_driverCount = size;    
}

HANDLE_t createSocket(socket_type_t socketType, HANDLE_t options)
{
    for (int i = 0; i < sock_driverCount; i++)
    {
        if(sock_drivers[i].socketType == socketType)
        {
            return sock_drivers[i].createInstance(options);
        }
    }
    
    return NULL_HANDLE;
}