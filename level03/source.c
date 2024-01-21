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