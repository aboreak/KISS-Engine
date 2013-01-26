#ifndef __LIST_H__
#define __LIST_H__

struct list;

struct list {
	unsigned char *	data;
	struct list *	prev;
	struct list *	next;
	unsigned	esize;
	unsigned	occupied;
};

#define list_new(x) (struct list_new_real(sizeof(x)))

struct list * list_new_real(unsigned esize);

void list_delete(struct list **lst);

void list_reserve(struct list *lst, unsigned n);

unsigned list_reserved_num(struct list *lst);

struct list * list_insert_back(struct list *lst, void *data);

struct list * list_remove_front(struct list **lst);

inline void list_occupy(struct list *lst, void *data);

inline void list_unoccupy(struct list *lst);

struct list * list_unlink_front(struct list **lst);

struct list * list_link_front(struct list **lst, struct list *other);

unsigned char * list_get(struct list *lst, unsigned i);

#endif
