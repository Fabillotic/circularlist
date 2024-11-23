#ifndef LIST_H
#define LIST_H

/* circularlist v1.2.1
 *
 * Circular doubly linked list implementation
 *
 * The nodes in these lists reference each other with two pointers: One pointing
 * to the next node in the list and one pointing to the previous node in the
 * list. Additionally what sets this type of list apart from the usual
 * non-circular singly linked list is that the last node of the list points to
 * the first node and the first node to the last. This leads to a circular
 * arrangement of nodes like this:
 *
 *  +-----------------------------------------------+
 *  |                                               |
 *  |    +---A---+      +---B---+      +---C---+    |
 *  +--> |   next| ---> |   next| ---> |   next| ---+
 *       |       |      |       |      |       |
 *  +--- |prev   | <--- |prev   | <--- |prev   | <--+
 *  |    +-------+      +-------+      +-------+    |
 *  |                                               |
 *  +-----------------------------------------------+
 *
 * Circular doubly linked lists are more efficient than the usual non-circular
 * singly linked list because inserting or removing a single element can be
 * performed irrelevant of the amount of nodes in the list. In addition to that,
 * both the first and last node can be easily accessed through the list pointer,
 * (when /list/ is the head, /list->prev/ is the tail) and iterating through
 * this type of list in reverse is as trivial as iterating forwards.
 * Furthermore, circular doubly linked lists have an advantage over non-circular
 * doubly linked lists because there is no need to save two list pointers
 * (the head and the tail) and instead only one is necessary.
 *
 * These macros assume that all node elements have a /next/ and /prev/ element
 * like this:
 * struct node {
 * 	struct node *next;
 * 	struct node *prev;
 * 	--SOME DATA--
 * };
 *
 * LISTs are represented as a pointer to the first node in the list. In these
 * macros they're expected to be double pointers like /struct node ** / because
 * where the start of the list points to could change when inserting/deleting
 * nodes. As such, initializing a list means just making a pointer variable and
 * setting it to NULL.
 *
 * NODEs are expected to be single pointers like /struct node * /.
 * They don't require any special initialization, all of this is handled when
 * inserting nodes into the list.
 *
 * These assumptions are more for the purpose of convention than anything else
 * because macros would very much be able to change LISTs even as single
 * pointers and on the other hand NODE pointers are sometimes changed even
 * though they are passed as a single pointer, for example when iterating with
 * list_foreach.
 */

/* BSD Zero Clause License
This specific header file is licensed under the terms of the
BSD Zero Clause License.
The license text goes as follows:

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
PERFORMANCE OF THIS SOFTWARE.
 */

/* Insert a node into the end of a list
 *
 * This operation will make NODE be the last node in the list.
 */
#define list_append(LIST, NODE) {\
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
 * calling list_append and updating the head of the list to NODE.
 */
#define list_prepend(LIST, NODE) {\
	list_append(LIST, NODE)\
	*LIST = NODE;\
}

/* Insert a node after another node
 *
 * Requirements:
 * 	LIST has to have at least one node
 * 	AFTER has to be in LIST
 */
#define list_insert_after(LIST, NODE, AFTER) {\
	(NODE)->prev = (AFTER);\
	(NODE)->next = (AFTER)->next;\
	(AFTER)->next->prev = (NODE);\
	(AFTER)->next = (NODE);\
}

/* Insert a node before another node
 *
 * Requirements:
 * 	LIST has to have at least one node
 * 	BEFORE has to be in LIST
 */
#define list_insert_before(LIST, NODE, BEFORE) {\
	(NODE)->prev = (BEFORE)->prev;\
	(NODE)->next = (BEFORE);\
	(BEFORE)->prev->next = (NODE);\
	(BEFORE)->prev = (NODE);\
	if((BEFORE) == *(LIST)) *(LIST) = (NODE);\
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
	for((NODE) = (*LIST) ? (*(LIST))->prev : (void*) 0;\
		(NODE) && (\
		((TMP) = (NODE)->prev == (*(LIST))->prev ? \
		(void*) 0 : (NODE)->prev)\
		|| 1);\
		(NODE) = (TMP))

/* Iterate through each entry in the list until reaching a certain node
 *
 * NODE is the iterator letting you access the current node.
 * UNTIL will never be visited by the loop.
 *
 * Example visiting all but the last node:
 * struct node *list, *node;
 * list_foreach_until(&list, node, list->prev) {
 * 	printf("%d\n", node->some_value);
 * }
 */
#define list_foreach_until(LIST, NODE, UNTIL) for((NODE) = *(LIST);\
		(NODE);\
		(NODE) = ((NODE)->next == (UNTIL) ? (void*) 0 : (NODE)->next))


/* Dynamic arrays
 *
 *         true memory pointer
 *         V
 * memory: | ARRAY METADATA |[ Element 0 ][ Element 1 ][ Element 2 ]
 *                           ^
 *                           the pointer you get
 *
 * These arrays grow as more and more elements are added.
 * All of the elements contained in the array have to be of the same size.
 *
 * This metadata struct gets put at the front of the allocated memory block.
 * The array pointer starts after this metadata struct, as such the resulting
 * pointer can be used like a normal C array.
 *
 * It is incredibly important that you do not attempt to free the dynamic array
 * by itself! Always use the array_free macro.
 */
struct dyn_array_data {
	int count; // amount of elements in array
	int alloc; // amount of elements allocated
	int esize; // bytes in a single element
};

#define dyn_array_msize sizeof(struct dyn_array_data)

/* Get the pointer to the metadata of the dynamic array
 *
 * This pointer is equivalent to the true memory pointer of the array.
 */
#define array_meta(A) (((struct dyn_array_data*) (void*) (A)) - 1)

/* Reverse of array_meta
 *
 * Calculates the array pointer of the given memory pointer.
 */
#define array_ptr_off(P) ((void*) ((struct dyn_array_data*) (P) + 1))

/* Amount of elements in an array
 */
#define array_len(A) (array_meta(A)->count)

/* Amount of elements allocated by the array
 */
#define array_allocated(A) (array_meta(A)->alloc)

/* Allocate a new empty array
 *
 * Creates a new array and writes the pointer to the memory pointed to by P.
 * T gives the type of the array elements.
 * Thus, P should be of type T**.
 *
 * Example:
 * int* values;
 * array_new(&values, int);
 */
#define array_new(P, T) {\
	*(P) = array_ptr_off(calloc(1, dyn_array_msize)); \
	array_meta((*P))->esize = sizeof(T);\
}

/* Free the array
 */
#define array_free(A) free(array_meta(A))

/* Reserve a certain amount of elements
 *
 * This macro reserves the space for at least R elements in the array.
 */
#define array_reserve(A, R) {\
	while(array_meta(A)->alloc < (R)) {\
		if(array_meta(A)->alloc == 0) array_meta(A)->alloc = 8;\
		else array_meta(A)->alloc *= 2;\
		(A) = array_ptr_off(realloc(array_meta(A), dyn_array_msize + \
				array_meta(A)->alloc * array_meta(A)->esize));\
	}\
}

/* Add a value to the end of an array
 */
#define array_append(A, E) {\
	array_reserve(A, array_len(A) + 1);\
	(A)[array_meta(A)->count++] = (E);\
}

#endif
