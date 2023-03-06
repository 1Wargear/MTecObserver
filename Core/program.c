#include "program.h"

int programCount;
program_t *programs;

thread_t* ct;
thread_t threads[MAX_THREADS];

void registerPrograms(program_t *progs, int length)
{
  programCount = length;
  programs = progs;

  //Init Threads
  for(int i = 0; i < MAX_THREADS; i++)
  {
    threads[i].stack = NULL;
    threads[i].status = TS_DONE;
  }
}

HANDLE_t startProgram(static_id_param_t programName)
{
  program_t* program = NULL;
  for (int i = 0; i < programCount; i++)
  {
      if(mto_sidcmp(&(programs[i].name.cstr[0]), programName))
      {
          program = &programs[i];
          break;
      }
  }

  if(program == NULL)
    return NULL_HANDLE;

  thread_t* myThread = NULL;
  for(int i = 0; i < MAX_THREADS; i++)
  {
    if(threads[i].status == TS_DONE)
    {
      myThread = &threads[i];
      myThread->ep = program->entry_ptr;
      break;
    }
  }

  if(myThread == NULL)
    return NULL_HANDLE;

  if(!setjmp(myThread->callee_context))
  {
    myThread->status = TS_WAITING;
    return 1;
  }

  //----------------Current Thread----------------------------

  ct->ep();
  ct->status = TS_DONE;

  longjmp(ct->caller_context, TS_DONE);
    
  return NULL_HANDLE;
}

#include <LiquidCrystal_I2C.h>
extern LiquidCrystal_I2C lcd;

void update(void)
{
  for(int i = 0; i < MAX_THREADS; i++)
  {
    if(threads[i].status == TS_WAITING)
    {
      ct = &threads[i];
      ct->status = TS_RUNNING;
      thread_resume();

      if(ct->status == TS_RUNNING)
        ct->status = TS_WAITING;
    }
  }
}

inline void thread_resume(void)
{
  int ret = setjmp(ct->caller_context);
  if(!ret) 
  {
    get_sp(ct->sp);
    longjmp(ct->callee_context, 1);
  }
}
