#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

struct node {
	// List components
	struct node *next;
	struct node *prev;

	// Struct data
	int value;
};

void example_list() {
	int i;
	struct node *list, *node, *tmp;

	// Initialize the list
	// Empty lists are just NULL pointers
	list = NULL;

	// Create 16 nodes and add them to the list
	for(i = 0; i < 16; i++) {
		node = calloc(1, sizeof(struct node));
		node->value = i + 1;

		list_append(&list, node);
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
}

void example_array() {
	int i, n;
	int *vals;
	int othervals[] = {17, 18, 19, 20, 21, 22, 23, 24};

	n = sizeof(othervals) / sizeof(int);

	// create a new int array
	array_new(&vals, int);

	// add 16 values
	for(i = 0; i < 16; i++) {
		array_append(vals, i + 1);
	}

	// manually expand array and copy values
	array_reserve(vals, array_len(vals) + n);
	memcpy(vals + array_len(vals), othervals, sizeof(int) * n);
	array_len(vals) += n;

	// print the values
	for(i = 0; i < array_len(vals); i++) {
		printf("%d\n", vals[i]);
	}

	// free the array
	array_free(vals);
}

int main() {
	printf("List example:\n");
	example_list();

	printf("\n\nArray example:\n");
	example_array();

	return 0;
}
