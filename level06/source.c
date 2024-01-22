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