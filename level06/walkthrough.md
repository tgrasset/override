# Level06

## Code

```
#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <string.h>

int auth(char *login,unsigned int serial)

{
  size_t loginLen;
  int uVar2;
  long lVar3;
  int i;
  unsigned int weird;
  
  loginLen = strcspn(login,"\n");
  login[loginLen] = '\0';
  loginLen = strnlen(login,32);
  if ((int)loginLen < 6) {
    uVar2 = 1;
  }
  else {
    lVar3 = ptrace(PTRACE_TRACEME);
    if (lVar3 == -1) {
      puts("\x1b[32m.---------------------------.");
      puts("\x1b[31m| !! TAMPERING DETECTED !!  |");
      puts("\x1b[32m\'---------------------------\'");
      uVar2 = 1;
    }
    else {
      weird = ((int)login[3] ^ 4919) + 6221293;
      for (i = 0; i < (int)loginLen; i = i + 1) {
        if (login[i] < ' ') {
          return 1;
        }
        weird = weird + ((int)login[i] ^ weird) % 1337;
      }
      if (serial == weird) {
        uVar2 = 0;
      }
      else {
        uVar2 = 1;
      }
    }
  }
  return uVar2;
}

int main(void)

{
  int iVar1;
  int in_GS_OFFSET;
  char buff[32];
  int serial;
  
  puts("***********************************");
  puts("*\t\tlevel06\t\t  *");
  puts("***********************************");
  printf("-> Enter Login: ");
  fgets(buff,32,stdin);
  puts("***********************************");
  puts("***** NEW ACCOUNT DETECTED ********");
  puts("***********************************");
  printf("-> Enter Serial: ");
  scanf("%d", &serial);
  iVar1 = auth(buff,serial);
  if (iVar1 == 0) {
    puts("Authenticated!");
    system("/bin/sh");
  }
  return iVar1 != 0;
}
```
## Explanation

- This program takes a login with `fgets()` and then a serial number with `scanf()`, and then calls its `auth()` function to compare both and see if the login corresponds to the serial number after having been transformed into a specific number by a very specific but hard coded and easy to reproduce algorithm.

- So all we had to do was to make a copy of the `auth()` function with a small difference with the original one : our version prints the final value obtained after the algorith, which gives us a a login/serial pair that we can use with the original binary.

- By compiling the program given in the `resources` folder, we associated the string "fsociety" with the serial 6234483. Of course we could have done this with any other login string instead. Let's use this pair with our binary now.


## Exploit
```
level06@OverRide:~$ ./level06 
***********************************
*		level06		  *
***********************************
-> Enter Login: fsociety
***********************************
***** NEW ACCOUNT DETECTED ********
***********************************
-> Enter Serial: 6234483
Authenticated!
$ whoami
level07
$ cat /home/users/level07/.pass
GbcPDRgsFK77LNnnuh7QyFYA2942Gp8yKj9KrWD8

```