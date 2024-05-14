#include <stdio.h>
#include <stdlib.h>

struct Node
{
    int data;
    struct Node *prev;
    struct Node *next;
};

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

//print list nodes
void printList(struct Node* node) {
    while (node != NULL) {
        printf("%d ", node->data);
        node = node->next;
    }
    printf("\n");
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
    new_node->prev = NULL;
    (*head) = new_node;
}

//insert node at the bottom of the list
void appendNode(struct Node **head, int data)
{
    struct Node *new_node = (struct Node*)malloc(sizeof(struct Node));
    new_node->data = data;
    new_node->next = NULL;
    new_node->prev = NULL;

    if(*head == NULL)
    {
        (*head)=new_node;
        return;
    }

    struct Node *tail = getNodeTail(*head);
    
    tail->next = new_node;
    new_node->prev = tail;
}


//returns and delete node from the top of the list
struct Node *pop(struct Node **head)
{
    if(!head) return NULL;

    struct Node *temp = *head;
    *head = (*head)->next;
    temp->next = NULL;
    (*head)->prev = NULL;
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
    end->prev = NULL;
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
            temp->next->prev = temp->prev;
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

//insert after an item
void insertAfterItem(struct Node** head, int item, int data) {
    
    struct Node* prevNode = *head;
    while(prevNode != NULL && prevNode->data != item)
    prevNode = prevNode->next;

    if (prevNode == NULL) {
        return;
    }

    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->next = NULL;
    newNode->prev = NULL;
    newNode->data = data;

    // Aggiorna il puntatore al nodo successivo del nuovo nodo
    newNode->next = prevNode->next;

    // Aggiorna il puntatore al nodo precedente del nodo successivo
    if (prevNode->next != NULL)
        prevNode->next->prev = newNode;

    // Aggiorna il puntatore al nodo successivo del nodo precedente
    prevNode->next = newNode;

    // Aggiorna il puntatore al nodo precedente del nuovo nodo
    newNode->prev = prevNode;
}

//insert before an item
void insertBeforeItem(struct Node** head, int item, int data) {
    
    struct Node* nextNode = *head;
    while(nextNode != NULL && nextNode->data != item)
    nextNode = nextNode->next;

    if (nextNode == NULL) {
        return;
    }

    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->next = NULL;
    newNode->prev = NULL;
    newNode->data = data;

    if (*head == nextNode) {
        newNode->next = nextNode;
        nextNode->prev = newNode;
        *head = newNode;
        return;
    }
    
    newNode->next = nextNode;

    newNode->prev = nextNode->prev;

    nextNode->prev->next = newNode;

    nextNode->prev = newNode;
}

int main(int argc, char** argv)
{
    //generate list
    struct Node* node = NULL;

    //push data
    pushNode(&node,10);
    pushNode(&node,20);
    pushNode(&node,30);

    //append data
    appendNode(&node,40);
    appendNode(&node,50);
    appendNode(&node,60);

    printf("\n");
    printf("printing the list: \n");
    printList(node);

    //reverse the list
    // reverseList(&node);
    // printf("\n");
    // printf("printing the reversed list: \n");
    // printList(node);

    //popEnd and pop the list
    printf("\n");
    printf("pop the last number in list: \n%d \n",popEnd(&node)->data);
    printf("\n");
    printf("pop the first number in list: \n%d \n",pop(&node)->data);
    printf("\n");
    printf("printing the actual list: \n");
    printList(node);

    //delete from index
    delete(&node,1);
    printf("\n");
    printf("printing the list after the cancellation of the second number: \n");
    printList(node);

    //insert after and before items
    insertAfterItem(&node, 20, 21);
    insertBeforeItem(&node, 20, 19);
    printf("printing list after insertion of 21 and 19 after and before 20: \n");
    printList(node);

    return 0;
}