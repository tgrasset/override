#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ptrace.h>
#include <sys/types.h>

int main(int argc, const char **argv, const char **envp)
{
  int stat_loc; 
  char s[128]; 
  int v6;
  int v7;
  int v8;
  pid_t pid;

  pid = fork();
  memset(s, 0, sizeof(s));
  v8 = 0;
  stat_loc = 0;
  if ( pid )
  {
    do
    {
      wait(&stat_loc);
      v6 = stat_loc;
      if ( (stat_loc & 0x7F) == 0 || (v7 = stat_loc, (char)((stat_loc & 0x7F) + 1) >> 1 > 0) )
      {
        puts("child is exiting...");
        return 0;
      }
      v8 = ptrace(PTRACE_PEEKUSER, pid, 44, 0);
    }
    while ( v8 != 11 );
    puts("no exec() for you");
    kill(pid, 9);
  }
  else
  {
    prctl(1, 1);
    ptrace(PTRACE_TRACEME, 0, 0, 0);
    puts("Give me some shellcode, k");
    gets(s);
  }
  return 0;
}