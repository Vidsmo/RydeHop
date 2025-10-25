#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "linked_list.h"

#define TABLE_SIZE 100

typedef struct 
{
    char *key;
    void *data;
} HashEntry;

typedef struct 
{
    LinkedList **buckets;
    int size;
} HashTable;

HashTable* create_hash_table();
unsigned int hash_function(char *key);
void hash_insert(HashTable *table, char *key, void *data);
void* hash_get(HashTable *table, char *key);
void hash_remove(HashTable *table, char *key);
void free_hash_table(HashTable *table);

#endif