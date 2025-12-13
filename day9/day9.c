#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PAIRS 500
#define HASHBUCKETS 1000000

typedef struct {
  long x;
  long y;
} coord;

long absl(long a) {
  if (a < 0) {
    return -a;
  }
  return a;
}
long minl(long a, long b) {
  if (a < b) {
    return a;
  }
  return b;
}
long maxl(long a, long b) {
  if (a > b) {
    return a;
  }
  return b;
}

struct node {
  long x;
  long y;
  struct node *next;
};

typedef struct {
  struct node *nodes[HASHBUCKETS];
  int size;
  long minx;
  long maxx;
  long miny;
  long maxy;
} map;

map *newmap() {
  map *m = (map *)malloc(sizeof(map));
  m->size = 0;
  m->minx = LONG_MAX;
  m->miny = LONG_MAX;
  m->maxx = LONG_MIN;
  m->maxy = LONG_MIN;
  for (int i = 0; i < HASHBUCKETS; i++) {
    struct node *n = (struct node *)malloc(sizeof(struct node));
    n->x = -1;
    n->y = -1;
    n->next = NULL;
    m->nodes[i] = n;
  }

  return m;
}
char mapcontains(map *m, long x, long y) {
  int h = (x ^ y) % HASHBUCKETS;
  for (struct node *n = m->nodes[h]; n != NULL; n = n->next) {
    if (n->x == x && n->y == y) {
      return 1;
    }
  }
  return 0;
}

void mapput(map *m, long x, long y) {
  if (mapcontains(m, x, y)) {
    return;
  }
  m->size++;
  m->minx = minl(m->minx, x);
  m->maxx = maxl(m->maxx, x);
  m->miny = minl(m->miny, y);
  m->maxy = maxl(m->maxy, y);

  int h = (x ^ y) % HASHBUCKETS;
  struct node *e = m->nodes[h];
  if (e->x == -1) {
    e->x = x;
    e->y = y;
    return;
  }
  while (e->next != NULL) {
    e = e->next;
  }

  struct node *n = (struct node *)malloc(sizeof(struct node));
  n->next = NULL;
  n->x = x;
  n->y = y;
  e->next = n;
}

void insert(int i, int j, coord *p, map *m) {
  coord from = p[i];
  coord to = p[j];
  for (int y = minl(from.y, to.y); y <= maxl(from.y, to.y); y++) {
    for (int x = minl(from.x, to.x); x <= maxl(from.x, to.x); x++) {
      mapput(m, x, y);
    }
  }
}

void fillfence(int initx, int inity, map *green, map *fence) {
  coord direction[4] = {0};
  direction[0].x = 1;
  direction[1].x = -1;
  direction[2].y = 1;
  direction[3].y = -1;

  int x = initx;
  int y = inity;
  char changed = 1;
  while (changed) {
    changed = 0;
    for (int d = 0; !changed && d < 4; d++) {
      int nx = x + direction[d].x;
      int ny = y + direction[d].y;
      if (!mapcontains(fence, nx, ny) && !mapcontains(green, nx, ny)) {
        for (int dy = -1; !changed && dy < 2; dy++) {
          for (int dx = -1; !changed && dx < 2; dx++) {
            if (dx == 0 && dy == 0) {
              continue;
            }

            int gy = ny + dy;
            int gx = nx + dx;
            if (mapcontains(green, gx, gy)) {
              mapput(fence, nx, ny);
              x = nx;
              y = ny;
              changed = 1;
            }
          }
        }
      }
    }
  }
}

long part2(int pid, coord *p) {
  map *green = newmap();
  // Insert green paths
  for (int i = 0; i < pid - 1; i++) {
    insert(i, i + 1, p, green);
  }
  insert(pid - 1, 0, p, green);

  printf("Map info: X: %ld - %ld, Y: %ld - %ld, Size: %d\n", green->minx,
         green->maxx, green->miny, green->maxy, green->size);
  //  Fill the outside
  map *fence = newmap();
  // find first wall
  int dy = green->miny + (green->maxy - green->miny) / 2;
  int dx = -1;
  for (int x = green->minx - 1; x < green->maxx; x++) {
    if (mapcontains(green, x, dy)) {
      dx = x - 1;
      break;
    }
  }
  mapput(fence, dx, dy);
  fillfence(dx, dy, green, fence);
  printf("Fence info: X: %ld - %ld, Y: %ld - %ld, Size: %d\n", fence->minx,
         fence->maxx, fence->miny, fence->maxy, fence->size);

  // Find largest green rectangle
  long largest = 0;
  for (int i = 0; i < pid; i++) {
    for (int j = i + 1; j < pid; j++) {
      long width = absl(p[i].x - p[j].x) + 1;
      long height = absl(p[i].y - p[j].y) + 1;
      long area = width * height;
      char isvalid = 1;
      if (area < largest) {
        isvalid = 0;
      }

      if (isvalid) {
        long x1 = minl(p[i].x, p[j].x);
        long x2 = maxl(p[i].x, p[j].x);
        for (int x = x1; x <= x2; x++) {
          if (mapcontains(fence, x, p[i].y) || mapcontains(fence, x, p[j].y)) {
            isvalid = 0;
            break;
          }
        }
      }

      if (isvalid) {
        long y1 = minl(p[i].y, p[j].y);
        long y2 = maxl(p[i].y, p[j].y);
        for (int y = y1; y <= y2; y++) {
          if (mapcontains(fence, p[i].x, y) || mapcontains(fence, p[j].x, y)) {
            isvalid = 0;
            break;
          }
        }
      }
      if (isvalid) {
        if (area > largest) {
          largest = area;
        }
      }
    }
  }

  return largest;
}

long part1(int pid, coord *p) {
  long largest = 0;
  for (int i = 0; i < pid; i++) {
    for (int j = i + 1; j < pid; j++) {
      long width = absl(p[i].x - p[j].x) + 1;
      long height = absl(p[i].y - p[j].y) + 1;
      long area = width * height;
      if (area > largest) {
        largest = area;
      }
    }
  }
  return largest;
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

  int pid = 0;
  coord p[PAIRS] = {0};
  while (fgets(buffer, sizeof(buffer), file) != NULL) {
    int a, b;
    sscanf(buffer, "%d,%d", &a, &b);
    p[pid].x = a;
    p[pid].y = b;
    pid++;
  }

  long result = isPart2 ? part2(pid, p) : part1(pid, p);
  printf("Result: %ld \n", result);

  if (ferror(file)) {
    printf("An error occurred.\n");
  }

  fclose(file);
  return 0;
}
