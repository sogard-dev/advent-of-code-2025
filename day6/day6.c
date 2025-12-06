#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PROBLEMS 1000
#define NUMBERS_IN_PROBLEM 1000

typedef struct {
  long int numbers[NUMBERS_IN_PROBLEM];
  int insidx;
  char op;
} problem;

long part1(problem *problems) {
  long sum = 0;
  for (int i = 0; i < PROBLEMS; i++) {
    problem *pr = &problems[i];
    if (pr->insidx) {
      char isMult = pr->op == '*';
      long psum = isMult ? 1 : 0;
      for (int j = 0; j < pr->insidx; j++) {
        if (isMult) {
          psum = psum * pr->numbers[j];
        } else {
          psum = psum + pr->numbers[j];
        }
      }
      sum += psum;
    }
  }
  return sum;
}

void remove_spaces(char *s) {
  char *d = s;
  do {
    while (*d == ' ') {
      ++d;
    }
  } while (*s++ = *d++);
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

  problem problems[PROBLEMS] = {0};
  long columns[5000] = {0};
  while (fgets(buffer, sizeof(buffer), file) != NULL) {
    if (isPart2) {
      if (buffer[0] == '*' || buffer[0] == '+') {
        int problemidx = -1;
        for (int i = 0; buffer[i] != '\0'; i++) {
          char c = buffer[i];
          if (c != ' ') {
            problemidx++;
            problems[problemidx].op = c;
          }
          if (columns[i] > 0) {
            problem *pr = &problems[problemidx];
            pr->numbers[pr->insidx] = columns[i];
            pr->insidx++;
          }
        }
      } else {
        for (int i = 0; buffer[i] != '\0'; i++) {
          char c = buffer[i];
          if (c != ' ') {
            int n = c - 48;
            columns[i] = columns[i] * 10 + n;
          }
        }
      }
    } else {
      if (buffer[0] == '*' || buffer[0] == '+') {
        remove_spaces(buffer);
        for (int i = 0; buffer[i] != '\0'; i++) {
          problem *pr = &problems[i];
          pr->op = buffer[i];
        }
      } else {
        char *q;
        char *p = buffer;
        for (int n = 0; n < PROBLEMS; n++) {
          long int num = strtold(p, &q);
          if (p == q) {
            break;
          }

          problem *pr = &problems[n];
          pr->numbers[pr->insidx] = num;
          pr->insidx++;

          p = q;
        }
      }
    }
  }

  long sum = part1(problems);
  printf("Result: %ld \n", sum);

  if (ferror(file)) {
    printf("An error occurred.\n");
  }

  fclose(file);
  return 0;
}
