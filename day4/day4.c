#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define dim 140

int toidx(int i, int j) { return i + j * dim; }

int part1(char input[], char adj[]) {
  int sum = 0;
  for (int j = 1; j < dim - 1; j++) {
    for (int i = 1; i < dim - 1; i++) {
      int idx = toidx(i, j);
      if (input[idx] && adj[idx] < 5) {
        sum++;
      }
    }
  }
  return sum;
}

int part2(char input[], char adj[]) {
  // return 0;
  int sum = 0;
  while (1) {
    char madeChanges = 0;

    for (int j = 1; j < dim - 1; j++) {
      for (int i = 1; i < dim - 1; i++) {
        int idx = toidx(i, j);
        if (input[idx] && adj[idx] < 5) {
          input[idx] = 0;
          sum++;
          madeChanges = 1;

          for (int di = -1; di < 2; di++) {
            for (int dj = -1; dj < 2; dj++) {
              int adjidx = toidx(i + di, j + dj);
              adj[adjidx] -= 1;
            }
          }
        }
      }
    }
    if (!madeChanges) {
      break;
    }
  }
  return sum;
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

  char input[dim * dim] = {0};
  char adj[dim * dim] = {0};

  int j = 0;
  while (fgets(buffer, sizeof(buffer), file) != NULL) {
    int len = strlen(buffer) - 1;
    for (int i = 0; i < len; i++) {
      int idx = toidx(i + 1, j + 1);
      if (buffer[i] == '@') {
        input[idx] = 1;
        for (int di = -1; di < 2; di++) {
          for (int dj = -1; dj < 2; dj++) {
            int adjidx = toidx(i + 1 + di, j + 1 + dj);
            adj[adjidx] += 1;
          }
        }
      } else {
        input[idx] = 0;
      }
    }

    j++;
  }
  int sum = isPart2 ? part2(input, adj) : part1(input, adj);

  printf("Result: %d \n", sum);

  if (ferror(file)) {
    printf("An error occurred.\n");
  }

  fclose(file);
  return 0;
}
