#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

#include "fcntl.h"
#include "x86.h"

#undef NULL
#define NULL ((void*)0)

#define PGSIZE (4096)

int ppid;
int global = 1;

#define assert(x) if (x) {} else { \
   printf(1, "%s: %d ", __FILE__, __LINE__); \
   printf(1, "assert failed (%s)\n", # x); \
   printf(1, "TEST FAILED\n"); \
   kill(ppid); \
   exit(); \
}

void worker(void *arg_ptr);

int
main(int argc, char *argv[])
{
   ppid = getpid();

   void *stack = malloc(PGSIZE*2);
   assert(stack != NULL);
   if((uint)stack % PGSIZE)
     stack = stack + (4096 - (uint)stack % PGSIZE);

   int arg = 42;
   int clone_pid = clone(worker, &arg, stack);
   assert(clone_pid > 0);

   void *join_stack;
   int join_pid = join(&join_stack);
   assert(join_pid == clone_pid);
   printf(1, "passed assret 1\n");

   printf(1, "%p\n", stack);
   printf(1, "%p\n", join_stack);
   assert(stack == join_stack);
   printf(1, "passed assert 2\n");
   assert(global == 2);

   printf(1, "TEST PASSED\n");
   exit();
}

void
worker(void *arg_ptr) {
   int arg = *(int*)arg_ptr;
   assert(arg == 42);
   assert(global == 1);
   global++;
   exit();
}

//void printstuff(void* str);

/*int main(int argc, char** argv) {
  // lock test
  lock_t *lock = malloc(sizeof(lock_t));
  lock_init(lock);
  lock_acquire(lock);
  int a = 1;
  lock_release(lock);
  printf(1, "a: %d\n", a);
  
  // clone test
  void *stack = malloc(4096);
  char* word = "word";
  char** arg;
  arg = &word;
  void (*fcn)(void*) = &printstuff;
  int pid =  clone(fcn, word, stack);
  printf(1, "pid: %d\n", pid);

  exit();
}

void printstuff(void* str) {
  printf(1, "%s\n", (char*)str);
}*/
