#ifndef LIST_H
#define LIST_H

/* circularlist v2.0.0
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
 * These dynamic array macros expect a structure like the one generated by
 * dyn_array(TYPE) passed as a pointer.
 *
 * The structs have three `size_t` members (`count`, `alloc`, `esize`)
 * as well as a last member called `data`, which is a pointer type of
 * the entry elements and is automatically allocated, reallocated and
 * free'd by these macros.
 *
 * Dynamic arrays are heap-allocated C-arrays that hold elements of the
 * same type and automatically reallocate when more space is needed.
 *
 * The arrays are represented as structs with metadata and the data
 * pointer to the actual array elements.
 *
 * To initialize the dynamic array, use array_init.
 */

/* Convenience macro to define a dynamic array structure
 *
 * `TYPE` can only be a syntactically simple data type like `int`, `float`,
 * `struct whatever`, `struct whateverelse*`, etc., because this macro naively
 * adds an asterisk at the of the type to make it be a pointer type, which
 * fails for things such as function pointers.
 *
 * In cases where this isn't possible, simple create a struct just like
 * this macro generates but adjust the `data` pointer to be of the
 * correct pointer type to the object you want to store.
 */
#define dyn_array(TYPE) struct {\
	size_t count; /* amount of elements in array */ \
	size_t alloc; /* amount of elements allocated */ \
	size_t esize; /* bytes in a single element */ \
	TYPE* data;   /* data pointer */ \
}

/* Initialize dynamic array
 *
 * array_init MUST be invoked before the array is used in any other macros.
 *
 * Example:
 * dyn_array(int) values;
 * array_init(&values, int);
 */
#define array_init(A, TYPE) {\
	(A)->esize = sizeof(TYPE);\
	(A)->count = 0;\
	(A)->alloc = 0;\
	(A)->data = (void*) 0;\
}

/* Free dynamic array data
 *
 * This convenience macro free's the data pointer of the dynamic array, when
 * it is not NULL and after free'ing updates it to NULL.
 * Additionally, this macro also resets the `count` and `alloc` fields of the
 * dynamic array structure.
 *
 * This macro DOES NOT attempt to free the dynamic array structure, because it
 * might only be part of another structure or a local variable on the stack.
 *
 * Because all of the fields are reset to an initial state and the structure
 * itself will not be free'd, this macro also serves effectively serves the
 * functionality of fully clearing the array.
 */
#define array_free(A) {\
	if((A)->data) {\
		free((A)->data);\
		(A)->data = (void*) 0;\
	}\
	(A)->count = 0;\
	(A)->alloc = 0;\
}

/* Remove all elements from the dynamic array and reset it to an initial state
 *
 * This macro simply evaluates to array_free for reasons explained in that
 * macros documentation.
 *
 * It should be used to clarify the semantic difference between destroying
 * the dynamic array and freeing up all of its resources and simply
 * clearing it for later use.
 */
#define array_clear(A) array_free(A)

/* Reserve a certain amount of elements
 *
 * This macro reserves the space for at least R elements in the array.
 */
#define array_reserve(A, R) {\
	while((A)->alloc < (R)) {\
		if((A)->alloc == 0) {\
			(A)->alloc = 8;\
			(A)->data = malloc((A)->alloc * (A)->esize);\
			continue;\
		}\
		(A)->alloc *= 2;\
		(A)->data = realloc((A)->data, (A)->alloc * (A)->esize);\
	}\
}

/* Add a value to the end of an array
 */
#define array_append(A, E) {\
	array_reserve(A, (A)->count + 1);\
	(A)->data[(A)->count++] = (E);\
}

#endif
