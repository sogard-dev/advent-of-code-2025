#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define WIDTH 1000

static long beams[WIDTH] = {0};

int part1(int i, char c) {
  static int splits = 0;
  if (c == 'S') {
    beams[i] = 1;
  } else if (c == '^' && beams[i]) {
    splits++;
    beams[i] = 0;
    beams[i - 1] = 1;
    beams[i + 1] = 1;
  }
  return splits;
}

void part2(int i, char c) {
  if (c == 'S') {
    beams[i] = 1;
  } else if (c == '^' && beams[i]) {
    beams[i - 1] += beams[i];
    beams[i + 1] += beams[i];
    beams[i] = 0;
  }
}

int main(int argc, char *argv[]) {
  int isPart2 = !strcmp(argv[2], "part2");

  printf("\nStarting %s, %s \n", argv[1], argv[2]);
  FILE *file = fopen(argv[1], "r");
  char buffer[5000];

  if (file == NULL) {
    printf("Failed to open file.\n");
    return 1;
  }

  long result = 0;
  while (fgets(buffer, sizeof(buffer), file) != NULL) {
    for (int i = 0; buffer[i] != '\n'; i++) {
      char c = buffer[i];
      if (isPart2) {
        part2(i, c);
      } else {
        result = part1(i, c);
      }
    }
  }
  if (isPart2) {
    for (int i = 0; i < WIDTH; i++) {
      result += beams[i];
    }
  }

  printf("Result: %ld \n", result);

  if (ferror(file)) {
    printf("An error occurred.\n");
  }

  fclose(file);
  return 0;
}
