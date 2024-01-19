#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, const char **argv, const char **envp)
{
  char password[96]; 
  int clue1; 
  char ptr[48]; 
  char username[96]; 
  int clue2; 
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