#include <stdlib.h>
#include <stdio.h>
#include "pri_queue.h"
/** @file pri_queue.c */
static Node_ptr_t head = NULL;

/**
 * Insert a Node into a priority queue.
 * @param priority
 * @param data
 * @author YOUR NAME
 */



void PQ_insert(int priority, char * data) {

    Node_ptr_t new_node = malloc(sizeof (Node_t));
    Node_ptr_t tmp;
    int check = 0;
    new_node->data = data;
    new_node->priority = priority;

    if ((PQ_get_size() == 0) || (priority > (head->priority))) {
        new_node->next = head;
        head = new_node;


    } else {

        tmp = head;
        
        while ((tmp->next != NULL) && ((tmp->next->priority)>(priority))) {
            tmp = tmp->next;

        }

        new_node->next = tmp->next;
        tmp->next = new_node;
    }


}


/**
 * Delete and return the node with the highest priority.
 * @return The highest priority Node.
 */
Node_ptr_t PQ_delete() {
    Node_ptr_t tmp = malloc(sizeof (Node_t));
    if (PQ_get_size() == 0) {
        return NULL;
    } else {
        tmp = head;
        head = head->next;
        return tmp;
    }
}

/**
 * Do NOT modify this function.
 * @return A pointer to the head of the list.  (NULL if list is empty.)
 */
Node_ptr_t PQ_get_head() {
    return head;
}

/**
 * Do NOT modify this function.
 * @return the number of items in the queue
 */
int PQ_get_size() {
    int size = 0;
    Node_ptr_t tmp;
    for (tmp = head; tmp != NULL; tmp = tmp->next, size++)
        ;
    return size;
}


