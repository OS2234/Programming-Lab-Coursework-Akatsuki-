#ifndef MYHASHMAP_H
#define MYHASHMAP_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// hashmap structure
struct hashMap
{
    int capacity;
    struct node **arr;
};

// node format
struct node
{
    char *key;
    int frequency;
    int index;
    struct node *next;
};

int malloc_fail_flag = 0;

// set attributes in a node
void setNode(struct node *node, char *key, int init_frequency, int index)
{
    node->key = strdup(key);
    if (node->key == NULL)
    {
        printf("Memory allocation for %s word in hashmap failed", key);
        getch();
        exit(1);
    }
    node->frequency = init_frequency;
    node->index = index;
    node->next = NULL;
}

// Iitialize and allocate memory space for hashmap of a defined capacity
void initializeHashMap(struct hashMap *mp, int capacity)
{
    mp->capacity = capacity;
    mp->arr = (struct node **)malloc(sizeof(struct node *) * mp->capacity);

    if (mp->arr == NULL)
    {
        printf("Error: Memory Allocation for hashmap failed.");
        getch();
        exit(1);
    }

    for (int i = 0; i < mp->capacity; i++)
        mp->arr[i] = NULL;
}

// hashfunction to determine location of stored word in the hashmap
// returns assigned bucket index location of word
int hashFunction(struct hashMap *mp, char *key)
{
    int sum = 0, factor = 31;
    for (int i = 0; key[i]; i++)
    {
        sum = ((sum % mp->capacity) + ((key[i] * factor) % mp->capacity)) % mp->capacity;
        factor = ((factor * 31) * (factor * 13)) % mp->capacity;
    }
    return sum;
}

// store word in hashmap with set attributes
void insert(struct hashMap *mp, char *key, int init_frequency, int index)
{
    int bucketIndex = hashFunction(mp, key);
    struct node *newNode = (struct node *)malloc(sizeof(struct node));
    if (newNode == NULL)
    {
        printf("Memory Allocation for new node failed");
        getch();
        exit(1);
    }

    setNode(newNode, key, init_frequency, index);

    newNode->next = mp->arr[bucketIndex];
    mp->arr[bucketIndex] = newNode;
}

// delete a specific node in the hashmap and free its memory
void delete_node(struct hashMap *mp, char *key)
{
    int bucketIndex = hashFunction(mp, key);
    struct node *prev = NULL;
    struct node *curr = mp->arr[bucketIndex];

    while (curr)
    {
        if (strcmp(curr->key, key) == 0)
        {
            if (prev)
                prev->next = curr->next;
            else
                mp->arr[bucketIndex] = curr->next;

            free(curr->key);
            free(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

// search a specific word in hashmap
// returns the word if found
// returns NULL is not found
char *search(struct hashMap *mp, char *key)
{
    int bucketIndex = hashFunction(mp, key);
    struct node *bucketHead = mp->arr[bucketIndex];

    while (bucketHead)
    {
        if (strcmp(bucketHead->key, key) == 0)
        {
            return bucketHead->key;
        }
        bucketHead = bucketHead->next;
    }

    return NULL;
}

// search for the word in hashmap
// return its index pointer in the word array structure
int searchIndex(struct hashMap *mp, char *key)
{
    int bucketIndex = hashFunction(mp, key);
    struct node *bucketHead = mp->arr[bucketIndex];

    while (bucketHead)
    {
        if (strcmp(bucketHead->key, key) == 0)
        {
            return bucketHead->index;
        }
        bucketHead = bucketHead->next;
    }

    return -1;
}

// seach for a word in the hashmap
// prints out the word and its frequency
// If not found, prints "Key is not found"
void findhash(struct hashMap *mp, char *key)
{
    int bucketIndex = hashFunction(mp, key);
    struct node *bucketHead = mp->arr[bucketIndex];

    while (bucketHead)
    {
        if (strcmp(bucketHead->key, key) == 0)
        {
            printf("Key :%s\n"
                   "Frequency :%d\n\n",
                   bucketHead->key, bucketHead->frequency);
            return;
        }
        bucketHead = bucketHead->next;
    }

    printf("Key: %s not found in hashmap.\n\n", key);
}

// clears hashmap and frees all allocated memory space
void clearHashMap(struct hashMap *mp)
{
    for (int i = 0; i < mp->capacity; i++)
    {
        struct node *curr = mp->arr[i];
        while (curr != NULL)
        {
            struct node *temp = curr;
            curr = curr->next;

            free(temp->key);

            free(temp);
        }

        mp->arr[i] = NULL;
    }
}

#endif