#include <stdio.h>

int main(int argc, char *argv[])
{
  char buf[16];
  int val;
  int aim = 0;
  int depth = 0;
  int hDist = 0;

  while (scanf("%s %d", buf, &val) != EOF) {
    switch (buf[0]) {
      case 'f':
        hDist += val;
        depth += val * aim;
        break;
      case 'u':
        aim -= val;
        break;
      case 'd':
        aim += val;
        break;
      default:
        printf("bepsi\n");
    }
  }

  printf("Answer = %d\n", depth * hDist);

  return 0;
}
