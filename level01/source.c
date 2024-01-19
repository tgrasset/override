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