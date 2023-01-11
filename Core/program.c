#include "program.h"

int programCount;
program_t *programs;

thread_t* threads;
int threadCount;

void registerPrograms(program_t *progs, int length)
{
    programCount = length;
    programs = progs;
}

HANDLE_t startProgram(static_id_param_t programName)
{
    for (int i = 0; i < programCount; i++)
    {
        if(mto_sidcmp(&(programs[i].name.name[0]), programName))
        {
            programs[i].entry_ptr();
            break;
        }
    }
    
    return NULL_HANDLE;
}

void resume(void)
{
    //save params before stack switching
    p->c = c;
    p->f = f;
    p->arg = arg;
    get_sp(p->old_sp);
    get_fp(p->old_fp);

    set_sp(p - FRAME_SZ);
    set_fp(p); 
    get_fp(p);

  //and now we read our params from p
  if(!setjmp(p->c->callee_context)) {
    set_sp(p->old_sp);
    set_fp(p->old_fp);
    return;
  }
  (*p->f)(p->arg);
  longjmp(p->c->caller_context, 0);
}

void yield(void)
{
  (*p->f)(p->arg);
  longjmp(p->c->caller_context, 0);
}
