# Level03

## Code
```
void decrypt(char key)
{
  char hash[] = "Q}|u`sfg~sf{}|a3";
  unsigned int len = strlen(hash);

  for (unsigned int  i = 0; i < len; ++i )
    hash[i] ^= key;
  if ( !strcmp(hash, "Congratulations!") )
    system("/bin/sh");
  else
    puts("\nInvalid Password");
}

void     test(int password, int n)
{
  char  r;
  int   key;

  key = n - password;
  switch ( key );
  {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
      decrypt(key);
      break;
    default:
      r = rand();
      decrypt(r);
      break;
  }
}

int     main(int argc, const char **argv)
{
  int password;

  unsigned int t = time(0);
  srand(t);
  puts("***********************************");
  puts("*\t\tlevel03\t\t**");
  puts("***********************************");
  printf("Password:");
  scanf("%d", &password);
  test(password, 322424845);
  return 0;
}
```

## Explanation

- In this binary, to win, we must pass the following condition of the function `decrypt()` :
```
if ( !strcmp(hash, "Congratulations!") )
    system("/bin/sh");
```
- `hash` is created by `"Q}|u'sfg~sf{}|a3" xor (password - 322424845)`
- So we must find what integer value `password` to give at `scanf("%d", &password);` so that : \
`(password - 322424845) xor "Q}|u'sfg~sf{}|a3" == "Congratulations!"`
- We created a program that check every number `n` from 1 to 21 to see if it verify :
`n xor "Q}|u'sfg~sf{}|a3" == "Congratulations!"`
```
#include <string.h>
#include <stdio.h>

int main(void)
{
  for (unsigned int key = 1; key <= 21; key++)
  {
    char v4[] = "Q}|u`sfg~sf{}|a3";

    unsigned int len = strlen(v4);
    for ( int i = 0; i < len; ++i )
        v4[i] ^= key;
    if ( !strcmp(v4, "Congratulations!") )
    {
        printf("%d\n", 322424845 - key);
        return (0);
    }
  }
  return (1);
}
```
- When using a xor on each char of the string `Q}|u'sfg~sf{}|a3` with the value 18 we get : `Congratulations!`
- So we have to give `322424845 - 18` => `322424827` as a password.



# Exploit

```
level03@OverRide:~$ ./level03
***********************************
*        level03        **
***********************************
Password:322424827
$ whoami
level04
$ cat /home/users/level04/.pass
```