#ifndef LINKED_LIST
# define LINKED_LIST


#ifdef __cplusplus
	extern "C";
#endif

# include <stdlib.h>

typedef struct s_node {
	s_node *prev;
	s_node *next;
	void *data;
}t_node;

static inline t_node **initList(void) {
	t_node **node;

	node = (t_node **)malloc(sizeof(t_node*));
	if (!node) {
		abort();
	}
	return (node);
}

static inline t_node *createNode(void *data) {
	t_node *new_node;

	new_node = (t_node *)malloc(sizeof(t_node));
	if (!new_node) {
		abort();
	}
	new_node->data = data;
	new_node->prev = 0x00;
	new_node->next = 0x00;
	return (new_node);
}

static inline t_node *listLast(t_node **list) {
	t_node *span = (*list);

	if (!*list) {
		return (0x00);
	}
	while (span->next) {
		span = span->next;
	}
	return (span);
}

static inline void addNodeBack(t_node **list, t_node *new_elem) {\
	t_node *span = listLast(list);
	if (span) {
		span->next = new_elem;
		new_elem->prev = span;
		return;
	}
	(*list) = new_elem;
}

static inline void addNodeFront(t_node **list, t_node *new_elem) {\
	t_node *span = (*list);

	if (!span) {
		(*list) = new_elem;
		return;
	}
	span->prev = new_elem;
	new_elem->next = span;
}

#endif
