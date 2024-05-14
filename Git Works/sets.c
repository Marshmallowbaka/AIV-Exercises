#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define SIZE 32

// hash function
unsigned long hash(char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) ^ c; // Modifica rispetto a djb2
    }
    return hash % SIZE;
}

struct Node {
    char *key;
    struct Node *next;
};

struct Set {
    struct Node *table[SIZE];
};

//create new node function
struct Node *createNode(char *key) {
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    if (newNode == NULL) {
        printf("Memoria esaurita!\n");
        exit(1);
    }
    newNode->key = strdup(key);
    newNode->next = NULL;
    return newNode;
}

//initialize set to 0
void initializeSet(struct Set *set) {
    for (int i = 0; i < SIZE; i++) {
        set->table[i] = NULL;
    }
}

// insert element in set
void insert(struct Set *set, char *key) {
    unsigned long index = hash(key);
    struct Node *newNode = createNode(key);
    newNode->next = set->table[index];
    set->table[index] = newNode;
}

// check element in set
bool contains(struct Set *set, char *key) {
    unsigned long index = hash(key);
    struct Node *current = set->table[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return true;
        }
        current = current->next;
    }
    return false;
}

// remove element from set
void removeElement(struct Set *set, char *key) {
    unsigned long index = hash(key);
    struct Node *current = set->table[index];
    struct Node *prev = NULL;
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            if (prev == NULL) {
                set->table[index] = current->next;
            } else {
                prev->next = current->next;
            }
            free(current->key);
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

// to print set
void printSet(struct Set *set) {
    int i;
    for (i = 0; i < SIZE; i++) {
        struct Node *current = set->table[i];
        printf("Set[%d]: ", i);
        while (current != NULL) {
            printf("%s ", current->key);
            current = current->next;
        }
        printf("\n");
    }
}

int main() {
    struct Set mySet;
    initializeSet(&mySet);

    insert(&mySet, "fragola");
    insert(&mySet, "panna");
    insert(&mySet, "champagne");
    insert(&mySet, "luna");

    printf("Il set contiene 'banana'? %s\n", contains(&mySet, "fragola") ? "true" : "false");
    printf("Il set contiene 'watermelon'? %s\n", contains(&mySet, "sotto") ? "true" : "false");

    printf("Set prima della rimozione:\n");
    printSet(&mySet);

    removeElement(&mySet, "luna");

    printf("Set dopo la rimozione:\n");
    printSet(&mySet);

    return 0;
}