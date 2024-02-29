/* Tests for list.h
 *
 * These tests ensure that the macros handle general operation and edge-cases in
 * the manner that they are supposed to.
 *
 * If a test functions returns a non-zero value, it is deemed as an error.
 */

#include <stdio.h>
#include <stdlib.h>
#include "list.h"

typedef int (*testfunc_t)();

struct test {
	char *name;
	testfunc_t func;
};

#define TRUE 1
#define FALSE 0

#define tassert(EVAL) if(!(EVAL)) {\
	printf("\nERROR! Assertion [%s] failed.\n", #EVAL); return 1;}

#define declare_test(TEST) {.name = #TEST, .func = TEST}

/* The struct for these tests
 * Similar structs will also work as well, as long as /next/ and /prev/ are
 * there with the correct type.
 */
struct element {
	struct element *next;
	struct element *prev;
	char id;
};

struct element* create_element(char id) {
	struct element *elm;

	elm = calloc(1, sizeof(struct element));

	if(!elm) {
		printf("\n\nNot enough memory! Failed to complete tests.\n");
		abort();
	}

	elm->id = id;

	return elm;
}

int test_empty_list() {
	struct element *list;

	list = NULL;

	tassert(list_is_empty(&list));

	return 0;
}

int test_single_insert_delete() {
	struct element *list, *elmA;

	list = NULL;

	tassert(list_is_empty(&list));

	elmA = create_element('A');
	list_append(&list, elmA);

	tassert(!list_is_empty(&list));
	tassert(elmA->next == elmA);
	tassert(elmA->prev == elmA);

	list_remove(&list, elmA);
	tassert(list_is_empty(&list));

	free(elmA);

	return 0;
}

int test_insert_four_elements() {
	struct element *list, *elmA, *elmB, *elmC, *elmD;

	list = NULL;

	tassert(list_is_empty(&list));

	elmA = create_element('A');
	list_append(&list, elmA);

	tassert(list == elmA);
	tassert(elmA->next == elmA);
	tassert(elmA->prev == elmA);

	elmB = create_element('B');
	list_append(&list, elmB);

	tassert(list == elmA);
	tassert(elmA->next == elmB);
	tassert(elmA->prev == elmB);
	tassert(elmB->next == elmA);
	tassert(elmB->prev == elmA);

	elmC = create_element('C');
	list_append(&list, elmC);

	tassert(list == elmA);
	tassert(elmA->next == elmB);
	tassert(elmA->prev == elmC);
	tassert(elmB->next == elmC);
	tassert(elmB->prev == elmA);
	tassert(elmC->next == elmA);
	tassert(elmC->prev == elmB);

	elmD = create_element('D');
	list_append(&list, elmD);

	tassert(list == elmA);
	tassert(elmA->next == elmB);
	tassert(elmA->prev == elmD);
	tassert(elmB->next == elmC);
	tassert(elmB->prev == elmA);
	tassert(elmC->next == elmD);
	tassert(elmC->prev == elmB);
	tassert(elmD->next == elmA);
	tassert(elmD->prev == elmC);

	free(elmA);
	free(elmB);
	free(elmC);
	free(elmD);

	return 0;
}

int test_insert_four_elements_reverse() {
	struct element *list, *elmA, *elmB, *elmC, *elmD;

	list = NULL;

	tassert(list_is_empty(&list));

	elmA = create_element('A');
	list_prepend(&list, elmA);

	tassert(list == elmA);
	tassert(elmA->next == elmA);
	tassert(elmA->prev == elmA);

	elmB = create_element('B');
	list_prepend(&list, elmB);

	tassert(list == elmB);
	tassert(elmB->next == elmA);
	tassert(elmB->prev == elmA);
	tassert(elmA->next == elmB);
	tassert(elmA->prev == elmB);

	elmC = create_element('C');
	list_prepend(&list, elmC);

	tassert(list == elmC);
	tassert(elmC->next == elmB);
	tassert(elmC->prev == elmA);
	tassert(elmB->next == elmA);
	tassert(elmB->prev == elmC);
	tassert(elmA->next == elmC);
	tassert(elmA->prev == elmB);

	elmD = create_element('D');
	list_prepend(&list, elmD);

	tassert(list == elmD);
	tassert(elmD->next == elmC);
	tassert(elmD->prev == elmA);
	tassert(elmC->next == elmB);
	tassert(elmC->prev == elmD);
	tassert(elmB->next == elmA);
	tassert(elmB->prev == elmC);
	tassert(elmA->next == elmD);
	tassert(elmA->prev == elmB);

	free(elmA);
	free(elmB);
	free(elmC);
	free(elmD);

	return 0;
}

int test_four_element_removal() {
	struct element *list, *elmA, *elmB, *elmC, *elmD;

	list = NULL;

	elmA = create_element('A');
	list_append(&list, elmA);
	elmB = create_element('B');
	list_append(&list, elmB);
	elmC = create_element('C');
	list_append(&list, elmC);
	elmD = create_element('D');
	list_append(&list, elmD);

	tassert(list == elmA);

	list_remove(&list, elmA);

	tassert(list == elmB);
	tassert(elmB->next == elmC);
	tassert(elmB->prev == elmD);
	tassert(elmC->next == elmD);
	tassert(elmC->prev == elmB);
	tassert(elmD->next == elmB);
	tassert(elmD->prev == elmC);

	list_remove(&list, elmB);

	tassert(list == elmC);
	tassert(elmC->next == elmD);
	tassert(elmC->prev == elmD);
	tassert(elmD->next == elmC);
	tassert(elmD->prev == elmC);

	list_remove(&list, elmC);

	tassert(list == elmD);
	tassert(elmD->next == elmD);
	tassert(elmD->prev == elmD);

	list_remove(&list, elmD);

	tassert(list_is_empty(&list));

	free(elmA);
	free(elmB);
	free(elmC);
	free(elmD);

	return 0;
}

int test_iteration() {
	int i;
	struct element *list, *tmp, *elmA, *elmB, *elmC, *elmD, *elmE;

	list = NULL;

	elmA = create_element('A');
	list_append(&list, elmA);
	elmB = create_element('B');
	list_append(&list, elmB);
	elmC = create_element('C');
	list_append(&list, elmC);
	elmD = create_element('D');
	list_append(&list, elmD);
	elmE = create_element('E');
	list_append(&list, elmE);

	i = 0;
	list_foreach(&list, tmp) {
		switch(i) {
			case 0: tassert(tmp == elmA); break;
			case 1: tassert(tmp == elmB); break;
			case 2: tassert(tmp == elmC); break;
			case 3: tassert(tmp == elmD); break;
			case 4: tassert(tmp == elmE); break;
			default: tassert(FALSE && "unreachable"); break;
		}
		i++;
	}

	free(elmA);
	free(elmB);
	free(elmC);
	free(elmD);
	free(elmE);

	return 0;
}

int test_iteration_reverse() {
	int i;
	struct element *list, *tmp, *elmA, *elmB, *elmC, *elmD, *elmE;

	list = NULL;

	elmA = create_element('A');
	list_append(&list, elmA);
	elmB = create_element('B');
	list_append(&list, elmB);
	elmC = create_element('C');
	list_append(&list, elmC);
	elmD = create_element('D');
	list_append(&list, elmD);
	elmE = create_element('E');
	list_append(&list, elmE);

	i = 0;
	list_foreach_reverse(&list, tmp) {
		switch(i) {
			case 0: tassert(tmp == elmE); break;
			case 1: tassert(tmp == elmD); break;
			case 2: tassert(tmp == elmC); break;
			case 3: tassert(tmp == elmB); break;
			case 4: tassert(tmp == elmA); break;
			default: tassert(FALSE && "unreachable"); break;
		}
		i++;
	}

	free(elmA);
	free(elmB);
	free(elmC);
	free(elmD);
	free(elmE);

	return 0;
}

int test_empty_iteration() {
	struct element *list, *tmp, *tmp2;

	list = NULL;

	list_foreach(&list, tmp) {
		tassert(FALSE && "unreachable");
	}

	list_foreach_reverse(&list, tmp) {
		tassert(FALSE && "unreachable");
	}

	list_foreach_safe(&list, tmp, tmp2) {
		tassert(FALSE && "unreachable");
	}

	list_foreach_reverse_safe(&list, tmp, tmp2) {
		tassert(FALSE && "unreachable");
	}

	return 0;
}

int test_single_iteration() {
	int i;
	struct element *list, *e, *tmp;

	list = NULL;

	e = create_element('E');
	list_append(&list, e);

	i = 0;
	list_foreach(&list, tmp) {
		tassert(tmp == e);
		tassert(i == 0);
		i++;
	}

	i = 0;
	list_foreach_reverse(&list, tmp) {
		tassert(tmp == e);
		tassert(i == 0);
		i++;
	}

	free(e);

	return 0;
}

int test_iteration_removal() {
	int i;
	struct element *list, *node, *tmp, *elmA, *elmB, *elmC, *elmD, *elmE;

	list = NULL;

	elmA = create_element('A');
	list_append(&list, elmA);
	elmB = create_element('B');
	list_append(&list, elmB);
	elmC = create_element('C');
	list_append(&list, elmC);
	elmD = create_element('D');
	list_append(&list, elmD);
	elmE = create_element('E');
	list_append(&list, elmE);

	i = 0;
	list_foreach_safe(&list, node, tmp) {
		switch(i) {
			case 0: tassert(node == elmA); break;
			case 1: tassert(node == elmB); break;
			case 2: tassert(node == elmC); break;
			case 3: tassert(node == elmD); break;
			case 4: tassert(node == elmE); break;
			default: tassert(FALSE && "unreachable"); break;
		}
		list_remove(&list, node);
		free(node);
		i++;
	}

	tassert(list_is_empty(&list));

	return 0;
}

int test_iteration_removal_reverse() {
	int i;
	struct element *list, *node, *tmp, *elmA, *elmB, *elmC, *elmD, *elmE;

	list = NULL;

	elmA = create_element('A');
	list_append(&list, elmA);
	elmB = create_element('B');
	list_append(&list, elmB);
	elmC = create_element('C');
	list_append(&list, elmC);
	elmD = create_element('D');
	list_append(&list, elmD);
	elmE = create_element('E');
	list_append(&list, elmE);

	i = 0;
	list_foreach_reverse_safe(&list, node, tmp) {
		switch(i) {
			case 0: tassert(node == elmE); break;
			case 1: tassert(node == elmD); break;
			case 2: tassert(node == elmC); break;
			case 3: tassert(node == elmB); break;
			case 4: tassert(node == elmA); break;
			default: tassert(FALSE && "unreachable"); break;
		}
		list_remove(&list, node);
		free(node);
		i++;
	}

	tassert(list_is_empty(&list));

	return 0;
}

int test_insert_after() {
	int i;
	struct element *list, *tmp, *elmA, *elmB, *elmC, *elmD, *elmE;

	list = NULL;

	elmA = create_element('A');
	list_append(&list, elmA);

	elmB = create_element('B');
	list_insert_after(&list, elmB, elmA);

	elmC = create_element('C');
	list_append(&list, elmC);

	elmE = create_element('E');
	list_insert_after(&list, elmE, elmB);

	elmD = create_element('D');
	list_append(&list, elmD);

	i = 0;
	list_foreach(&list, tmp) {
		switch(i) {
			case 0: tassert(tmp == elmA); break;
			case 1: tassert(tmp == elmB); break;
			case 2: tassert(tmp == elmE); break;
			case 3: tassert(tmp == elmC); break;
			case 4: tassert(tmp == elmD); break;
			default: tassert(FALSE && "unreachable"); break;
		}
		i++;
	}

	free(elmA);
	free(elmB);
	free(elmC);
	free(elmD);
	free(elmE);

	return 0;
}

int test_insert_before() {
	int i;
	struct element *list, *tmp, *elmA, *elmB, *elmC, *elmD, *elmE;

	list = NULL;

	elmA = create_element('A');
	list_append(&list, elmA);

	elmB = create_element('B');
	list_insert_before(&list, elmB, elmA);

	elmC = create_element('C');
	list_append(&list, elmC);

	elmE = create_element('E');
	list_insert_before(&list, elmE, elmC);

	elmD = create_element('D');
	list_append(&list, elmD);

	i = 0;
	list_foreach(&list, tmp) {
		switch(i) {
			case 0: tassert(tmp == elmB); break;
			case 1: tassert(tmp == elmA); break;
			case 2: tassert(tmp == elmE); break;
			case 3: tassert(tmp == elmC); break;
			case 4: tassert(tmp == elmD); break;
			default: tassert(FALSE && "unreachable"); break;
		}
		i++;
	}

	free(elmA);
	free(elmB);
	free(elmC);
	free(elmD);
	free(elmE);

	return 0;
}

int test_iteration_end() {
	int i;
	struct element *list, *tmp, *tmp2, *elmA, *elmB, *elmC, *elmD, *elmE;

	list = NULL;

	elmA = create_element('A');
	list_append(&list, elmA);
	elmB = create_element('B');
	list_append(&list, elmB);
	elmC = create_element('C');
	list_append(&list, elmC);
	elmD = create_element('D');
	list_append(&list, elmD);
	elmE = create_element('E');
	list_append(&list, elmE);

	i = 0;
	list_foreach(&list, tmp) {
		switch(i) {
			case 0: tassert(tmp == elmA); break;
			case 1: tassert(tmp == elmB); break;
			case 2: tassert(tmp == elmC); break;
			case 3: tassert(tmp == elmD); break;
			case 4: tassert(tmp == elmE); break;
			default: tassert(FALSE && "unreachable"); break;
		}
		i++;
	}
	tassert(tmp == NULL);

	i = 0;
	list_foreach_reverse(&list, tmp) {
		switch(i) {
			case 0: tassert(tmp == elmE); break;
			case 1: tassert(tmp == elmD); break;
			case 2: tassert(tmp == elmC); break;
			case 3: tassert(tmp == elmB); break;
			case 4: tassert(tmp == elmA); break;
			default: tassert(FALSE && "unreachable"); break;
		}
		i++;
	}
	tassert(tmp == NULL);

	i = 0;
	list_foreach_safe(&list, tmp, tmp2) {
		switch(i) {
			case 0: tassert(tmp == elmA); break;
			case 1: tassert(tmp == elmB); break;
			case 2: tassert(tmp == elmC); break;
			case 3: tassert(tmp == elmD); break;
			case 4: tassert(tmp == elmE); break;
			default: tassert(FALSE && "unreachable"); break;
		}
		i++;
	}
	tassert(tmp == NULL);

	i = 0;
	list_foreach_reverse_safe(&list, tmp, tmp2) {
		switch(i) {
			case 0: tassert(tmp == elmE); break;
			case 1: tassert(tmp == elmD); break;
			case 2: tassert(tmp == elmC); break;
			case 3: tassert(tmp == elmB); break;
			case 4: tassert(tmp == elmA); break;
			default: tassert(FALSE && "unreachable"); break;
		}
		i++;
	}
	tassert(tmp == NULL);

	free(elmA);
	free(elmB);
	free(elmC);
	free(elmD);
	free(elmE);

	return 0;
}

int test_size() {
	struct element *list, *elmA, *elmB, *elmC, *elmD, *elmE;

	list = NULL;

	elmA = create_element('A');
	list_append(&list, elmA);
	elmB = create_element('B');
	list_append(&list, elmB);
	elmC = create_element('C');
	list_append(&list, elmC);
	elmD = create_element('D');
	list_append(&list, elmD);
	elmE = create_element('E');
	list_append(&list, elmE);

	tassert(list_size(&list) == 5);

	free(elmA);
	free(elmB);
	free(elmC);
	free(elmD);
	free(elmE);

	return 0;
}

int test_get() {
	struct element *list, *elmA, *elmB, *elmC, *elmD, *elmE;

	list = NULL;

	elmA = create_element('A');
	list_append(&list, elmA);
	elmB = create_element('B');
	list_append(&list, elmB);
	elmC = create_element('C');
	list_append(&list, elmC);
	elmD = create_element('D');
	list_append(&list, elmD);
	elmE = create_element('E');
	list_append(&list, elmE);

	tassert(list_get(&list, 0) == elmA);
	tassert(list_get(&list, 1) == elmB);
	tassert(list_get(&list, 2) == elmC);
	tassert(list_get(&list, 3) == elmD);
	tassert(list_get(&list, 4) == elmE);

	free(elmA);
	free(elmB);
	free(elmC);
	free(elmD);
	free(elmE);

	return 0;
}

int main() {
	int i, num_tests, failures;

	struct test tests[] = {
		declare_test(test_empty_list),
		declare_test(test_single_insert_delete),
		declare_test(test_insert_four_elements),
		declare_test(test_insert_four_elements_reverse),
		declare_test(test_four_element_removal),
		declare_test(test_iteration),
		declare_test(test_iteration_reverse),
		declare_test(test_empty_iteration),
		declare_test(test_single_iteration),
		declare_test(test_iteration_removal),
		declare_test(test_iteration_removal_reverse),
		declare_test(test_insert_after),
		declare_test(test_insert_before),
		declare_test(test_iteration_end),
		declare_test(test_size),
		declare_test(test_get),
	};

	num_tests = sizeof(tests) / sizeof(struct test);

	failures = 0;
	for(i = 0; i < num_tests; i++) {
		printf("Running test '%s' (%d of %d)...\n", tests[i].name,
				i + 1, num_tests);
		if(tests[i].func() != 0) {
			printf("Test %d failed!\n\n", i + 1);
			failures++;
		}
	}

	printf("\nCompleted %d tests with %d errors.\n", num_tests, failures);

	return 0;
}
