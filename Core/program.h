#ifndef MTO_PROGRAM
#define MTO_PROGRAM

#include "mto.h"

#include <setjmp.h>

#ifdef WIN32
#define get_sp(p) \
  asm volatile("movq %%rsp, %0" : "=r"(p))
#define get_fp(p) \
  asm volatile("movq %%rbp, %0" : "=r"(p))
#define set_sp(p) \
  asm volatile("movq %0, %%rsp" : : "r"(p))
#define set_fp(p) \
  asm volatile("movq %0, %%rbp" : : "r"(p))
#endif // !WIN32
#ifdef ARDUINO
#define get_sp(p) \
  asm volatile("MOV %0, sp" : "=r"(p))
#define set_sp(p) \
  asm volatile("MOV sp, %0" : : "r"(p))
#endif // !ARDUINO

#define MAX_THREADS 2

typedef enum thread_stats_t 
{
  TS_DONE = 0,
  TS_WAITING = 1,
  TS_RUNNING = 2,
} thread_stats_t;

typedef struct program_t
{
    static_id_t name;
    void (*entry_ptr)(void);

} program_t;

typedef struct thread_t
{
  jmp_buf callee_context;
  jmp_buf caller_context;

  void* sp;
  void* sep;
  void* stack;
  uint32_t stackSize;
  enum thread_stats_t status;
  void(*ep)(void);
} thread_t;

extern thread_t* ct;
extern thread_t threads[MAX_THREADS];

extern int programCount;
extern program_t *programs;

void registerPrograms(program_t *programs, int length);
HANDLE_t startProgram(static_id_param_t programName);

void update(void);

inline void thread_resume(void);

#define thread_yield \
  get_sp(ct->sep);\
  ct->stackSize = ((uint32_t)ct->sp - (uint32_t)ct->sep);\
  ct->stack = mto_alloc(ct->stack, ct->stackSize);\
  mto_memcpy(ct->stack, ct->sep, ct->stackSize);\
  if(!setjmp(ct->callee_context)) {\
    longjmp(ct->caller_context, TS_RUNNING);\
  }\
  else\
  {\
    mto_memcpy(ct->sep, ct->stack, ct->stackSize);\
  }\

#define thread_done \
  ct->status = TS_DONE;\
  longjmp(ct->caller_context, TS_DONE);\

#endif // !MTO_PROGRAM