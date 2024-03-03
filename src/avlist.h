#ifndef AVLIST_H
#define AVLIST_H

/**************************** AVLIST functionality ****************************/
/* These macros require avlist.c to be compiled alongside your code           */

/* Get the amount of nodes in a list
 *
 * This iterates over the entire list, so use it only when necessary.
 */
#define list_size(LIST) _list_size((LIST))
int _list_size(void *list);

/* Get a node from a list
 *
 * This iterates over the entire list, so use it only when necessary.
 */
#define list_get(LIST, INDEX) _list_get((LIST), INDEX)
void* _list_get(void *list, int index);
/**************************** AVLIST functionality ****************************/

#endif
