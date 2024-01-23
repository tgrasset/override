#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

void clear_stdin(void)
{
  char  c = 0;
  
  while (c != -1)
  {
    c = getchar();
    if (c == '\n') {
      return;
    }
  }
  return;
}

int get_unum(void)
{
  int local_10[4];
  
  local_10[0] = 0;
  fflush(stdout);
  scanf("%u",local_10);
  clear_stdin();
  return local_10[0];
}

int store_number(int *integer)
{
  unsigned int n;
  unsigned int index;
  
  printf(" Number: ");
  n = get_unum();
  printf(" Index: ");
  index = get_unum();
  if ((index % 3 == 0) || (n >> 24 == 183)) {
    puts(" *** ERROR! ***");
    puts("   This index is reserved for wil!");
    puts(" *** ERROR! ***");
    return 1;
  }
  else {
    integer[index] = n;
    return 0;
  }
}

int read_number(int *integer)
{
  unsigned int n;
  
  printf(" Index: ");
  n = get_unum();
  printf(" Number at data[%u] is %u\n", n, integer[n]);
  return 0;
}


int main(int argc,char **argv,char **envp)
{
  int   read_ret = 0;
  char  buf[20] = {0};
  int   integer[100] = {0};

  for (char **i = argv; *i != (char *)0x0; i++) {
    memset(*i, 0, strlen(*i) - 1);
  }
  for (char **i = envp; *i != (char *)0x0; i++) {
    memset(*i, 0, strlen(*i) - 1);
  }
  puts(
      "----------------------------------------------------\n  Welcome to wil\'s crappy number storage service!   \n----------------------------------------------------\n Commands:                                          \n    store - store a number into the data storage    \n    read  - read a number from the data storage     \n    quit  - exit the program                        \n----------------------------------------------------\n   wil has reserved some storage :>                 \n----------------------------------------------------\n"
      );
  while (1) {
    printf("Input command: ");
    fgets(buf, 20, stdin);
    buf[strlen(buf) - 1] = 0;

    if (!strncmp(buf, "store", 5)) {
      read_ret = store_number(integer);
    }
    else if (!strncmp(buf, "read", 4)) {
      read_ret = read_number(integer);
    }
    else if (!strncmp(buf, "quit", 4)) {
      return 0;
    }
    if (read_ret == 0) {
      printf(" Completed %s command successfully\n", buf);
    }
    else {
      printf(" Failed to do %s command\n", buf);
    }
    memset(&buf, 0, 20);
  };
}