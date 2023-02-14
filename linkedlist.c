#include "linkedlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

// Function to initialize the linked list
struct LinkedList* initList() {
  // Allocate memory for the linked list
  struct LinkedList* list = (struct LinkedList*) malloc(sizeof(struct LinkedList));

  // Initialize the head node to NULL
  list->headNode = NULL;

  // Return the pointer to the newly created linked list
  return list;
}

// Function to add a node to the linked list
void addNode(struct LinkedList* list, char* filePath, pid_t pid) {
  // Allocate memory for the new node
  struct Node* newNode = (struct Node*) malloc(sizeof(struct Node));

  // Set the data for the new node
  newNode->filePath = (char*) malloc(strlen(filePath) + 1);
  strcpy(newNode->filePath, filePath);
  newNode->pid = pid;

  // Make the next node of the new node the current head node
  newNode->nextNode = list->headNode;

  // Make the new node the head node
  list->headNode = newNode;
}

// Function to delete a node from the linked list
void deleteNodeByPID(struct LinkedList* list, pid_t pid) {
  // Initialize pointers to keep track of the current node and the previous node
  struct Node* curr = list->headNode;
  struct Node* prev = NULL;

  // Iterate through the linked list to find the node with the given PID
  while (curr != NULL) {
    if (curr->pid == pid) {
      // If the previous node is NULL, the current node is the head node, so update the head node
      if (prev == NULL) {
        list->headNode = curr->nextNode;
      } else {
        // Otherwise, update the next node of the previous node to be the next node of the current node
        prev->nextNode = curr->nextNode;
      }

      // Free the memory for the current node and its file path
      free(curr->filePath);
      free(curr);

      // Break out of the loop
      break;
    }

    // Update the previous node to be the current node and the current node to be the next node
    prev = curr;
    curr = curr->nextNode;
  }
}

// Function to find a node in the linked list
struct Node* findNodeByPID(struct LinkedList* list, pid_t pid) {
  // Initialize a pointer to keep track of the current node
  struct Node* curr = list->headNode;

  // Iterate through the linked list to find the node with the given PID
  while (curr != NULL) {
    if (curr->pid == pid) {
      // Return the pointer to the node if it is found
      return curr;
    }
    curr = curr->nextNode;
  }

  // Return NULL if the node with the given PID is not found
  return NULL;
}


// Function to print the contents of the linked list
void printList(struct LinkedList* list) {
// Initialize a pointer to keep track of the current node
  struct Node* curr = list->headNode;

  // Iterate through the linked list and print the contents of each node
  while (curr != NULL) {
    printf("%d: %s\n", curr->pid, curr->filePath);
    // printf("File Path: %s, PID: %d\n", curr->filePath, curr->pid);
    curr = curr->nextNode;
  } 
}

// Function to count the number of nodes in the linked list
int listLength(struct LinkedList* list) {
// Initialize a pointer to keep track of the current node and a count variable
  struct Node* curr = list->headNode;
  int count = 0;

// Iterate through the linked list and increment the count for each node
  while (curr != NULL) {
    count++;
    curr = curr->nextNode;
  }

// Return the count
  return count;
}
