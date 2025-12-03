#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static long sum = 0;

long count(int digits[], char picked[], int len) {
  long mult = 1;
  long sum = 0;
  for (int i = len - 1; i >= 0; i--) {
    if (picked[i]) {
      sum += digits[i] * mult;
      mult *= 10;
    }
  }
  return sum;
}

long solve(int digits[], int len, int topick) {
  char picked[len];
  for (int o = 0; o < len; o++) {
    picked[o] = 0;
  }

  long max = 0;
  for (int i = 0; i < topick; i++) {
    int maxIdx = 0;

    for (int o = 0; o < len; o++) {
      if (!picked[o]) {
        picked[o] = 1;
        long c = count(digits, picked, len);
        if (c > max) {
          max = c;
          maxIdx = o;
        }
        picked[o] = 0;
      }
    }
    picked[maxIdx] = 1;
  }

  return max;
}

void part1(int digits[], int len) {
  sum += solve(digits, len, 2);
}

void part2(int digits[], int len) {
  sum += solve(digits, len, 12);
}

void foreachline(char *line, void (*calc)(int[], int)) {
  int len = strlen(line) - 1;
  int digits[len];
  for (int i = 0; i < len; i++) {
    digits[i] = line[i] - 48;
  }

  calc(digits, len);
}

int main(int argc, char *argv[]) {
  int isPart2 = !strcmp(argv[2], "part2");

  printf("\nStarting %s, %s \n", argv[1], argv[2]);
  FILE *file = fopen(argv[1], "r");
  char buffer[200];

  if (file == NULL) {
    printf("Failed to open file.\n");
    return 1;
  }

  while (fgets(buffer, sizeof(buffer), file) != NULL) {
    if (isPart2) {
      foreachline(buffer, part2);
    } else {
      foreachline(buffer, part1);
    }
  }
    printf("Result: %ld \n", sum);

  if (ferror(file)) {
    printf("An error occurred.\n");
  }

  fclose(file);
  return 0;
}
