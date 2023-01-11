#ifndef MTO_SOCKET
#define MTO_SOCKET

#include "mto.h"

typedef struct socket_driver_t
{
    uint16_t socketType;

    HANDLE_t (*createInstance)(HANDLE_t options);

} socket_driver_t;

typedef uint16_t socket_type_t;

void registerSocketDrivers(socket_driver_t *socketDrivers, uint8_t size);

HANDLE_t createSocket(socket_type_t socketType, HANDLE_t options);

#endif // !MTO_SOCKET