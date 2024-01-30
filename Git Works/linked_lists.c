#include <stdio.h>
#include <stdlib.h>

struct Node
{
    int data;
    struct Node *next;
};

//print list nodes
void printList(struct Node *head)
{
    while(head)
    {
        printf("%i  %p -> %p \n", head->data, head, head->next);
        head = head->next;
    }
}

//get list items count
int getListCount(struct Node *head)
{
    int count = 0;
    struct Node* temp = head;
    while(!temp->next)
    {
        count++;
        temp = temp->next;
    }

    return count;
}

//Insert node at the top of the list
void pushNode(struct Node **head, int data)
{
    struct Node *new_node = (struct Node*)malloc(sizeof(struct Node));
    new_node->data = data;
    new_node->next = (*head);
    (*head) = new_node;
}

//returns last node of the list
struct Node* getNodeTail(struct Node* head)
{
    if(!head)   return NULL;

    struct Node* current_node = head;
    struct Node* last_node = head;
    
    while(current_node)
    {
        last_node = current_node;
        current_node = current_node -> next;
    }

    return last_node;
}

//insert node at the bottom of the list
void appendNode(struct Node **head, int data)
{
    struct Node *new_node = (struct Node*)malloc(sizeof(struct Node));
    new_node->data = data;
    new_node->next = NULL;

    if(!head)
    {
        (*head)=new_node;
        return;
    }

    struct Node *tail = getNodeTail(*head);
    tail->next = new_node;
}

//returns and delete node from the top of the list
struct Node *pop(struct Node **head)
{
    if(!head) return NULL;

    struct Node *temp = *head;
    *head = (*head)->next;
    temp->next = NULL;
    return temp;
}

//returns and delete node from the botton of the list
struct Node *popEnd(struct Node **head)
{
    if(!head) return NULL;

    struct Node *end = *head;
    struct Node *prev = NULL;
    while(end->next)
    {
        prev = end;
        end = end->next;
    }

    prev->next = NULL;
    return end;
}

//returns and delete node from the middle of the list
struct Node *delete(struct Node **head, int index)
{
    struct Node *temp = *head;
    struct Node *prev = *head;

    for(int i=0;i<=index;i++)
    {
        if(i == index)
        {
            prev->next = temp->next;
            break;
        }
        else
        {
            prev = temp;
            if(!prev) break;
            temp = temp->next;
        }
    }
    return temp;
}

//reversing of the list
void reverseList(struct Node **head)
{
    if(!head || (*head)->next) return;

    struct Node *temp = *head;
    struct Node *prev = NULL;

    int nodeCount = getListCount(*head);

    for (int i = 0; i < nodeCount; i++)
    {
        while(!temp->next)
        {
            prev = temp;
            temp = temp->next;
        }

        prev->next = NULL;
        temp->next = *head;
        *head = temp;
    }
}

int main(int argc, char** argv)
{

    return 0;
}