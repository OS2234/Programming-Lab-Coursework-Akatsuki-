#ifndef STATS_H
#define STATS_H

#include <stdio.h>
#include <string.h>

#include "utils.h"
#include "input.h"
#include "analysis.h"

#define N 3

// variables of word file statistics / general summary
int totalwords = 0;
int totaltoxicwords = 0;
int totaluniquewords = 0;
int totalsentences = 0;
float averageSentenceLength = 0.0;
float toxicWordPercentage = 0.0;
float lexicalDiversityIndex = 0.0;

// print vertical bar chart of top N words to terminal
void printbarchart(struct WordStructure *instance, char str[], int *linecount)
{
    int height = instance[0].frequency;

    printf("\nBar Chart of Top %d %s:\n\n", N, str);
    for (int i = height; i > 0; i--)
    {
        for (int j = 0; j < N; j++)
        {
            if (instance[j].frequency >= i)
            {
                if (strlen(instance[j].word) % 2 == 0)
                {
                    for (int k = 0; k < (strlen(instance[j].word) / 2) - 1; k++)
                    {
                        printf(" ");
                    }
                    printf(" * ");
                    for (int k = 0; k < (strlen(instance[j].word) / 2); k++)
                    {
                        printf(" ");
                    }
                }
                else
                {
                    for (int k = 0; k < (strlen(instance[j].word) / 2); k++)
                    {
                        printf(" ");
                    }
                    printf(" * ");
                    for (int k = 0; k < (strlen(instance[j].word) / 2); k++)
                    {
                        printf(" ");
                    }
                }
            }
        }
        printf("\n");
    }
    for (int i = 0; i < N; i++)
    {
        printf(" %s ", instance[i].word);
    }
    printf("\n\n");
}

// calculate and update statistics for the input file
void calculateStatistics()
{
    for (int i = 0; i < twline; i++)
    {
        totaltoxicwords += TWStruct[i].frequency;
    }

    for (int i = 0; i < wline; i++)
    {
        if (WStruct[i].frequency == 1)
        {
            totaluniquewords++;
        }
    }
    averageSentenceLength = (float)totalwords / (float)totalsentences;
    toxicWordPercentage = ((float)totaltoxicwords / (float)totalwords) * 100;
    lexicalDiversityIndex = (float)totaluniquewords / (float)totalwords;
}

// print summary of input file to terminal
void printSummary()
{
    printf(
        "\n   Toxic Word                                 Total Words Count : %d                      Unique Word\n\n"
        "Total toxic words : %d                         Number of sentences: %d                   Total Unique Words: %d\n\n"
        "Percentage of toxic words: %.2f%%          Average Sentence Length: %.2f               Lexical Diversity Index : %.2f\n\n\n",
        totalwords,
        totaltoxicwords, totalsentences, totaluniquewords,
        toxicWordPercentage, averageSentenceLength, lexicalDiversityIndex);

    printf("Top %d Toxic Words & Unique Words\n\n", N);

    if (twline < N || wline < N)
    {
        printf("Not enough words to display top %d list.\n", N);
        return;
    }

    else
    {
        for (int i = 0, j = 0; i < N; i++, j++)
        {
            printf("%-*s: %-*d%-*s: %d \n\n", 15, TWStruct[i].word, 5, TWStruct[i].frequency, 15, WStruct[j].word, WStruct[j].frequency);
        }
    }

    printbarchart(TWStruct, "Toxic Words", &twline);
    printbarchart(WStruct, "Words", &wline);
}

// display all toxic words and all unique words in their respective word array structures
void display_Word_Stat(struct WordStructure *instance, struct hashMap *mp, char word[])
{

    int bucketIndex = hashFunction(mp, word);
    struct node *bucketHead = mp->arr[bucketIndex];

    while (bucketHead)
    {
        if (strcmp(bucketHead->key, word) == 0)
        {
            int idx = bucketHead->index;
            char severity[9];
            strcpy(severity, instance[idx].severity);
            printf("Word : %s\n"
                   "Frequency : %d\n"
                   "Severity : %s\n",
                   bucketHead->key, bucketHead->frequency, severity);
            return;
        }
        bucketHead = bucketHead->next;
    }

    printf("\n\nWord '%s' not found in the provided hashmap.\n\n", word);
}

#endif