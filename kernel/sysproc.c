#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

#include "sysinfo.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

////////////////////////////
// The functions to retrieve system call arguments from user space are in kernel/syscall.c, and you can see examples of their use in kernel/sysproc.c.
uint64
sys_trace(void)
{
  // printf("sys_trace!!\n");
  //TODO
  int mask_num;
  struct proc *p = myproc();

  argint(0, &mask_num);
  p->mask = mask_num;
  // printf("mask: %d\n", mask_num);
  
  return 0;
}

uint64
sys_sysinfo(void) {
  struct sysinfo sysi;
  struct proc *p = myproc();
  uint64 si;  // user pointer to struct sysinfo
 
  argaddr(0, &si);
  /// 需要搞一个内核的 sysinfo : sysi
  /// 通过那两个函数赋值
  /// 然后用copyout传出去
  sysi.nproc = proccount();
  sysi.freemem = freemem();
  // printf("%d %d\n", sysi.nproc, sysi.freemem);
  if(copyout(p->pagetable, si, (char *)&sysi, sizeof(sysi)) < 0) {
    return -1;
  }

  return 0;
}