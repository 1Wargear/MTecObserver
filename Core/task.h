#ifndef MTO_TASK
#define MTO_TASK

#include "mto.h"
#include "time.h"

#define CURR_TASK "CTASK"

typedef struct task_def_t
{
    static_id_t name;
    static_id_t program;

    time_stamp_t execIntervall;
} task_def_t;

void scheduleTasks(void);
void runInterrupt(void);

int createTask(static_id_param_t name, static_id_param_t program, time_stamp_t intervall);
HANDLE_t getTask(static_id_t task);
void deleteTask(static_id_param_t task);

#endif // !MTO_TASK