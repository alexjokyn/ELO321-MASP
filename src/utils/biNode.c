#include <stdio.h>
#include <stdlib.h>
#include "biNode.h"

typedef struct biNode{
    int value;
    struct biNode* next;
    struct biNode* prev;
}biNode;

void push(int newValue, biNode** list){
    biNode* newNode = (biNode*)malloc(sizeof(biNode)); 
    if(!*list){
        newNode->value = newValue;
        newNode->next = NULL;
        newNode->prev = NULL;
    }else{
        newNode->value = newValue;
        newNode->next = *list;
        newNode->prev = NULL;
        (*list)->prev = newNode;
    }  
    *list = newNode;

    // To keep track
    printf("%d\n",(*list)->value);
}

int valueAt(int index, biNode* list){
    if(!list){
        printf("ERROR empty list\n");
        return -1;
    }

    int topIndex = 0;    
    while(list->next){
        list = list->next;
        topIndex++;
    }
    if(index>topIndex){
        printf("ERROR value out of index\n");
        return -1;
    }

    int value;
    for(int i=0; i<index; i++){
        list = list->prev;
    }
    value = list->value;
    return value;
}

int pop(biNode** list){
    if(!*list){
        printf("ERROR empty list\n");
        return -1;
    }

    biNode* toFree = *list;
    int popped = (*list)->value;

    *list = (*list)->next;

    if(*list){ // in case the list is NOT left empty
        (*list)->prev = NULL;
    }
    free(toFree);
    return popped;
}

int popValue(int findValue, biNode** list){
    biNode* listHeader = *list;

    if(!*list){
        printf("ERROR empty list\n");
        return -1;
    }

    while(*list){
        if((*list)->value==findValue){
            if((*list)->next && (*list)->prev){
                ((*list)->prev)->next=(*list)->next;
                ((*list)->next)->prev=(*list)->prev;
            }
            else if((*list)->next && !(*list)->prev){
                ((*list)->next)->prev=NULL;
                listHeader = listHeader->next;
            }
            else if(!(*list)->next && (*list)->prev){
                ((*list)->prev)->next=NULL;
            }
            else{
                *list = NULL;
                return findValue;
            }
            *list = listHeader;
            return findValue;
        }
        *list = (*list)->next;
    }
    
    *list = listHeader;
    printf("ERROR value not found\n");
    return -1;
}

int popAt(int index, biNode** list){
    biNode* listHeader = *list;
    int popped;
    
    if(!*list){
        printf("ERROR empty list\n");
        return -1;
    }

    int topIndex = 0;    
    while((*list)->next){
        *list = (*list)->next;
        topIndex++;
    }
    if(index>topIndex){
        printf("ERROR value out of index\n");
        return -1;
    }
    
    // IN CASE THE LIST IS LEFT EMPTY
    // if(!(*list)->next && !(*list)->prev){}
    if(topIndex==0){
        popped = (*list)->value;
        *list = NULL;
        return popped;
    }

    for(int i=0; i<index; i++){
        *list = (*list)->prev;
    }
    popped = (*list)->value;

    // IN CASE NEITHER THE LAST NOR FIRST ELEMENT IS POPPED
    if((*list)->next && (*list)->prev){
        ((*list)->prev)->next=(*list)->next;
        ((*list)->next)->prev=(*list)->prev;
    }
    // IN CASE ONLY THE LAST ELEMENT IS POPPED (RELOCATE HEADER POINTER)
    else if((*list)->next){
        ((*list)->next)->prev=NULL;
        listHeader = listHeader->next;

        // or pop(list) fixing the pointer direction before executing
    }
    
    else if((*list)->prev){  // (*list)->prev && !(*list)->next
        ((*list)->prev)->next=NULL;
    }

    free(*list);
    //*list = NULL;
    *list = listHeader;

    return popped;
}

void clear(biNode **list){
    if(*list==NULL){
        return;
    }

    biNode *parser;
    while ((*list)->next)
    {
        parser = (*list)->next;
        free(*list);
        *list = parser;
    }

    free(*list);
    *list = NULL;
}


void printList(biNode* list){
    printf("[");
    if(!list){
        printf("]\n");
    }else{
        while(list->next){
            list = list->next;
            //printf("%d\n",list->value);
        }
        while(list){
            printf("%d",list->value);
            list = list->prev;
            if(list){
                printf(", ");
            }
        }
        printf("]\n");
    }
}
