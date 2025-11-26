#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef struct
{
    char **lines;
    int count;
    int capacity;
} TextBuffer;

void growBuffer(TextBuffer *buf)
{
    int newCap = (buf->capacity == 0) ? 8 : buf->capacity * 2;
    char **temp = (char **)realloc(buf->lines, newCap * sizeof(char *));
    if (!temp)
    {
        fprintf(stderr, "ERROR: realloc failed - Out of memory!\n");
        exit(1);
    }
    buf->lines = temp;
    buf->capacity = newCap;
}

void shrinkToFit(TextBuffer *buf)
{
    if (buf->count == buf->capacity)
        return;
    if (buf->count == 0)
    {
        free(buf->lines);
        buf->lines = NULL;
        buf->capacity = 0;
        return;
    }
    char **temp = (char **)realloc(buf->lines, buf->count * sizeof(char *));
    if (!temp)
    {
        fprintf(stderr, "Warning: shrinkToFit failed, keeping old size\n");
        return;
    }
    buf->lines = temp;
    buf->capacity = buf->count;
}

void insertLine(TextBuffer *buf, int index, const char *text)
{
    if (index < 0 || index > buf->count)
    {
        printf("Invalid index!\n");
        return;
    }
    if (buf->count == buf->capacity)
        growBuffer(buf);

    for (int i = buf->count; i > index; i--)
    {
        buf->lines[i] = buf->lines[i - 1];
    }

    int len = strlen(text);
    buf->lines[index] = (char *)malloc(len + 1);
    if (!buf->lines[index])
    {
        fprintf(stderr, "ERROR: malloc failed for new line!\n");
        exit(1);
    }
    strcpy(buf->lines[index], text);
    buf->count++;
}

void deleteLine(TextBuffer *buf, int index)
{
    if (index < 0 || index >= buf->count)
    {
        printf("Invalid index to delete!\n");
        return;
    }
    free(buf->lines[index]);

    memmove(&buf->lines[index], &buf->lines[index + 1],
            (buf->count - index - 1) * sizeof(char *));

    buf->count--;

    if (buf->count > 0 && buf->count == buf->capacity / 4)
    {
        shrinkToFit(buf);
    }
}

void printAllLines(TextBuffer *buf)
{
    printf("\n--- Current Buffer (%d lines) ---\n", buf->count);
    for (int i = 0; i < buf->count; i++)
    {
        printf("%d: %s\n", i, buf->lines[i]);
    }
    printf("--- End of Buffer ---\n\n");
}

void saveToFile(TextBuffer *buf, const char *filename)
{
    FILE *fp = fopen(filename, "w");
    if (!fp)
    {
        fprintf(stderr, "ERROR: Cannot open file %s for writing!\n", filename);
        return;
    }
    for (int i = 0; i < buf->count; i++)
    {
        fprintf(fp, "%s\n", buf->lines[i]);
    }
    fclose(fp);
    printf("Buffer saved to %s\n", filename);
}

void loadFromFile(TextBuffer *buf, const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (!fp)
    {
        printf("File %s not found. Starting with empty buffer.\n", filename);
        return;
    }

    char line[1024];
    while (fgets(line, sizeof(line), fp))
    {
        line[strcspn(line, "\n")] = 0;
        insertLine(buf, buf->count, line);
    }
    fclose(fp);
    printf("Loaded %d lines from %s\n", buf->count, filename);
}

void freeAll(TextBuffer *buf)
{
    for (int i = 0; i < buf->count; i++)
    {
        free(buf->lines[i]);
    }
    free(buf->lines);
    buf->lines = NULL;
    buf->count = 0;
    buf->capacity = 0;
}

int main()
{
    TextBuffer buffer = {NULL, 0, 0};

    printf("Minimal Line-Based Text Editor\n");
    printf("Commands: insert <index> <text> | delete <index> | print | save <file> | load <file> | shrink | quit\n");

    loadFromFile(&buffer, "notes.txt");

    char cmd[20];
    while (1)
    {
        printf("> ");
        if (!fgets(cmd, sizeof(cmd), stdin))
            break;
        cmd[strcspn(cmd, "\n")] = 0;

        if (strcmp(cmd, "quit") == 0)
            break;
        else if (strcmp(cmd, "print") == 0)
            printAllLines(&buffer);
        else if (strcmp(cmd, "shrink") == 0)
        {
            shrinkToFit(&buffer);
            printf("Shrunk to fit: capacity = %d\n", buffer.capacity);
        }
        else if (strncmp(cmd, "save ", 5) == 0)
        {
            saveToFile(&buffer, cmd + 5);
        }
        else if (strncmp(cmd, "load ", 5) == 0)
        {
            freeAll(&buffer);
            loadFromFile(&buffer, cmd + 5);
        }
        else if (strncmp(cmd, "insert ", 7) == 0)
        {
            int idx;
            char text[1000];
            if (sscanf(cmd + 7, "%d %999[^\n]", &idx, text) == 2)
            {
                insertLine(&buffer, idx, text);
            }
        }
        else if (strncmp(cmd, "delete ", 7) == 0)
        {
            int idx;
            if (sscanf(cmd + 7, "%d", &idx) == 1)
            {
                deleteLine(&buffer, idx);
            }
        }
    }

    saveToFile(&buffer, "Q5_notes.txt");
    freeAll(&buffer);
    printf("Goodbye!\n");
    return 0;

}

/*
    Why Dynamic Allocation is better than fixed arrays:
    - Fixed array like char lines[1000][1000] wastes 1MB even if user types 1 line
    - Our method uses only ~100 bytes for 10 short lines
    - Can handle 100,000+ lines if needed
    - shrinkToFit removes waste after deletions
*/
