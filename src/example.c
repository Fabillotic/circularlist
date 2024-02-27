#include <stdio.h>
#include <stdlib.h>
#include "list.h"

struct node {
	// List components
	struct node *next;
	struct node *prev;

	// Struct data
	int value;
};

int main() {
	int i;
	struct node *list, *node, *tmp;

	// Initialize the list
	// Empty lists are just NULL pointers
	list = NULL;

	// Create 16 nodes and add them to the list
	for(i = 0; i < 16; i++) {
		node = calloc(1, sizeof(struct node));
		node->value = i + 1;

		list_insert_last(&list, node);
	}

	// Iterate through the list
	// Use this instead of manually iterating
	list_foreach(&list, node) {
		printf("%d\n", node->value);
	}

	// Iterate through the list to remove all nodes
	// Note the safe version! You can't remove nodes using non-safe foreach.
	// list_foreach_safe requires an additional temporary iterator.
	list_foreach_safe(&list, node, tmp) {
		list_remove(&list, node);
		free(node);
	}

	return 0;
}
