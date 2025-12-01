#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "myhashmap.h"
#include "input.h"
#include "analysis.h"

// detects any weird encodings in words
// handle cases of words containing apostrophe(') and dash (-)
int is_Word(char str[])
{
    for (int i = 0; i < strlen(str); i++)
    {
        if (isalpha(str[i]) == 0)
        {
            if (str[i] == '-' || str[i] == '\'')
            {
                if (isalpha(str[i - 1]) && isalpha(str[i + 1]))
                {
                    continue;
                }
            }
            return 0;
        }
    }
    return 1;
}

// swap position of words in word array structure and update their indexes
void swapWords(struct hashMap *mp, struct WordStructure *a, struct WordStructure *b)
{
    int temp_a = 0;
    int temp_b = 0;

    int bucketIndex1 = hashFunction(mp, a->word);
    struct node *bucketHead1 = mp->arr[bucketIndex1];

    while (bucketHead1)
    {
        if (strcmp(bucketHead1->key, a->word) == 0)
        {
            temp_a = bucketHead1->index;
        }
        bucketHead1 = bucketHead1->next;
    }

    int bucketIndex2 = hashFunction(mp, b->word);
    struct node *bucketHead2 = mp->arr[bucketIndex2];

    while (bucketHead2)
    {
        if (strcmp(bucketHead2->key, b->word) == 0)
        {
            temp_b = bucketHead2->index;
            bucketHead2->index = temp_a;
        }
        bucketHead2 = bucketHead2->next;
    }

    bucketHead1 = mp->arr[bucketIndex1];

    while (bucketHead1)
    {
        if (strcmp(bucketHead1->key, a->word) == 0)
        {
            bucketHead1->index = temp_b;
        }
        bucketHead1 = bucketHead1->next;
    }

    struct WordStructure temp = *a;
    *a = *b;
    *b = temp;
}

// sort words in alphabetical order in the array structure
void sortWordsByAlpha(struct WordStructure *item, struct hashMap *mp, int linecount)
{
    for (int i = 0; i < linecount; i++)
    {
        for (int j = 0; j < linecount - i - 1; j++)
        {

            if ((item[j].word[0] > item[j + 1].word[0]))
            {
                swapWords(mp, &item[j], &item[j + 1]);
            }
        }
    }
}

// quicksort words by frequency in array structure
int partition(struct WordStructure *item, struct hashMap *mp, int low, int high)
{
    int p = item[high].frequency;
    int i = low - 1;

    for (int j = low; j <= high - 1; j++)
    {
        if (item[j].frequency > p)
        {
            i++;
            swapWords(mp, &item[i], &item[j]);
        }
    }
    swapWords(mp, &item[i + 1], &item[high]);

    return (i + 1);
}

void quicksort(struct WordStructure *item, struct hashMap *mp, int low, int high)
{
    if (low < high)
    {
        int pi = partition(item, mp, low, high);

        quicksort(item, mp, low, pi - 1);
        quicksort(item, mp, pi + 1, high);
    }
}

// reintitialize every member of word array structure to 0 or NULL(\0)
void resetStruct(struct WordStructure *instance, int count)
{
    for (int i = 0; i < count; i++)
    {
        memset(&instance[i], 0, sizeof(instance[0]));
    }
}

#endif