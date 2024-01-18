- This program asks for a password to launch a shell as user level01, we simply need to guess it.

- When disassembling it, we can see that it gets the user input with scanf, which formats the string and automatically interprets it as an int with specifier `%d`, and then it makes a comparison :
```
    0x080484e7 <+83>:	cmp    $0x149c,%eax
    0x080484ec <+88>:	jne    0x804850d <main+121>
```

- We can see that if the comparison between **0x149c** (5276 in decimal) and %**eax** (which contains the return value of scanf at this point in the program) doesn't return equal, the program will jump to an address which makes it miss the call to `system()` and display **"Invalid Password!"** instead.

- So all we have to do is give the right value as password, and then cat the .pass file in our new shell session !

***


