int main(void)

{
  char str[] = "fsociety\n";
  int           len;
  unsigned int  c;
  int           ret;
  
  len = strcspn(str,"\n");
  str[len] = '\0';
  len = strnlen(str, ' ');
  if ((int)len < 6) {
    ret = 1;
  }
  else {
    c = ((int)str[3] ^ 4919) + 6221293;
    for (int i = 0; i < len; i++) {
        if (str[i] < ' ') {
            return 1;
        }
        c = c + ((int)str[i] ^ c) % 1337;
    }
    printf("%d\n", c);
  }
  return 0;
}