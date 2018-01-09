#include final.h

char buzz() {
  printf("SAMPLE QUESTION\n");
  char buf[256];
  fgets(buf, 256, stdin);
  printf("Your answer: ");
  fgets(buf, 256, stdin);
  return *buf;
}

int main() {
  printf("%s\n", buzz());
  
  return 1;
}
