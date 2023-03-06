#ifndef MTO_SENS_DRIVER
#define MTO_SENS_DRIVER

#include "mto.h"

typedef struct sens_def_t
{
    static_id_t name;
    static_id_t driver;

    device_addr_t addr;

    HANDLE_t dsInfo;

} sens_def_t;

typedef struct sens_driver_t
{
    static_id_t name;
    int (*measure_fun_ptr)(sens_def_t* sdef, void* buffer);

} sens_driver_t;

void registerSensorDrivers(sens_driver_t *drivers, int length);

#endif // !MTO_SENS_DRIVER