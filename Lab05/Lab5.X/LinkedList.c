/* 
 * File:   LinkedList.c
 * Author: James Ensminger (jensming@ucsc.edu)
 *
 * Created on January 31, 2022, 6:57 PM
 */

// Standard libraries
#include <stdio.h>
#include <string.h>

// User libraries
#include "BOARD.h"
#include "LinkedList.h"

// **** Declared function prototypes ****
ListItem *LinkedListNew(char *data);
ListItem *LinkedListCreateAfter(ListItem *item, char *data);
char *LinkedListRemove(ListItem *item);
int LinkedListSize(ListItem *list);
ListItem *LinkedListGetFirst(ListItem *list);
ListItem *LinkedListGetLast(ListItem *list);
int LinkedListSwapData(ListItem *firstItem, ListItem *secondItem);
int LinkedListPrint(ListItem *list);

/********************************************************************************
 * Defined LinkedListNew function.
 ********************************************************************************/
ListItem *LinkedListNew(char *data) {
    ListItem *item = malloc(sizeof (ListItem)); // allocate item in the heap

    if (item == NULL) { // checks for malloc() failure
        return item;
    } else {
        item->nextItem = NULL; // initialize nextItem
        item->previousItem = NULL; // initialize previousItem
        item->data = data; // assign data to item
        return item;
    }
}

/********************************************************************************
 * Defined LinkedListCreateAfter function.
 ********************************************************************************/
ListItem *LinkedListCreateAfter(ListItem *item, char *data) {
    ListItem *newItem = malloc(sizeof (ListItem)); // new ListItem

    if (newItem == NULL) { // checks for malloc() failure
        return newItem;
    } else if (item == NULL) { // treat 'newItem' as as new LinkedList if 'item' is NULL
        newItem->nextItem = NULL;
        newItem->previousItem = NULL;
        newItem->data = data;
    } else if (item->nextItem == NULL) { // nextItem of 'item' is NULL if at the end of the list
        newItem->nextItem = NULL;
    } else { // shift pointers of two other ListItems if 'item' isn't the end of the list
        newItem->nextItem = item->nextItem;
        item->nextItem->previousItem = newItem;
    }

    item->nextItem = newItem; // next ListItem for 'item' is 'newItem' now
    newItem->previousItem = item; // previous ListItem for 'newItem' is 'item' now
    newItem->data = data;
    return newItem;
}

/********************************************************************************
 * Defined LinkedListRemove function.
 ********************************************************************************/
char *LinkedListRemove(ListItem *item) {
    char *itemToRemove = NULL;

    if (item == NULL) { // checks if 'item' is NULL
        return itemToRemove;
    } else {
        itemToRemove = item->data;
    }

    if ((item->previousItem == NULL) && (item->nextItem != NULL)) { // nextItem is new head of list if 'item' is head
        item->nextItem->previousItem = NULL;
    } else if ((item->previousItem != NULL) && (item->nextItem == NULL)) { // previousItem is new tail of list if 'item' is tail
        item->previousItem->nextItem = NULL;
    } else if ((item->previousItem != NULL) && (item->nextItem != NULL)) { // make two ListItems point at each 
        // other if 'item' is between them
        item->previousItem->nextItem = item->nextItem;
        item->nextItem->previousItem = item->previousItem;
    } else {
        return STANDARD_ERROR; // return error if no other condition was met for removing such an 'item'
    }

    free(item); // free up memory that item was using
    return itemToRemove;
}

/********************************************************************************
 * Defined LinkedListSize function.
 ********************************************************************************/
int LinkedListSize(ListItem *list) {
    ListItem *head = LinkedListGetFirst(list); // get head of list
    int listSize = 0;

    while (head != NULL) // increments listSize until end of list is reached
    {
        listSize++;
        head = head->nextItem;
    }
    return listSize;
}

/********************************************************************************
 * Defined LinkedListGetFirst function.
 ********************************************************************************/
ListItem *LinkedListGetFirst(ListItem *list) {
    if (list == NULL) { // checks if 'list' is NULL
        return list;
    } else if (list->previousItem == NULL) { // 'list' is head of list if previousItem is NULL
        return list;
    } else { // assigns 'list' to its previousItem until head of list is reached
        ListItem *tempItem = list;
        while (list->previousItem != NULL) {
            tempItem = tempItem->previousItem;
            list = tempItem;
        }
        return list;
    }
}

/********************************************************************************
 * Defined LinkedListGetLast function.
 ********************************************************************************/
ListItem *LinkedListGetLast(ListItem *list) {
    if (list == NULL) { // checks if 'list' is NULL
        return list;
    } else if (list->nextItem == NULL) { // 'list' is tail of list if nextItem is NULL
        return list;
    } else { // assigns 'list' to its nextItem until tail of list is reached
        ListItem *tempItem = list;
        while (list->nextItem != NULL) {
            tempItem = tempItem->nextItem;
            list = tempItem;
        }
        return list;
    }
}

/********************************************************************************
 * Defined LinkedListSwapData function.
 ********************************************************************************/
int LinkedListSwapData(ListItem *firstItem, ListItem *secondItem) {
    if ((firstItem == NULL) || (secondItem == NULL)) { // if either 'item' is NULL, return error
        return STANDARD_ERROR;
    } else { // swap data assignments for both ListItems
        char *temp = firstItem->data;
        firstItem->data = secondItem->data;
        secondItem->data = temp;
        return SUCCESS;
    }
}

/********************************************************************************
 * Defined LinkedListPrint function.
 ********************************************************************************/
int LinkedListPrint(ListItem *list) {
    if (list == NULL) // if 'list' is NULL, return error
    {
        return STANDARD_ERROR;
    } else { // prints out the complete list starting from the head
        ListItem *head = LinkedListGetFirst(list);
        printf("{");
        while (head != NULL) {
            if (head->nextItem != NULL) {
                printf("%s, ", head->data);
            } else {
                printf("%s}\n", head->data);
            }
            head = head->nextItem;
        }
        return SUCCESS;
    }
}