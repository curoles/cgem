#pragma once

#include <stdbool.h>
#include <assert.h>

/**
 *
 * Example:
 * struct MyList {
 *     ListNode list;
 *     T data;
 * };
 */
typedef struct ListNode
{
    struct ListNode* prev;
    struct ListNode* next;
} ListNode;

/**
 * Tests whether a list is empty.
 * @param node the list to test.
 */
static inline
bool List_isEmpty(ListNode* node)
{
    return node->next == node;
}

/** Set both fields prev and next to point on the node itself, 
 *  which means the list is empty.
 */
static inline
void List_init(ListNode* node)
{
    node->prev = node;
    node->next = node;

    assert( List_isEmpty(node) );
}

/**
 * If the structure is statically created at compile time,
 * and you have a direct reference to it, you can simply do this:
 *
 * struct my_struct mine = {
 *     .list  = LIST_NODE_INIT(mine.list),
 *     .dog   = 0,
 *     .cat   = NULL
 * };
 */
#define LIST_NODE_INIT(name) { &(name), &(name) }

/**
 *
 */
#define LIST_HEAD(name) \
    struct ListNode name = LIST_NODE_INIT(name)

/*
 * Insert a new entry between two known consecutive entries. 
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline
void List_insert__(
    ListNode* new_node,
    ListNode* prev,
    ListNode* next
)
{
    next->prev = new_node; //          new_node <-  next
    new_node->next = next; //          new_node  -> next
    new_node->prev = prev; // prev <-  new_node
    prev->next = new_node; // prev  -> new_node
                           // prev <-> new_node <-> next
}

/**
 * List_insertAfter - add a new entry
 * @param new_node new entry to be added
 * @param node list head to add new_node after
 *
 * Insert a new entry after the specified head.
 * This is good for implementing stacks.
 */
static inline
void List_insertAfter(ListNode* node, ListNode* new_node)
{
    // node <-> new_node <-> node.next
    List_insert__(new_node, node, node->next);
}

/**
 * List_insertBefore - add a new entry
 * @param new_node new entry to be added
 * @param node list head to add new_node before
 *
 * Insert a new entry before the specified head.
 * This is useful for implementing queues.
 */
static inline
void List_insertBefore(ListNode* node, ListNode* new_node)
{
    // node.prev <-> new_node <-> node
    List_insert__(new_node, node->prev, node);
}

/**
 * List_add - add a new entry
 * @param new_node new entry to be added
 * @param node list head to add new_node after
 *
 * Alias for List_insertAfter.
 * Insert a new entry after the specified head.
 * This is good for implementing stacks.
 */
static inline
void List_add(ListNode* node, ListNode* new_node)
{
    List_insertAfter(node, new_node);
}


/*
 * Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline
void List_connectNodes__(ListNode* prev, ListNode* next)
{
    next->prev = prev;
    prev->next = next;
}

/**
 * list_del - deletes entry from list.
 * @entry: the element to delete from the list.
 * Note: list_empty on entry does not return true after this, the entry is in an undefined state.
 */
static inline
void List_delete(ListNode* node)
{
    List_connectNodes__(node->prev, node->next);
    node->next = (void *) 0;
    node->prev = (void *) 0;
}



/**
 * list_entry - get the struct for this entry
 * @ptr:	the &struct list_head pointer.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the list_struct within the struct.
 */
#define List_entry(ptr, type, member) \
    ((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))

/**
 * list_for_each	-	iterate over a list
 * @pos:	the &struct list_head to use as a loop counter.
 * @head:	the head for your list.
 */
#define List_forEach(head, iterator) \
    for (iterator = (head)->next; iterator != (head); \
         iterator = iterator->next)

/**
 * list_for_each_prev	-	iterate over a list backwards
 * @pos:	the &struct list_head to use as a loop counter.
 * @head:	the head for your list.
 */
#define List_forEachReverse(head, pos) \
    for (pos = (head)->prev; pos != (head); \
         pos = pos->prev)


/**
 * list_for_each_entry	-	iterate over list of given type
 * @pos:	the type * to use as a loop counter.
 * @head:	the head for your list.
 * @member:	the name of the list_struct within the struct.
 */
#define List_forEachEntry(head, member, pos)                 \
    for (pos = List_entry((head)->next, typeof(*pos), member); \
        &pos->member != (head);                                \
         pos = List_entry(pos->member.next, typeof(*pos), member))

/**
 * list_for_each_entry_safe - iterate over list of given type safe against removal of list entry
 * @pos:	the type * to use as a loop counter.
 * @n:		another type * to use as temporary storage
 * @head:	the head for your list.
 * @member:	the name of the list_struct within the struct.
 */
#define List_forEachEntrySafe(head, member, pos, n)			\
	for (pos = List_entry((head)->next, typeof(*pos), member),	\
		n = List_entry(pos->member.next, typeof(*pos), member);	\
	     &pos->member != (head); 					\
	     pos = n, n = List_entry(n->member.next, typeof(*n), member))




#if 0
#ifndef __LIST_H
#define __LIST_H

/* This file is from Linux Kernel (include/linux/list.h) 
 * Here by copyright, credits attributed to wherever they belong.
 */

/*
 * Simple doubly linked list implementation.
 *
 * Some of the internal functions ("__xxx") are useful when
 * manipulating whole lists rather than single entries, as
 * sometimes we already know the next/prev entries and we can
 * generate better code by using them directly rather than
 * using the generic single-entry routines.
 */

struct list_head {
	struct list_head *next, *prev;
};

#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
	struct list_head name = LIST_HEAD_INIT(name)

#define INIT_LIST_HEAD(ptr) do { \
	(ptr)->next = (ptr); (ptr)->prev = (ptr); \
} while (0)

/*
 * Insert a new entry between two known consecutive entries. 
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __list_add(struct list_head *new,
			      struct list_head *prev,
			      struct list_head *next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}

/**
 * list_add - add a new entry
 * @new: new entry to be added
 * @head: list head to add it after
 *
 * Insert a new entry after the specified head.
 * This is good for implementing stacks.
 */
static inline void list_add(struct list_head *new, struct list_head *head)
{
	__list_add(new, head, head->next);
}

/**
 * list_add_tail - add a new entry
 * @new: new entry to be added
 * @head: list head to add it before
 *
 * Insert a new entry before the specified head.
 * This is useful for implementing queues.
 */
static inline void list_add_tail(struct list_head *new, struct list_head *head)
{
	__list_add(new, head->prev, head);
}

/*
 * Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __list_del(struct list_head *prev, struct list_head *next)
{
	next->prev = prev;
	prev->next = next;
}

/**
 * list_del - deletes entry from list.
 * @entry: the element to delete from the list.
 * Note: list_empty on entry does not return true after this, the entry is in an undefined state.
 */
static inline void list_del(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
	entry->next = (void *) 0;
	entry->prev = (void *) 0;
}

/**
 * list_del_init - deletes entry from list and reinitialize it.
 * @entry: the element to delete from the list.
 */
static inline void list_del_init(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
	INIT_LIST_HEAD(entry); 
}

/**
 * list_move - delete from one list and add as another's head
 * @list: the entry to move
 * @head: the head that will precede our entry
 */
static inline void list_move(struct list_head *list, struct list_head *head)
{
        __list_del(list->prev, list->next);
        list_add(list, head);
}

/**
 * list_move_tail - delete from one list and add as another's tail
 * @list: the entry to move
 * @head: the head that will follow our entry
 */
static inline void list_move_tail(struct list_head *list,
				  struct list_head *head)
{
        __list_del(list->prev, list->next);
        list_add_tail(list, head);
}

/**
 * list_empty - tests whether a list is empty
 * @head: the list to test.
 */
static inline int list_empty(struct list_head *head)
{
	return head->next == head;
}

static inline void __list_splice(struct list_head *list,
				 struct list_head *head)
{
	struct list_head *first = list->next;
	struct list_head *last = list->prev;
	struct list_head *at = head->next;

	first->prev = head;
	head->next = first;

	last->next = at;
	at->prev = last;
}

/**
 * list_splice - join two lists
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 */
static inline void list_splice(struct list_head *list, struct list_head *head)
{
	if (!list_empty(list))
		__list_splice(list, head);
}

/**
 * list_splice_init - join two lists and reinitialise the emptied list.
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 *
 * The list at @list is reinitialised
 */
static inline void list_splice_init(struct list_head *list,
				    struct list_head *head)
{
	if (!list_empty(list)) {
		__list_splice(list, head);
		INIT_LIST_HEAD(list);
	}
}

/**
 * list_entry - get the struct for this entry
 * @ptr:	the &struct list_head pointer.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the list_struct within the struct.
 */
#define list_entry(ptr, type, member) \
	((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))

/**
 * list_for_each	-	iterate over a list
 * @pos:	the &struct list_head to use as a loop counter.
 * @head:	the head for your list.
 */
#define list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); \
        	pos = pos->next)
/**
 * list_for_each_prev	-	iterate over a list backwards
 * @pos:	the &struct list_head to use as a loop counter.
 * @head:	the head for your list.
 */
#define list_for_each_prev(pos, head) \
	for (pos = (head)->prev; pos != (head); \
        	pos = pos->prev)
        	
/**
 * list_for_each_safe	-	iterate over a list safe against removal of list entry
 * @pos:	the &struct list_head to use as a loop counter.
 * @n:		another &struct list_head to use as temporary storage
 * @head:	the head for your list.
 */
#define list_for_each_safe(pos, n, head) \
	for (pos = (head)->next, n = pos->next; pos != (head); \
		pos = n, n = pos->next)

/**
 * list_for_each_entry	-	iterate over list of given type
 * @pos:	the type * to use as a loop counter.
 * @head:	the head for your list.
 * @member:	the name of the list_struct within the struct.
 */
#define list_for_each_entry(pos, head, member)				\
	for (pos = list_entry((head)->next, typeof(*pos), member);	\
	     &pos->member != (head); 					\
	     pos = list_entry(pos->member.next, typeof(*pos), member))

/**
 * list_for_each_entry_safe - iterate over list of given type safe against removal of list entry
 * @pos:	the type * to use as a loop counter.
 * @n:		another type * to use as temporary storage
 * @head:	the head for your list.
 * @member:	the name of the list_struct within the struct.
 */
#define list_for_each_entry_safe(pos, n, head, member)			\
	for (pos = list_entry((head)->next, typeof(*pos), member),	\
		n = list_entry(pos->member.next, typeof(*pos), member);	\
	     &pos->member != (head); 					\
	     pos = n, n = list_entry(n->member.next, typeof(*n), member))


#endif


#endif //0
