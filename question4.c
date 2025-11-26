#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct
{
  int bookID;
  int popularity;
  int lastUsed;
} ShelfBook;

int searchBook(ShelfBook *rack, int total, int key)
{
  for (int i = 0; i < total; i++)
  {
    if (rack[i].bookID == key)
      return i;
  }
  return -1;
}

int getLeastRecent(ShelfBook *rack, int total)
{
  int idx = 0;
  int minTime = rack[0].lastUsed;

  for (int i = 1; i < total; i++)
  {
    if (rack[i].lastUsed < minTime)
    {
      minTime = rack[i].lastUsed;
      idx = i;
    }
  }
  return idx;
}

void addToShelf(ShelfBook *rack, int *total, int *timeCounter, int limit, int id, int score)
{
  int pos = searchBook(rack, *total, id);

  if (pos != -1)
  {
    rack[pos].popularity = score;
    rack[pos].lastUsed = ++(*timeCounter);
    return;
  }

  if (*total == limit)
  {
    int lru = getLeastRecent(rack, *total);
    for (int i = lru; i < *total - 1; i++)
      rack[i] = rack[i + 1];
    (*total)--;
  }

  rack[*total].bookID = id;
  rack[*total].popularity = score;
  rack[*total].lastUsed = ++(*timeCounter);
  (*total)++;
}

void accessShelf(ShelfBook *rack, int total, int *timeCounter, int id)
{
  int pos = searchBook(rack, total, id);

  if (pos == -1)
  {
    printf("-1\n");
    return;
  }

  printf("%d\n", rack[pos].popularity);
  rack[pos].lastUsed = ++(*timeCounter);
}

int main()
{
  int limit, queries;
  scanf("%d %d", &limit, &queries);

  ShelfBook *rack = (ShelfBook *)malloc(limit * sizeof(ShelfBook));

  int total = 0;
  int timeCounter = 0;

  for (int i = 0; i < queries; i++)
  {
    char command[10];
    scanf("%s", command);

    if (strcmp(command, "ADD") == 0)
    {
      int id, score;
      scanf("%d %d", &id, &score);
      addToShelf(rack, &total, &timeCounter, limit, id, score);
    }
    else if (strcmp(command, "ACCESS") == 0)
    {
      int id;
      scanf("%d", &id);
      accessShelf(rack, total, &timeCounter, id);
    }
  }

  free(rack);
  return 0;
}