#ifndef MTO_SENS_DRIVER
#define MTO_SENS_DRIVER

#include "mto.h"

typedef struct sens_driver_t
{
    static_id_t name;
    int (*measure_fun_ptr)(sens_def_t sdef, void* buffer);

} sens_driver_t;

#endif // !MTO_SENS_DRIVER