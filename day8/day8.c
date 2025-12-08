#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define JUNCTIONS 1000

typedef struct {
  int x;
  int y;
  int z;
  int cluster;
  char inuse;
} junction;

typedef struct {
  long distance;
  int a;
  int b;
} dist;

long distance(junction a, junction b) {
  long x = b.x - a.x;
  long y = b.y - a.y;
  long z = b.z - a.z;
  return x * x + y * y + z * z;
}

int cmp_int(const void *va, const void *vb) {
  int a = *(int *)va;
  int b = *(int *)vb;
  return a < b ? 1 : a > b ? -1 : 0;
}

int cmp_dist(const void *va, const void *vb) {
  dist *a = (dist *)va;
  dist *b = (dist *)vb;

  return a->distance < b->distance ? -1 : a->distance > b->distance ? 1 : 0;
}

long part1(junction *junctions, int pairs) {
  dist *distances = malloc(JUNCTIONS * JUNCTIONS * sizeof *distances);
  for (int i = 0; i < JUNCTIONS * JUNCTIONS; i++) {
    distances[i].distance = INT_MAX;
  }
  int didx = 0;
  for (int i = 0; junctions[i].inuse; i++) {
    for (int j = i + 1; junctions[j].inuse; j++) {
      distances[didx].distance = distance(junctions[i], junctions[j]);
      distances[didx].a = i;
      distances[didx].b = j;
      didx++;
    }
  }

  qsort(distances, JUNCTIONS * JUNCTIONS, sizeof distances[0], cmp_dist);

  int runfor = pairs == 0 ? JUNCTIONS * JUNCTIONS : pairs;
  int clusterid = 0;
  for (int p = 0; p < runfor; p++) {
    int mini = distances[p].a;
    int minj = distances[p].b;
    int icluster = junctions[mini].cluster;
    int jcluster = junctions[minj].cluster;
    if (icluster == jcluster && icluster > 0) {
      continue;
    }

    int ocluster;
    if (icluster && !jcluster) {
      ocluster = icluster;
    } else if (jcluster && !icluster) {
      ocluster = jcluster;
    } else if (icluster && jcluster) {
      for (int i = 0; junctions[i].inuse; i++) {
        if (junctions[i].cluster == jcluster) {
          junctions[i].cluster = icluster;
        }
      }
      ocluster = icluster;
    } else {
      ocluster = ++clusterid;
    }

    junctions[mini].cluster = ocluster;
    junctions[minj].cluster = ocluster;

    if (pairs == 0) {
      int incluster = -1;
      char keepgoing = 0;
      for (int j = 0; junctions[j].inuse; j++) {
        int c = junctions[j].cluster;
        if (c == 0) {
          keepgoing = 1;
          break;
        }

        if (incluster == -1) {
          incluster = c;
        }

        if (incluster != c) {
          keepgoing = 1;
        }
        if (keepgoing) {
          break;
        }
      }

      if (!keepgoing) {
        return junctions[mini].x * (long) junctions[minj].x;
      }
    }
  }

  int clusterlen = pairs + 1;
  int clustersizes[clusterlen];
  for (int j = 0; j < clusterlen; j++) {
    clustersizes[j] = 0;
  }

  for (int j = 0; junctions[j].inuse; j++) {
    if (junctions[j].cluster > 0) {
      clustersizes[junctions[j].cluster]++;
    }
  }
  qsort(clustersizes, clusterlen, sizeof clustersizes[0], cmp_int);

  return clustersizes[0] * clustersizes[1] * clustersizes[2];
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

  junction junctions[JUNCTIONS + 1] = {0};
  int jid = 0;
  while (fgets(buffer, sizeof(buffer), file) != NULL) {
    int x, y, z;
    sscanf(buffer, "%d,%d,%d", &x, &y, &z);
    junctions[jid].x = x;
    junctions[jid].y = y;
    junctions[jid].z = z;
    junctions[jid].inuse = 1;
    jid++;
  }

  long result = 0;
  if (isPart2) {
    result = part1(junctions, 0);
  } else {
    if (jid == 20) {
      result = part1(junctions, 10);
    } else {
      result = part1(junctions, 1000);
    }
  }

  printf("Result: %ld \n", result);

  if (ferror(file)) {
    printf("An error occurred.\n");
  }

  fclose(file);
  return 0;
}
