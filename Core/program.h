#ifndef MTO_PROGRAM
#define MTO_PROGRAM

#include "mto.h"

#define get_sp(p) \
  asm volatile("movq %%rsp, %0" : "=r"(p))
#define get_fp(p) \
  asm volatile("movq %%rbp, %0" : "=r"(p))
#define set_sp(p) \
  asm volatile("movq %0, %%rsp" : : "r"(p))
#define set_fp(p) \
  asm volatile("movq %0, %%rbp" : : "r"(p))

typedef struct program_t
{
    static_id_t name;
    int (*entry_ptr)(void);

} program_t;

typedef struct thread_t
{
    int sp;
    void* stack;
    int memSize;

} thread_t;


void registerPrograms(program_t *programs, int length);
HANDLE_t startProgram(static_id_param_t programName);

void resume(void);
void yield(void);

#endif // !MTO_PROGRAM