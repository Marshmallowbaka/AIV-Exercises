#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define SIZE 100

// hash function
unsigned long hash(char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) ^ c;
    }
    return hash % SIZE;
}

// Dictionary Node
struct KeyValuePair {
    char *key;
    char *value;
    struct KeyValuePair *next;
};

// Dictionary
struct Dictionary {
    struct KeyValuePair *table[SIZE];
};

// like create Node but use a pair
struct KeyValuePair *createKeyValuePair(char *key, char *value) {
    struct KeyValuePair *newPair = (struct KeyValuePair *)malloc(sizeof(struct KeyValuePair));
    if (newPair == NULL) {
        printf("Memoria esaurita!\n");
        exit(1);
    }
    newPair->key = strdup(key);
    newPair->value = strdup(value);
    newPair->next = NULL;
    return newPair;
}

// initialize empty dictionary
void initializeDictionary(struct Dictionary *dict) {
    for (int i = 0; i < SIZE; i++) {
        dict->table[i] = NULL;
    }
}

// insert value
void insertKeyValuePair(struct Dictionary *dict, char *key, char *value) {
    unsigned long index = hash(key);
    struct KeyValuePair *newPair = createKeyValuePair(key, value);
    newPair->next = dict->table[index];
    dict->table[index] = newPair;
}

// get value
char *getValue(struct Dictionary *dict, char *key) {
    unsigned long index = hash(key);
    struct KeyValuePair *current = dict->table[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current->value;
        }
        current = current->next;
    }
    return NULL; // Chiave non trovata
}

// remove a value
void removeKeyValuePair(struct Dictionary *dict, char *key) {
    unsigned long index = hash(key);
    struct KeyValuePair *current = dict->table[index];
    struct KeyValuePair *prev = NULL;
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            if (prev == NULL) {
                dict->table[index] = current->next;
            } else {
                prev->next = current->next;
            }
            free(current->key);
            free(current->value);
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

// print dictionary
void printDictionary(struct Dictionary *dict) {
    for (int i = 0; i < SIZE; i++) {
        struct KeyValuePair *current = dict->table[i];
        printf("Bucket[%d]: ", i);
        while (current != NULL) {
            printf("(%s, %s) ", current->key, current->value);
            current = current->next;
        }
        printf("\n");
    }
}

int main() {
    struct Dictionary myDict;
    initializeDictionary(&myDict);

    insertKeyValuePair(&myDict, "apple", "mela");
    insertKeyValuePair(&myDict, "banana", "banana");
    insertKeyValuePair(&myDict, "orange", "arancia");
    insertKeyValuePair(&myDict, "grape", "uva");

    printf("Il valore associato a 'banana' è: %s\n", getValue(&myDict, "banana"));

    printf("Dizionario prima della rimozione:\n");
    printDictionary(&myDict);

    removeKeyValuePair(&myDict, "banana");

    printf("Dizionario dopo la rimozione:\n");
    printDictionary(&myDict);

    return 0;
}
