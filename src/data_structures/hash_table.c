#include "hash_table.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

HashTable *create_hash_table()
{
    HashTable *table = (HashTable *)malloc(sizeof(HashTable));
    if (!table)
        return NULL;

    table->buckets = (LinkedList **)malloc(TABLE_SIZE * sizeof(LinkedList *));
    if (!table->buckets)
    {
        free(table);
        return NULL;
    }

    for (int i = 0; i < TABLE_SIZE; i++)
    {
        table->buckets[i] = create_linked_list();
    }

    table->size = TABLE_SIZE;
    return table;
}

unsigned int hash_function(char *key)
{
    unsigned int hash = 0;
    while (*key)
    {
        hash = (hash * 31) + (unsigned char)(*key);
        key++;
    }
    return hash % TABLE_SIZE;
}

int hash_entry_compare(void *a, void *b)
{
    HashEntry *entryA = (HashEntry *)a;
    HashEntry *entryB = (HashEntry *)b;
    return strcmp(entryA->key, entryB->key);
}

void hash_insert(HashTable *table, char *key, void *data)
{
    unsigned int index = hash_function(key);

    HashEntry searchEntry = {key, NULL};
    list_remove(table->buckets[index], &searchEntry, hash_entry_compare);

    HashEntry *newEntry = (HashEntry *)malloc(sizeof(HashEntry));
    newEntry->key = strdup(key);
    newEntry->data = data;

    list_insert(table->buckets[index], newEntry);
}

void *hash_get(HashTable *table, char *key)
{
    unsigned int index = hash_function(key);

    HashEntry searchEntry = {key, NULL};
    HashEntry *found = (HashEntry *)list_find(table->buckets[index], &searchEntry, hash_entry_compare);

    return found ? found->data : NULL;
}

// ✅ Remove key-value pair
void hash_remove(HashTable *table, char *key)
{
    unsigned int index = hash_function(key);

    HashEntry searchEntry = {key, NULL};
    HashEntry *removed = (HashEntry *)list_remove(table->buckets[index], &searchEntry, hash_entry_compare);

    if (removed)
    {
        free(removed->key);
        free(removed);
    }
}

// ✅ Free entire hash table
void free_hash_table(HashTable *table)
{
    for (int i = 0; i < table->size; i++)
    {
        ListNode *node = table->buckets[i]->head;

        while (node)
        {
            HashEntry *entry = (HashEntry *)node->data;
            free(entry->key);
            free(entry);
            node = node->next;
        }

        free_linked_list(table->buckets[i]);
    }

    free(table->buckets);
    free(table);
}
