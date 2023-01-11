#include "task.h"

#include "program.h"
#include "storage.h"
#include "power.h"

void scheduleTasks(void)
{
    uint16_t storageSize = 0;
    HANDLE_t tStorage = getStorage(ST_TASKS, &storageSize);

    task_def_t nextTask = { "NULLT", "NULLP", -1};

    do
    {
        static_id_t buffer[8];
        int sRead = read(tStorage, &buffer, 0, sizeof(buffer));

        for (int i = 0; i < sRead; i++)
        {
            HANDLE_t htask = getEntry(tStorage, buffer[i].name);
            task_def_t task;
            read(htask, &task, 0, sizeof(task));

            if(task.execIntervall < nextTask.execIntervall)
                nextTask = task;

            free_handle(htask);
        }
        
        storageSize -= sRead;
    } while(storageSize > 0);

    HANDLE_t tmpStor = getStorage(ST_TMP, NULL);
    HANDLE_t currTask = getEntry(tmpStor, CURR_TASK);
    if(currTask == NULL_HANDLE)
        currTask = addEntry(tmpStor, CURR_TASK);
    write(currTask, &nextTask, 0, sizeof(task_def_t));

    free_handle(tStorage);
}

void runInterrupt(void)
{
    powerUp();
    onTimerWakeup();

    HANDLE_t tmpStor = getStorage(ST_TMP, NULL);
    HANDLE_t currTask = getEntry(tmpStor, CURR_TASK);
    if(currTask != NULL_HANDLE)
    {
        task_def_t task;
        read(currTask, &task, 0, sizeof(task_def_t));
        startProgram(task.program.name);
    }

    scheduleTasks();
    powerDown();
}

int createTask(static_id_param_t name, static_id_param_t program, time_stamp_t intervall)
{
    uint16_t size = 0;
    HANDLE_t storage = getStorage(ST_TASKS, &size);
    HANDLE_t entry = addEntry(storage, name);

    task_def_t task = { name, program, intervall };
    write(entry, &task, 0, sizeof(task));

    free_handle(entry);
    free_handle(storage);

    return 0;
}

void deleteTask(static_id_param_t task)
{
    HANDLE_t storage = getStorage(ST_TASKS, NULL);
    removeEntry(storage, task);

    free_handle(storage);
}