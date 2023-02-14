#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <sys/types.h>
#include <unistd.h>

struct Node {
char* filePath;
pid_t pid;
struct Node* nextNode;
};

struct LinkedList {
struct Node* headNode;
};

struct LinkedList* initList();
void addNode(struct LinkedList*, char*, pid_t);
void deleteNodeByPID(struct LinkedList*, pid_t);
struct Node* findNodeByPID(struct LinkedList*, pid_t);
void printList(struct LinkedList* list);
int listLength(struct LinkedList* list);

#endif