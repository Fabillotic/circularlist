#include <stdio.h>
#include <stdlib.h>
#include "list.h"

struct element {
	struct element *next;
	struct element *prev;
	int number;
};

int main() {
	struct element *list, *e, *element_A, *element_B, *element_C;

	list = NULL;
	e = NULL;
	element_A = NULL;
	element_B = NULL;
	element_C = NULL;

	element_A = calloc(1, sizeof(struct element));
	element_A->number = 1;
	list_insert_last(&list, element_A);

	printf("After A inserted\n");
	printf("element_A: %d\n", element_A->number);
	printf("element_A->next: %d\n", element_A->next->number);
	printf("element_A->prev: %d\n", element_A->prev->number);
	printf("list: %d\n", list->number);

	element_B = calloc(1, sizeof(struct element));
	element_B->number = 2;
	list_insert_last(&list, element_B);

	printf("\nAfter B inserted\n");
	printf("element_A: %d\n", element_A->number);
	printf("element_A->next: %d\n", element_A->next->number);
	printf("element_A->prev: %d\n", element_A->prev->number);
	printf("element_B: %d\n", element_B->number);
	printf("element_B->next: %d\n", element_B->next->number);
	printf("element_B->prev: %d\n", element_B->prev->number);
	printf("list: %d\n", list->number);

	element_C = calloc(1, sizeof(struct element));
	element_C->number = 3;
	list_insert_last(&list, element_C);

	printf("\nAfter C inserted\n");
	printf("element_A: %d\n", element_A->number);
	printf("element_A->next: %d\n", element_A->next->number);
	printf("element_A->prev: %d\n", element_A->prev->number);
	printf("element_B: %d\n", element_B->number);
	printf("element_B->next: %d\n", element_B->next->number);
	printf("element_B->prev: %d\n", element_B->prev->number);
	printf("element_C: %d\n", element_C->number);
	printf("element_C->next: %d\n", element_C->next->number);
	printf("element_C->prev: %d\n", element_C->prev->number);
	printf("list: %d\n", list->number);

	printf("\nElements:\n");
	list_foreach(&list, e) {
		printf("\t%d\n", e->number);
	};

	list_remove(&list, element_C);

	printf("\nAfter C removed\n");
	printf("element_A: %d\n", element_A->number);
	printf("element_A->next: %d\n", element_A->next->number);
	printf("element_A->prev: %d\n", element_A->prev->number);
	printf("element_B: %d\n", element_B->number);
	printf("element_B->next: %d\n", element_B->next->number);
	printf("element_B->prev: %d\n", element_B->prev->number);
	printf("list: %d\n", list->number);

	list_remove(&list, element_A);

	printf("\nAfter A removed\n");
	printf("element_B: %d\n", element_B->number);
	printf("element_B->next: %d\n", element_B->next->number);
	printf("element_B->prev: %d\n", element_B->prev->number);
	printf("list: %d\n", list->number);

	list_remove(&list, element_B);

	printf("\nAfter B removed\n");
	printf("list: %p\n", list);

	return 0;
}
