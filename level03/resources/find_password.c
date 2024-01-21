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