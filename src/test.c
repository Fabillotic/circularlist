#include <stdio.h>
#include <stdlib.h>
#include "list.h"

struct element {
	struct element *next;
	struct element *prev;
	int number;
};

int main() {
	struct element *list, *e, *tmp, *element_A, *element_B, *element_C;

	list = NULL;
	e = NULL;
	tmp = NULL;
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
	printf("\nElements:\n");
	list_foreach_safe(&list, e, tmp) {
		printf("\t%d, %d\n", e->number, tmp ? tmp->number : -1);
		list_remove(&list, e);
		free(e);
	}
	printf("\nElements:\n");
	list_foreach(&list, e) {
		printf("\t%d\n", e->number);
	};

	element_A = calloc(1, sizeof(struct element));
	element_A->number = 1;
	list_insert_first(&list, element_A);

	element_B = calloc(1, sizeof(struct element));
	element_B->number = 2;
	list_insert_first(&list, element_B);

	element_C = calloc(1, sizeof(struct element));
	element_C->number = 3;
	list_insert_first(&list, element_C);

	printf("\nElements:\n");
	list_foreach(&list, e) {
		printf("\t%d\n", e->number);
	};

	list_foreach_safe(&list, e, tmp) {
		list_remove(&list, e);
		free(e);
	}
	printf("\nElements:\n");
	list_foreach(&list, e) {
		printf("\t%d\n", e->number);
	};

	return 0;
}
