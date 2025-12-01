#include <stdio.h>
#include <stdlib.h>
#include <string.h>:

void part1(char *line, int *crossesZero) {
  static int dial = 50;
  static int dialsize = 99;

  char *end;
  const long i = strtol(line + 1, &end, 10);
  if (line[0] == 'R') {
    dial += i;
  } else {
    dial -= i;
    dial += 100;
  }
  dial = dial % 100;
  if (dial == 0) {
    *crossesZero += 1;
  }
}

void part2(char *line, int *crossesZero) {
  static int dial = 50;
  static int dialsize = 99;

  char *end;
  const long i = strtol(line + 1, &end, 10);
  for (int l = 0; l < i; l++) {
    if (line[0] == 'R') {
      dial += 1;
    } else {
      dial -= 1;
      dial += 100;
    }
    dial = dial % 100;
    if (dial == 0) {
      *crossesZero += 1;
    }
  }
}

int main(int argc, char *argv[]) {
  printf("\nStarting %s, %s \n", argv[1], argv[2]);
  FILE *file = fopen(argv[1], "r");
  char buffer[20];

  char *arg2 = argv[2];

  int isPart2 = !strcmp(arg2, "part2");

  if (file == NULL) {
    printf("Failed to open file.\n");
    return 1;
  }

  int crossesZero = 0;
  while (fgets(buffer, sizeof(buffer), file) != NULL) {
    if (isPart2) {
      part2(buffer, &crossesZero);
    } else {
      part1(buffer, &crossesZero);
    }
  }
  printf("Crossed zero %d \n", crossesZero);

  if (feof(file)) {
    printf("End of file reached.\n");
  } else if (ferror(file)) {
    printf("An error occurred.\n");
  }

  fclose(file);
  return 0;
}
