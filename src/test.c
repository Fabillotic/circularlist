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

#define TRUE 1
#define FALSE 0

#define tassert(EVAL) if(!(EVAL)) {\
	printf("ERROR! Assertion [%s] failed.\n", #EVAL); return 1;}

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
		printf("Not enough memory! Failed to complete tests.\n");
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
	list_insert_last(&list, elmA);

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
	list_insert_last(&list, elmA);

	tassert(list == elmA);
	tassert(elmA->next == elmA);
	tassert(elmA->prev == elmA);

	elmB = create_element('B');
	list_insert_last(&list, elmB);

	tassert(list == elmA);
	tassert(elmA->next == elmB);
	tassert(elmA->prev == elmB);
	tassert(elmB->next == elmA);
	tassert(elmB->prev == elmA);

	elmC = create_element('C');
	list_insert_last(&list, elmC);

	tassert(list == elmA);
	tassert(elmA->next == elmB);
	tassert(elmA->prev == elmC);
	tassert(elmB->next == elmC);
	tassert(elmB->prev == elmA);
	tassert(elmC->next == elmA);
	tassert(elmC->prev == elmB);

	elmD = create_element('D');
	list_insert_last(&list, elmD);

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

int main() {
	int i, num_tests, failures;

	testfunc_t tests[] = {
		test_empty_list,
		test_single_insert_delete,
		test_insert_four_elements,
	};

	num_tests = sizeof(tests) / sizeof(testfunc_t);

	failures = 0;
	for(i = 0; i < num_tests; i++) {
		printf("Running test %d of %d...\n", i + 1, num_tests);
		if(tests[i]() != 0) {
			printf("Test %d failed!\n", i + 1);
			failures++;
		}
	}

	printf("\nCompleted %d tests with %d errors.\n", num_tests, failures);

	return 0;
}
