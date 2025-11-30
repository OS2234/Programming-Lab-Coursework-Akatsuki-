#ifndef ANALYSIS_H
#define ANALYSIS_H

#include "input.h"
#include "myhashmap.h"
#include "stats.h"

#define n_gram_max 5

// initialize variables required for word analysis
int tw_mild = 0;
int tw_moderate = 0;
int tw_severe = 0;
int word_count = 0;
int toxic_score = 0;
int n_gram_count = 0;

// flag to be set when analysis is done
int analysis_flag = 0;

// search the toxic word hashmap for the n_gram word formed
//  if found, update its frequency
void update_n_grams(char str[], struct hashMap *mp, struct WordStructure *instance)
{
    int index = searchIndex(mp, str);
    if (index < 0)
        return;

    instance[index].frequency++;
    if (strcmp(instance[index].severity, "mild") == 0)
        tw_mild++;
    else if (strcmp(instance[index].severity, "moderate") == 0)
        tw_moderate++;
    else if (strcmp(instance[index].severity, "severe") == 0)
        tw_severe++;

    toxic_score = tw_mild * 1 + tw_moderate * 2 + tw_severe * 3;
}

// form combinations of n_gram words up to the max n_gram
// compare them to the toxic word list
// if match is found, update frequency accordingly
void analyze_n_gram(struct WordStructure *instance, struct hashMap *mp, char n_gram_buffer[][MAX_LEN], int n_gram_count)
{
    for (int n = 2; n <= n_gram_max; n++)
    {
        for (int j = 0; j <= (n_gram_count - n); j++)
        {
            char n_gram_combined[MAX_LEN] = "";

            for (int k = 0; k < n; k++)
            {
                if (k > 0)
                {
                    strcat(n_gram_combined, " ");
                }
                strcat(n_gram_combined, n_gram_buffer[j + k]);
            }

            char *result = search(mp, n_gram_combined);
            if (result != NULL)
            {
                update_n_grams(n_gram_combined, mp, instance);
            }
        }
    }
}

// count number of sentences for each line
// detects sentence ending punctuations and increment count
// handles edge cases (e.g. multiple punctuations in a row (...), \n with no sentence-ending punctuation (word\n) etc)
int countSentences(char text[])
{

    int let_num = strlen(text);
    for (int i = 0; text[i] != '\0'; i++)
    {
        if ((i > 0) && (text[i] == '.' || text[i] == '!' || text[i] == '?'))
        {
            if (i + 1 >= let_num)
            {
                if (isalnum(text[i - 1]))
                {
                    totalsentences++;
                    continue;
                }
            }

            if (isalnum(text[i - 1]) && isspace(text[i + 1]))
            {
                totalsentences++;
            }

            else if (isalnum(text[i - 1]) && ispunct(text[i + 1]))
            {
                totalsentences++;
            }
        }

        else if (i > 0 && text[i + 1] == '\0' && isalnum(text[i - 1]))
        {
            totalsentences++;
            continue;
        }
    }
    return totalsentences;
}

// update word frequency in word structure and hashmap when match is found
void update(struct WordStructure *instance, struct hashMap *mp, char *key)
{
    int bucketIndex = hashFunction(mp, key);
    struct node *bucketHead = mp->arr[bucketIndex];

    while (bucketHead)
    {
        if (strcmp(bucketHead->key, key) == 0)
        {
            bucketHead->frequency++;
            int idx = bucketHead->index;
            instance[idx].frequency++;

            if (strcmp(instance[idx].severity, "mild") == 0)
                tw_mild++;
            else if (strcmp(instance[idx].severity, "moderate") == 0)
                tw_moderate++;
            else if (strcmp(instance[idx].severity, "severe") == 0)
                tw_severe++;

            toxic_score = tw_mild * 1 + tw_moderate * 2 + tw_severe * 3;

            return;
        }
        bucketHead = bucketHead->next;
    }
}

#endif