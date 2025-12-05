#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define intervalcount 200
#define ingredientcount 1100

typedef struct {
  long from;
  long to;
} interval;

long part1(interval intervals[], int intervallen, long ingredients[],
           int ingredientlen) {
  int sum = 0;
  for (int i = 0; i < ingredientlen; i++) {
    long ing = ingredients[i];

    for (int j = 0; j < intervallen; j++) {
      interval interval = intervals[j];
      if (interval.from <= ing && interval.to >= ing) {
        sum++;
        break;
      }
    }
  }
  return sum;
}

int comp(const void *a, const void *b) {
  interval *i1 = (interval *)a;
  interval *i2 = (interval *)b;

  if (i1->from > i2->from) {
    return 1;
  }
  if (i2->from > i1->from) {
    return -1;
  }
  return 0;
}

long max(long a, long b) {
  if (a > b) {
    return a;
  } else {
    return b;
  }
}

long part2(interval intervals[], int intervallen, long ingredients[],
           int ingredientlen) {
  qsort(intervals, intervallen, sizeof(interval), comp);
  long sum = 0;

  long largestTo = -1;
  for (int j = 0; j < intervallen; j++) {
    interval i1 = intervals[j];
    if (i1.to >= largestTo) {
      long t = max(largestTo, i1.from);
      long extra = i1.to - t + 1;
      largestTo = max(largestTo, i1.to) + 1;
      sum += extra;
    }
  }
  return sum;
}

int main(int argc, char *argv[]) {
  int isPart2 = !strcmp(argv[2], "part2");

  printf("\nStarting %s, %s \n", argv[1], argv[2]);
  FILE *file = fopen(argv[1], "r");
  char buffer[50];

  if (file == NULL) {
    printf("Failed to open file.\n");
    return 1;
  }

  int intervalpointer = 0;
  int ingredientpointer = 0;

  interval intervals[intervalcount] = {0};
  long ingredients[ingredientcount] = {0};
  char parsingIntervals = 1;

  int j = 0;
  while (fgets(buffer, sizeof(buffer), file) != NULL) {
    int len = strlen(buffer) - 1;
    if (len < 1 && parsingIntervals) {
      parsingIntervals = 0;
    } else {
      if (parsingIntervals) {
        long int f;
        long int t;
        sscanf(buffer, "%ld-%ld", &f, &t);
        intervals[intervalpointer].from = f;
        intervals[intervalpointer].to = t;
        intervalpointer++;
      } else {
        long int v;
        sscanf(buffer, "%ld", &v);
        ingredients[ingredientpointer] = v;
        ingredientpointer++;
      }
    }
  }

  long sum =
      isPart2
          ? part2(intervals, intervalpointer, ingredients, ingredientpointer)
          : part1(intervals, intervalpointer, ingredients, ingredientpointer);

  printf("Result: %ld \n", sum);

  if (ferror(file)) {
    printf("An error occurred.\n");
  }

  fclose(file);
  return 0;
}
