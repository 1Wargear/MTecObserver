#ifndef MTO_SMAN
#define MTO_SMAN

#include <mto.h>
#include <sens_driver.h>
#include <program.h>

void sens_man_main(void);
#define SMAN_PROGRAM { "SMANP", &sens_man_main }

#define MAX_VALUE_SIZE 16

#endif // !MTO_SMAN