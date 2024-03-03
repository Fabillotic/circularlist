#include "list.h"

struct _list_node {
	struct _list_node *next;
	struct _list_node *prev;
};

/* Get the amount of nodes in a list
 *
 * This iterates over the entire list, so use it only when necessary.
 */
#define list_size(LIST) _list_size((LIST))
int _list_size(void *list) {
	int s;
	struct _list_node **nlist, *node;

	s = 0;
	nlist = (struct _list_node**) list;

	list_foreach(nlist, node) s++;

	return s;
}

/* Get a node from a list
 *
 * This iterates over the entire list, so use it only when necessary.
 */
#define list_get(LIST, INDEX) _list_get((LIST), INDEX)
void* _list_get(void *list, int index) {
	int i;
	struct _list_node **nlist, *node;

	i = 0;
	nlist = (struct _list_node**) list;

	list_foreach(nlist, node) {
		if(i == index) break;
		i++;
	}

	return (void*) node;
}
