#include <stdio.h>
#include <string.h>

#define MAX_SIZE 100

struct dicitonary 
{
    int size;
    char *keys[MAX_SIZE];
    int values[MAX_SIZE];
};

// int size = 0;
// char *keys[MAX_SIZE];
// int values[MAX_SIZE];

//get index of a key
int getIndex(struct dicitonary dc, char *key)
{
    for (int i = 0; i < dc.size; i++)
    {
        if(strcmp(dc.keys[i], key) == 0) return i; //string compare 
    }
    return -1; //no key found
}

//insert element
void insert(struct dicitonary dc, char *key, int data)
{
    int index = getIndex(dc, key);
    if(index == -1) //key not found
    {
        strcpy(dc.keys[dc.size],key);
        dc.values[dc.size] = data;
        dc.size++;
    }
    else dc.values[index] = data; //key found
}

//get element
int get(struct dicitonary dc, char *key)
{
    int index = getIndex(dc, key);
    
    if(index == -1) return -1;

    return dc.values[index];
}

//print
void printDictionary(struct dicitonary dc)
{
    for (int i = 0; i < dc.size; i++)
    {
        printf("%s: %d\n",dc.keys[i],dc.values[i]);
    }
    
}