#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isRepeating(char *str, int strlen, int repeatlen) {
  if (strlen % repeatlen != 0) {
    return 0;
  }

  char a[repeatlen + 1];
  a[repeatlen] = '\0';
  strncpy(a, str, repeatlen);

  for (int j = repeatlen; j < strlen; j += repeatlen) {
    char b[repeatlen + 1];
    b[repeatlen] = '\0';
    strncpy(b, str + j, repeatlen);

    if (strstr(a, b) == NULL) {
      return 0;
    }
  }
  return 1;
}

long part2(long from, long to) {
  char c[20];
  long sum = 0;
  for (long n = from; n <= to; n++) {
    sprintf(c, "%ld", n);
    int flen = strlen(c);
    int keepRunning = 1;
    for (int r = 1; r <= flen / 2 && keepRunning; r++) {
      if (isRepeating(c, flen, r)) {
        sum += n;
        keepRunning = 0;
      }
    }
  }
  return sum;
}

long part1(long from, long to) {
  char c[20];
  long sum = 0;
  for (long n = from; n <= to; n++) {
    sprintf(c, "%ld", n);
    int flen = strlen(c);
    if (flen % 2 == 1) {
      continue;
    }
    int len = flen / 2;

    char a[len + 1];
    a[len] = '\0';
    char b[len + 1];
    b[len] = '\0';
    strncpy(a, c, len);
    strncpy(b, c + len, len);

    if (strstr(a, b) == NULL) {
      continue;
    }

    sum += n;
  }
  return sum;
}

void foreachline(char *line, long (*calc)(long, long)) {
  char *ptr = strtok(line, ",");
  long long sum = 0;
  while (ptr != NULL) {
    long a, b;
    sscanf(ptr, "%ld-%ld", &a, &b);

    sum += calc(a, b);

    ptr = strtok(NULL, ",");
  }
  printf("Invalid sum = %llu\n", sum);
}

int main(int argc, char *argv[]) {
  int isPart2 = !strcmp(argv[2], "part2");

  printf("\nStarting %s, %s \n", argv[1], argv[2]);
  FILE *file = fopen(argv[1], "r");
  char buffer[1000];

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

  if (ferror(file)) {
    printf("An error occurred.\n");
  }

  fclose(file);
  return 0;
}
