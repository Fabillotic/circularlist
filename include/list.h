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

#define list_remove(LIST, NODE) {\
	if(*(LIST) == (NODE)) *(LIST) = (NODE)->next;\
	if(*(LIST) == (NODE)) *(LIST) = (void*) 0;\
	else {\
		(NODE)->next->prev = (NODE)->prev;\
		(NODE)->prev->next = (NODE)->next;\
	}\
}

/* Iterate through each entry in the list
 *
 * NODE is the iterator letting you access the current node.
 * NODE will be reset to NULL at the end.
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
 * NODE will be reset to NULL at the end.
 *
 * Example:
 * struct node *list, *node;
 * list_foreach_backwards(&list, node) {
 * 	printf("%d\n", node->some_value);
 * }
 */
#define list_foreach_backwards(LIST, NODE) for((NODE) = (*(LIST))->prev;\
		(NODE);\
		(NODE) = ((NODE)->prev == (*(LIST))->prev ? (void*) 0 : \
			(NODE)->prev))

#endif
