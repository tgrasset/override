#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

__uint64_t log_wrapper(FILE *a1, const char *hardCoded, const char *argv1)
{
  char dest[264]; 
  __uint64_t v6; 

  v6 = __readfsqword(0x28u);
  strcpy(dest, hardCoded);
  snprintf(&dest[strlen(dest)], 254 - strlen(dest), argv1);
  dest[strcspn(dest, "\n")] = 0;
  fprintf(a1, "LOG: %s\n", dest);
  return __readfsqword(0x28u) ^ v6;
}

int  main(int argc, const char **argv, const char **envp)
{
  FILE *logFile; 
  FILE *stream; 
  int fd; 
  char buf; 
  char dest[104]; 

  buf = -1;
  if ( argc != 2 )
    printf("Usage: %s filename\n", *argv);
  logFile = fopen("./backups/.log", "w");
  if ( !logFile )
  {
    printf("ERROR: Failed to open %s\n", "./backups/.log");
    exit(1);
  }
  log_wrapper(logFile, "Starting back up: ", argv[1]);
  stream = fopen(argv[1], "r");
  if ( !stream )
  {
    printf("ERROR: Failed to open %s\n", argv[1]);
    exit(1);
  }
  strcpy(dest, "./backups/");
  strncat(dest, argv[1], 99 - strlen(dest));
  fd = open(dest, 193, 432LL);
  if ( fd < 0 )
  {
    printf("ERROR: Failed to open %s%s\n", "./backups/", argv[1]);
    exit(1);
  }
  while ( 1 )
  {
    buf = fgetc(stream);
    if ( buf == -1 )
      break;
    write(fd, &buf, 1uLL);
  }
  log_wrapper(logFile, "Finished back up ", argv[1]);
  fclose(stream);
  close(fd);
  return 0;
}