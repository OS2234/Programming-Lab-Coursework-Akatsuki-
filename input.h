#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>
#include <string.h>

#include "myhashmap.h"

#define MAX_LINES 2048
#define MAX_SENTENCE_LENGTH 512
#define MAX_LEN 64
#define MAX_FILE_BYTE 1048576

// variables for tracking number of members in word array structure
int loadcount = 0;
int twline = 0;
int wline = 0;

#ifndef WORDSTRUCTURE_DEFINED
#define WORDSTRUCTURE_DEFINED

// word structure format
struct WordStructure
{
    char word[MAX_LEN];
    int frequency;
    char severity[9];
};

#endif

// toxic words and normal words array structure
struct WordStructure TWStruct[512];
struct WordStructure WStruct[MAX_LINES / 2];

// store lines read from user input text file into a buffer until the max_line of EOF is reached.
// handle edge cases (empty files, corrupted binary encodings in words, large files)
int store_line_in_buffer(const char *filename, char buffer[][MAX_SENTENCE_LENGTH], int max_lines)
{
    FILE *fileptr = fopen(filename, "r");

    if (fileptr == NULL)
    {
        perror("Error opening file\n\n");
        return -1;
    }

    fseek(fileptr, 0, SEEK_END);
    long size = ftell(fileptr);
    rewind(fileptr);

    if (size > MAX_FILE_BYTE)
    {
        printf("Error: File is too large (> 1 MB)\n\n");
        fclose(fileptr);
        return -2;
    }

    if (size == 0)
    {
        printf("Error: The file is empty\n\n");
        fclose(fileptr);
        return -3;
    }

    int i = 0;

    while (i < max_lines && fgets(buffer[i], MAX_SENTENCE_LENGTH, fileptr))
    {
        if (buffer[i][0] == '\0' || buffer[i][0] == '\n')
        {
            continue;
        }

        for (int j = 0; buffer[i][j]; j++)
        {
            unsigned char c = buffer[i][j];
            if (c < 32 && c != '\n' && c != '\t')
            {
                printf("Error: Possible binary/corrupted data on line %d\n\n", i + 1);
                fclose(fileptr);
                return -4;
            }
        }

        if (strcspn(buffer[i], "\n") == MAX_SENTENCE_LENGTH)
        {
            printf("Error: Line %d exceeds maximum length of %d characters.\n"
                   "Please shorten the sentence or divide into paragraphs and try again.\n\n",
                   i + 1, MAX_SENTENCE_LENGTH);
            fclose(fileptr);
            return -5;
        }

        buffer[i][strcspn(buffer[i], "\n")] = 0;
        i++;
    }

    fclose(fileptr);

    if (i == max_lines)
    {
        printf("Line limit (%d lines) reached\n", max_lines);
    }

    printf("File loaded successfully with %d lines.\n\n", i);

    return i;
}

// load toxic words into their word array structure and hashmap
void loadTWFileintoStructAndHash(const char *filename, struct WordStructure *instance, struct hashMap *mp, int *count)
{
    FILE *fileptr = fopen(filename, "r");
    if (fileptr == NULL)
    {
        printf("Failed to open toxic word file");
        getch();
        exit(1);
    }

    char buffer[MAX_LEN];
    (*count) = 0;

    fseek(fileptr, 0, SEEK_END);
    long size = ftell(fileptr);
    rewind(fileptr);

    if (size > MAX_FILE_BYTE)
    {
        printf("Error: Toxic Word File is too large (> 1 MB)\n\n");
        fclose(fileptr);
        getch();
        exit(1);
    }

    if (size == 0)
    {
        printf("Error: Toxic Word File is empty\n\n");
        fclose(fileptr);
        getch();
        exit(1);
    }

    while ((*count) < MAX_LINES && !feof(fileptr))
    {
        fgets(buffer, MAX_LEN, fileptr);
        if (buffer[0] == '\n' || buffer[0] == '\0')
        {
            continue;
        }
        buffer[strcspn(buffer, "\n")] = 0;
        for (int j = 0; j <= strlen(buffer); j++)
        {
            unsigned char c = buffer[j];
            if (c < 32 && c != '\n' && c != '\t' && c != '\0')
            {
                printf("Error: Possible binary/corrupted data on line %d\n\n", (*count) + 1);
                fclose(fileptr);
                getch();
                exit(1);
            }
        }
        strcpy(instance[*count].word, buffer);

        insert(mp, buffer, 0, *count);

        fgets(buffer, MAX_LEN, fileptr);
        buffer[strcspn(buffer, "\n")] = 0;
        for (int j = 0; buffer[j]; j++)
        {
            unsigned char c = buffer[j];
            if (c < 32 && c != '\n' && c != '\t' && c != '\0')
            {
                printf("Error: Possible binary/corrupted data on line %d\n\n", (*count) + 1);
                fclose(fileptr);
                getch();
                exit(1);
            }
        }
        strcpy(instance[*count].severity, buffer);

        (*count)++;
    }

    memset(buffer, 0, sizeof(buffer));
    fclose(fileptr);
    return;
}

// load stop words into their hashmap
void loadSWFileintoHash(const char *filename, struct hashMap *mp)
{
    FILE *fileptr = fopen(filename, "r");

    char buffer[MAX_LEN];
    int i = 0;
    while (fgets(buffer, MAX_LEN, fileptr) && !feof(fileptr))
    {
        buffer[strcspn(buffer, "\n")] = 0;
        for (int j = 0; buffer[j]; j++)
        {
            unsigned char c = buffer[j];
            if (c < 32 && c != '\n' && c != '\t' && c != '\0')
            {
                printf("Error: Possible binary/corrupted data on line %d\n\n", i + 1);
                fclose(fileptr);
                getch();
                exit(1);
            }
        }
        insert(mp, buffer, 1, i);
        i++;
    }
}

#endif