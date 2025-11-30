#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdio.h>
#include <string.h>

#include "input.h"
#include "stats.h"
#include "analysis.h"

// print bar chart of top N words into the analysis report,txt
void writebarchart(FILE *ptr, struct WordStructure *instance, char str[], int *linecount)
{
    int height = instance[0].frequency;

    fprintf(ptr, "\nBar Chart of Top %d %s:\n\n", N, str);
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
                        fprintf(ptr, " ");
                    }
                    fprintf(ptr, " * ");
                    for (int k = 0; k < (strlen(instance[j].word) / 2); k++)
                    {
                        fprintf(ptr, " ");
                    }
                }
                else
                {
                    for (int k = 0; k < (strlen(instance[j].word) / 2); k++)
                    {
                        fprintf(ptr, " ");
                    }
                    fprintf(ptr, " * ");
                    for (int k = 0; k < (strlen(instance[j].word) / 2); k++)
                    {
                        fprintf(ptr, " ");
                    }
                }
            }
        }
        fprintf(ptr, "\n");
    }

    for (int i = 0; i < N; i++)
    {
        fprintf(ptr, " %s ", instance[i].word);
    }
    fprintf(ptr, "\n\n");
}

// create and write summary to analysis report.txt
void savesummary()
{

    FILE *summary_ptr;

    summary_ptr = fopen("analysis_report.txt", "w");

    if (summary_ptr == NULL)
    {
        perror("Error creating and writing file!");
        return;
    }

    else
    {

        fprintf(summary_ptr,
                "\n   Toxic Word                                 Total Words Count : %d                      Unique Word\n\n"
                "Total toxic words : %d                         Number of sentences: %d                   Total Unique Words: %d\n\n"
                "Percentage of toxic words: %.2f%%          Average Sentence Length: %.2f               Lexical Diversity Index : %.2f\n\n\n",
                totalwords,
                totaltoxicwords, totalsentences, totaluniquewords,
                toxicWordPercentage, averageSentenceLength, lexicalDiversityIndex);

        if (ferror(summary_ptr) != 0)
        {
            printf("Error saving file!\n");
            return;
        }

        fprintf(summary_ptr, "Top %d Toxic Words & Unique Words\n\n", N);

        if (ferror(summary_ptr) != 0)
        {
            printf("Error saving file!\n");
            return;
        }

        if (twline < N || wline < N)
        {
            fprintf(summary_ptr, "Not enough words to display top %d list.\n", N);

            if (ferror(summary_ptr) != 0)
            {
                printf("Error saving file!\n");
            }
        }

        else
        {
            for (int i = 0, j = 0; i < N; i++, j++)
            {
                fprintf(summary_ptr, "%-*s: %-*d%-*s: %d \n\n", 15, TWStruct[i].word, 5, TWStruct[i].frequency, 15, WStruct[j].word, WStruct[j].frequency);
            }
        }

        writebarchart(summary_ptr, TWStruct, "Toxic Words", &twline);
        writebarchart(summary_ptr, WStruct, "Words", &wline);

        printf("Analysis report file saved successfully!\n");

        fclose(summary_ptr);
    }
}

#endif