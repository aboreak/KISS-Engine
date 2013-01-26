#include "list.h"
#include "common.h"

static struct list * new_link(unsigned esize)
{
	struct list *lst;

	lst = NEW(struct list);
	lst->data = malloc(esize);
	lst->prev = NULL;
	lst->next = NULL;
	lst->esize = esize;
	lst->occupied = 0;

	return lst;
}

struct list * list_new_real(unsigned esize)
{
	struct list *lst = NEW(struct list);

	lst->data = NULL;
	lst->prev = NULL;
	lst->next = NULL;
	lst->esize = esize;
	lst->occupied = 0;

	return lst;
}

void list_delete(struct list **lst)
{
	while (list_remove_front(lst));
	*lst = NULL;
}

void list_reserve(struct list *lst, unsigned n)
{
	unsigned cnt = 0;

	while (lst) {
		if (!lst->occupied) {
			if (!lst->data)
				lst->data = malloc(lst->esize);
			cnt++;
			if (cnt >= n)
				break;
		}
		if  (!lst->next)
			lst->next = new_link(lst->esize);
		lst = lst->next;
	}
}

unsigned list_reserved_num(struct list *lst)
{
	unsigned cnt = 0;

	while (lst) {
		if (!lst->occupied)
			cnt++;
		lst = lst->next;
	}

	return cnt;
}

struct list * list_insert_back(struct list *lst, void *data)
{
	while (lst) {
		if (!lst->occupied) {
			if (!lst->data)
				lst->data = malloc(lst->esize);
			memcpy(lst->data, data, lst->esize);
			lst->occupied = 1;
			break;
		} else if (!lst->next) {
			lst->next = new_link(lst->esize);
		}
		lst = lst->next;
	}

	return lst;
}

struct list * list_remove_front(struct list **lst)
{
	if (*lst) {
		struct list *next = (*lst)->next;
		if ((*lst)->data)
			free((*lst)->data);
		free((*lst));
		*lst = next;
	}

	return *lst;
}

inline void list_occupy(struct list *lst, void *data)
{
	memcpy(lst->data, data, lst->esize);
	lst->occupied = 1;
}

inline void list_unoccupy(struct list *lst)
{
	lst->occupied = 0;
}

struct list * list_unlink_front(struct list **lst)
{
	struct list *tmp = NULL;
	if (*lst) {
		tmp = *lst;
		*lst = tmp->next;
		tmp->next = NULL;
	}

	return tmp;
}

struct list * list_link_front(struct list **lst, struct list *other)
{
	other->next = *lst;
	*lst = other;
	return *lst;
}

unsigned char * list_get(struct list *lst, unsigned i)
{
	unsigned cnt = 0;

	while (lst) {
		if (cnt == i)
			return lst->data;
		if (!lst->next)
			break;
		lst = lst->next;
		cnt++;
	}

	return NULL;
}


