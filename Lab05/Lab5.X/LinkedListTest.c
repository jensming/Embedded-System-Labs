/* 
 * File:   LinkedListTest.c
 * Author: James Ensminger (jensming@ucsc.edu)
 *
 * Created on January 31, 2022, 6:58 PM
 */

// Standard libraries
#include <stdio.h>
#include <string.h>

// User libraries
#include "BOARD.h"
#include "LinkedList.h"

int main() {
    BOARD_Init();

    printf("\n###### Beginning 1725314's LinkedList test harness: ####\n\n");

    // LinkedListNew() test
#if 1
    char *item0[] = {"test", NULL};
    int size0 = 0;
    ListItem *newItem0 = LinkedListNew(item0[0]);
    printf("Does LinkedListNew() create a new list of size 1?\n");
//    printf("this is the item: %s\n", newItem0->data);
    while (newItem0 != NULL) {
        size0++;
        newItem0 = newItem0->nextItem;
    }
    if (size0 == 1) {
        printf("    Success!\n");
    } else {
        printf("    Failure, new list is an unexpected size. Result: %d\n", size0);
    }
    free(newItem0);
    
    
    char *item1[] = {NULL};
    int size1 = 0;
    ListItem *newItem1 = LinkedListNew(item1[0]);
    printf("Does LinkedListNew() create a new list with no data in it?\n");
//    printf("this is the item: %s\n", newItem1->data);
    while (newItem1 != NULL) {
        size1++;
        newItem1 = newItem1->nextItem;
    }
    if (size1 == 1) {
        printf("    Success!\n");
    } else {
        printf("    Failure, new list is an unexpected size. Result: %d\n", size1);
    }
    free(newItem1);
#endif

    
    // LinkedListCreateAfter() test
#if 0
    char *item0[] = {"1", NULL};
    char *item1[] = {"2", NULL};
    int size0 = 0;
    ListItem *head0 = LinkedListNew(item0[0]);
    ListItem *tail0 = LinkedListCreateAfter(head0, item1[0]);
    printf("Does LinkedListCreateAfter() create a new list of size 2?\n");
//    printf("this is the item: %s\n", head0->data);
//    printf("this is the item: %s\n", tail0->data);
    while (tail0 != NULL) {
        size0++;
        tail0 = tail0->previousItem;
    }
    if (size0 == 2) {
        printf("    Success!\n");
    } else {
        printf("    Failure, new list is an unexpected size. Result: %d\n", size0);
    }
    free(head0);
    free(tail0);
    
    
    char *itemA[] = {"1", NULL};
    char *itemB[] = {"2", NULL};
    char *itemC[] = {"3", NULL};
    int size1 = 0;
    ListItem *head1 = LinkedListNew(itemA[0]);
    ListItem *middle = LinkedListCreateAfter(head1, itemB[0]);
    ListItem *tail1 = LinkedListCreateAfter(middle, itemC[0]);
    printf("Does LinkedListCreateAfter() create a new list of size 3?\n");
//    printf("this is the item: %s\n", head1->data);
//    printf("this is the item: %s\n", middle->data);
//    printf("this is the item: %s\n", tail1->data);
    while (tail1 != NULL) {
        size1++;
        tail1 = tail1->previousItem;
    }
    if (size1 == 3) {
        printf("    Success!\n");
    } else {
        printf("    Failure, new list is an unexpected size. Result: %d\n", size1);
    }
    free(head1);
    free(middle);
    free(tail1);
#endif
    
    
    // LinkedListRemove() test
#if 0
    ListItem *head0 = LinkedListNew("Hello");
    ListItem *tail0 = LinkedListCreateAfter(head0, "World!");
    char *removedItem0 = LinkedListRemove(tail0);
    printf("Does LinkedListRemove() obtain the removed item?\n");
//    printf("this is the item: %s\n", removedItem0);
    if (strncmp(removedItem0, "World!", 6) == 0) {
        printf("    Success!\n");
    } else {
        printf("    Failure, unexpected item removed. Result: %s\n", removedItem0);
    }
    free(head0);
    
    ListItem *head1 = LinkedListNew("Hello");
    ListItem *second = LinkedListCreateAfter(head1, "World,");
    ListItem *third = LinkedListCreateAfter(second, "it's");
    ListItem *tail1 = LinkedListCreateAfter(third, "me!");
    int size = 0;
    char *removedItem1 = LinkedListRemove(third);
    printf("Does LinkedListRemove() obtain the removed item and free the memory it was using?\n");
//    printf("this is the item: %s\n", removedItem1);
    while (tail1 != NULL) {
        size++;
        tail1 = tail1->previousItem;
    }
    if ((strncmp(removedItem1, "it's", 4) == 0) && (size == 3)) {
        printf("    Success!\n");
    } else {
        printf("    Failure, unexpected item removed or LinkedList size. Removed Item: \"%s\", "
               "LinkedList size:%d\n", removedItem1, size);
    }
    free(head1);
    free(second);
    free(tail1);
#endif
    
    
    // LinkedListGetSize() test
#if 0
    ListItem *head0 = LinkedListNew("First!");
    ListItem *second0 = LinkedListCreateAfter(head0, "Second?");
    ListItem *third0 = LinkedListCreateAfter(second0, "Third??");
    ListItem *tail0 = LinkedListCreateAfter(third0, "Fourth?!");
    int size0 = LinkedListSize(second0);
    printf("Does LinkedListSize() work when any item in the middle of the list is passed?\n");
    if (size0 == 4) {
        printf("    Success!\n");
    } else {
        printf("    Failure, incorrect size of list returned. Result: %d\n", size0);
    }
    free(head0);
    free(second0);
    free(third0);
    free(tail0);
    
    
    ListItem *head1 = LinkedListNew("First!");
    ListItem *second1 = LinkedListCreateAfter(head1, "Second?");
    ListItem *third1 = LinkedListCreateAfter(second1, "Third??");
    ListItem *fourth1 = LinkedListCreateAfter(third1, "Fourth!?");
    ListItem *tail1 = LinkedListCreateAfter(fourth1, "Fifth?!!");
    int size1 = LinkedListSize(tail1);
    printf("Does LinkedListSize() work when the last item in the list is passed?\n");
    if (size1 == 5) {
        printf("    Success!\n");
    } else {
        printf("    Failure, incorrect size of list returned. Result: %d\n", size1);
    }
    free(head1);
    free(second1);
    free(third1);
    free(fourth1);
    free(tail1);
    
    
    ListItem *head2 = NULL;
    int size2 = LinkedListSize(head2);
    printf("Does LinkedListSize() work when item passed is NULL?\n");
    if (size2 == 0) {
        printf("    Success!\n");
    }
    else {
        printf("    Failure, incorrect size of list returned. Result: %d\n", size2);
    }
    free(head2);
#endif
    
    
    // LinkedListGetFirst() test
#if 0
    ListItem *head0 = LinkedListNew("First!");
    ListItem *second0 = LinkedListCreateAfter(head0, "Second...");
    ListItem *third0 = LinkedListCreateAfter(second0, "Third?");
    ListItem *tail0 = LinkedListCreateAfter(third0, "Fourth?!");
    ListItem *firstItem0 = LinkedListGetFirst(second0);
    printf("Does LinkedListGetFirst() work when any item in the middle of the list is passed?\n");
    if (strncmp(firstItem0->data, "First!", 6) == 0) {
        printf("    Success!\n");
    } else {
        printf("    Failure, item returned isn't head of the list. Result: %s\n", firstItem0->data);
    }
    free(head0);
    free(second0);
    free(third0);
    free(tail0);
    
    
    ListItem *head1 = LinkedListNew("First!");
    ListItem *second1 = LinkedListCreateAfter(head1, "Second...");
    ListItem *third1 = LinkedListCreateAfter(second1, "Third?");
    ListItem *tail1 = LinkedListCreateAfter(third1, "Fourth?!");
    ListItem *firstItem1 = LinkedListGetFirst(head1);
    printf("Does LinkedListGetFirst() work when the first item in the list is passed?\n");
    if (strncmp(firstItem1->data, "First!", 6) == 0) {
        printf("    Success!\n");
    } else {
        printf("    Failure, item returned isn't head of the list. Result: %s\n", firstItem1->data);
    }
    free(head1);
    free(second1);
    free(third1);
    free(tail1);
    
    
    ListItem *head2 = LinkedListNew("First!");
    ListItem *second2 = LinkedListCreateAfter(head2, "Second...");
    ListItem *third2 = LinkedListCreateAfter(second2, "Third?");
    ListItem *tail2 = LinkedListCreateAfter(third2, "Fourth?!");
    ListItem *firstItem2 = LinkedListGetFirst(tail2);
    printf("Does LinkedListGetFirst() work when the last item in the list is passed?\n");
    if (strncmp(firstItem2->data, "First!", 6) == 0) {
        printf("    Success!\n");
    } else {
        printf("    Failure, item returned isn't head of the list. Result: %s\n", firstItem2->data);
    }
    free(head2);
    free(second2);
    free(third2);
    free(tail2);
#endif
    
    
    // LinkedListGetLast() test
#if 0
    ListItem *head0 = LinkedListNew("First..");
    ListItem *second0 = LinkedListCreateAfter(head0, "Second...");
    ListItem *third0 = LinkedListCreateAfter(second0, "Third?");
    ListItem *tail0 = LinkedListCreateAfter(third0, "Last!");
    ListItem *lastItemA = LinkedListGetLast(head0);
    ListItem *lastItemB = LinkedListGetLast(third0);
    printf("Does LinkedListGetLast() work when the first and or any middle item in list is passed?\n");
    if ((strncmp(lastItemA->data, "Last!", 5) == 0) && (strncmp(lastItemB->data, "Last!", 5) == 0)) {
        printf("    Success!\n");
    } else {
        printf("    Failure, item returned isn't tail of the list. Results: "
               "lastItemA -> %s, lastItemB -> %s\n", lastItemA->data, lastItemB->data);
    }
    free(head0);
    free(second0);
    free(third0);
    free(tail0);
    
    
    ListItem *head1 = LinkedListNew("First..");
    ListItem *second1 = LinkedListCreateAfter(head1, "Second...");
    ListItem *third1 = LinkedListCreateAfter(second1, "Third?");
    ListItem *fourth1 = LinkedListCreateAfter(third1, "Fourth?!");
    ListItem *tail1 = LinkedListCreateAfter(fourth1, "Last!");
    ListItem *lastItem1 = LinkedListGetLast(tail1);
    printf("Does LinkedListGetLast() work when the last item in the list is passed?\n");
    if (strncmp(lastItem1->data, "Last!", 5) == 0) {
        printf("    Success!\n");
    } else {
        printf("    Failure, item returned isn't tail of the list. Result: %s\n", lastItem1->data);
    }
    free(head1);
    free(second1);
    free(third1);
    free(fourth1);
    free(tail1);
#endif
    
    
    // LinkedListSwapData() test
#if 0
    ListItem *head0 = LinkedListNew("First!");
    ListItem *tail0 = LinkedListCreateAfter(head0, "Last!");
    int result0 = LinkedListSwapData(head0, tail0);
    printf("Does LinkedListSwapData() swap the data in two items?\n");
    if (result0 == SUCCESS) {
        printf("    Success! Results: head0 -> %s, tail0 -> %s\n", head0->data, tail0->data);
    } else {
        printf("    Failure, the data wasn't swapped for either item. Results: head0 -> %s, tail0 -> %s\n", head0->data, tail0->data);
    }
    free(head0);
    free(tail0);
    
    
    ListItem *head1 = LinkedListNew("First!");
    ListItem *second1 = LinkedListCreateAfter(head1, NULL);
    ListItem *third1 = LinkedListCreateAfter(second1, "Third!");
    ListItem *tail1 = LinkedListCreateAfter(third1, "Fourth!");
    int result1 = LinkedListSwapData(second1, tail1);
    printf("Does LinkedListSwapData() swap the data of two items if one data item is NULL?\n");
    if (result1 == SUCCESS) {
        printf("    Success! Results: second1 -> %s, tail1 -> %s\n", second1->data, tail1->data);
    } else {
        printf("    Failure, the data was incorrectly swapped. Results: second1 -> %s, tail1 -> %s\n", second1->data, tail1->data);
    }
    free(head1);
    free(second1);
    free(third1);
    free(tail1);
    
    
    ListItem *head2 = LinkedListNew("First!");
    ListItem *middle = NULL;
    ListItem *tail2 = LinkedListCreateAfter(head0, "Last!");
    printf("Does LinkedListSwapData() account for if a ListItem to swap is NULL?\n");
    int result2 = LinkedListSwapData(middle, tail2);
    if (result2 == STANDARD_ERROR) {
        printf("    Success!\n");
    } else {
        printf("    Failure, standard error wasn't accounted for.\n");
    }
    free(head2);
    free(middle);
    free(tail2);
#endif
    
    
    //LinkedListPrint() test
#if 0
    ListItem *head0 = LinkedListNew("Hey");
    ListItem *second0 = LinkedListCreateAfter(head0, "there");
    ListItem *third0 = LinkedListCreateAfter(second0, "I'm");
    ListItem *tail0 = LinkedListCreateAfter(third0, "Mr. Robinson");
    printf("Does LinkedListPrint() print out the linked list of items correctly?\n"
            "Output: ");
    int result0 = LinkedListPrint(second0);
    if (result0 == SUCCESS) {
        printf("    Success!\n");
    } else {
        printf("    Failure, list is NULL. Expected output: {Hey, there, I'm, Mr. Robinson}\n");
    }
    free(head0);
    free(second0);
    free(third0);
    free(tail0);
    
    
    ListItem *head1 = LinkedListNew("Hey");
    ListItem *second1 = LinkedListCreateAfter(head1, "I think");
    ListItem *third1 = LinkedListCreateAfter(second1, "it's");
    ListItem *tail1 = LinkedListCreateAfter(third1, "working!");
    printf("Does LinkedListPrint() print out the linked list of items correctly?\n"
            "Output: ");
    int result1 = LinkedListPrint(tail1);
    if (result1 == SUCCESS) {
        printf("    Success!\n");
    } else {
        printf("    Failure, list is NULL. Expected output: {Hey, I think, it's, working!}\n");
    }
    free(head1);
    free(second1);
    free(third1);
    free(tail1);
#endif
    
    BOARD_End();
    while (1);
    return 0;
}