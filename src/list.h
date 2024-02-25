#ifndef LIST_H
#define LIST_H

/* Circular doubly linked list implementation
 *
 * These macros assume that all node elements have a /next/ and /prev/ element
 * like this:
 * struct node {
 * 	struct node *next;
 * 	struct node *prev;
 * 	--SOME DATA--
 * };
 *
 * LISTs are expected to be double pointers like /struct node ** / because
 * where the start of the list points to could change when inserting/deleting
 * nodes.
 *
 * NODEs are expected to be single pointers like /struct node * /.
 */

/* Insert a node into the end of a list
 *
 * This operation will make NODE be the last node in the list.
 */
#define list_insert_last(LIST, NODE) {\
	if(*(LIST) == (void*) 0) {\
		*(LIST) = (NODE);\
		(NODE)->next = (NODE);\
		(NODE)->prev = (NODE);\
	}\
	else {\
		(NODE)->next = *(LIST);\
		(NODE)->prev = (*(LIST))->prev;\
		(*(LIST))->prev->next = (NODE);\
		(*(LIST))->prev = (NODE);\
	}\
}

/* Insert a node into the start of a list
 *
 * This operation will make NODE be the first node in the list.
 *
 * Because of the circular nature of this list type, this is equivalent to
 * calling list_insert_last and updating the head of the list to NODE.
 */
#define list_insert_first(LIST, NODE) {\
	list_insert_last(LIST, NODE)\
	*LIST = NODE;\
}

/* Remove a node from a list
 *
 * This might change the head of the list when it is the one being removed.
 */
#define list_remove(LIST, NODE) {\
	if(*(LIST) == (NODE)) *(LIST) = (NODE)->next;\
	if(*(LIST) == (NODE)) *(LIST) = (void*) 0;\
	else {\
		(NODE)->next->prev = (NODE)->prev;\
		(NODE)->prev->next = (NODE)->next;\
	}\
}

/* Check whether a list is empty or not
 *
 * This macro evaluates to 1 if the list is empty.
 */
#define list_is_empty(LIST) (*(LIST) == (void*) 0)

/* Iterate through each entry in the list
 *
 * NODE is the iterator letting you access the current node.
 *
 * Example:
 * struct node *list, *node;
 * list_foreach(&list, node) {
 * 	printf("%d\n", node->some_value);
 * }
 */
#define list_foreach(LIST, NODE) for((NODE) = *(LIST);\
		(NODE);\
		(NODE) = ((NODE)->next == *(LIST) ? (void*) 0 : (NODE)->next))

/* Iterate through each entry in the list in reverse order
 *
 * NODE is the iterator letting you access the current node.
 *
 * Example:
 * struct node *list, *node;
 * list_foreach_reverse(&list, node) {
 * 	printf("%d\n", node->some_value);
 * }
 */
#define list_foreach_reverse(LIST, NODE) \
	for((NODE) = (*LIST) ? (*(LIST))->prev : (void*) 0;\
		(NODE);\
		(NODE) = ((NODE)->prev == (*(LIST))->prev ? (void*) 0 : \
			(NODE)->prev))

/* Iterate through each entry in the list safely
 *
 * Like list_foreach but entries can be removed safely.
 *
 * NODE is the iterator letting you access the current node.
 * TMP is another temporary value of the same type as NODE.
 *
 * Note that when removing entries, the LIST pointer may be changed while
 * iterating through the list. This is usually not an issue.
 *
 * Example:
 * struct node *list, *node, *tmp;
 * list_foreach_safe(&list, node, tmp) {
 * 	printf("%d\n", node->some_value);
 * }
 *
 * TECHNICAL STUFF:
 * This macro uses a bit of trickery with the boolean eval part of the for loop.
 * C still evaluates both sides of a boolean OR, even when one of them is
 * already true. Additionally a for loop is essentially a while loop:
 *
 * for(i = 0; i < 10; i++) {--CODE--} is pretty much equivalent to:
 * i = 0; while(i < 10) {--CODE--; i++}
 * This in turn is equivalent to:
 * --------------------------
 * i = 0;
 * top:
 * if(!(i < 10)) goto done; // we can execute code here
 * --CODE--
 * i++; // we can execute here
 * goto top;
 * done:
 * --------------------------
 * As such using the boolean eval and update expressions we can effectively
 * execute at the top and bottom of the loop only using a for expression.
 * Using this, it is possible to update TMP at the top of the loop and to set
 * NODE to TMP at the bottom of the loop, which is necessary for a safe version
 * of foreach.
 */
#define list_foreach_safe(LIST, NODE, TMP) for((NODE) = *(LIST);\
		(NODE) && (\
		((TMP) = (NODE)->next == *(LIST) ? (void*) 0 : (NODE)->next)\
		|| 1);\
		(NODE) = (TMP))

/* Iterate in reverse order through each entry in the list safely
 *
 * Like list_foreach_reverse but entries can be removed safely.
 *
 * NODE is the iterator letting you access the current node.
 * TMP is another temporary value of the same type as NODE.
 *
 * Example:
 * struct node *list, *node, *tmp;
 * list_foreach_reverse_safe(&list, node, tmp) {
 * 	printf("%d\n", node->some_value);
 * }
 */
#define list_foreach_reverse_safe(LIST, NODE, TMP)\
	for((NODE) = (*(LIST))->prev;\
		(NODE) && (\
		((TMP) = (NODE)->prev == (*(LIST))->prev ? \
		(void*) 0 : (NODE)->prev)\
		|| 1);\
		(NODE) = (TMP))

#endif
