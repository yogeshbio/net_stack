
#pragma once

#include "stdio.h"
#include "stddef.h"
#include "stdlib.h"
#include <assert.h>
/******************************************************************************
 * circular doubly linked list macros                                         *
 *****************************************************************************/

struct list_head {
    struct list_head *next, *prev;
};


#define CDL_APPEND(head,add)                                                                   \
    CDL_APPEND2(head,add,prev,next)

#define CDL_APPEND2(head,add,prev,next)                                                        \
do {                                                                                           \
 if (head) {                                                                                   \
   (add)->next = head;                                                                          \
   (add)->prev = head->prev;                                                                    \
   (head)->prev->next = add;                                                                  \
   (head)->prev = add;                                                                          \
 } else {                                                                                      \
   (add)->prev = (add);                                                                        \
   (add)->next = (add);                                                                        \
   (head) = (add);                                                                             \
 }                                                                                             \
} while (0)


#define CDL_PREPEND(head,add)                                                                  \
    CDL_PREPEND2(head,add,prev,next)

#define CDL_PREPEND2(head,add,prev,next)                                                       \
do {                                                                                           \
 if (head) {                                                                                   \
   (add)->prev = (head)->prev;                                                                 \
   (add)->next = (head);                                                                       \
   (head)->prev = (add);                                                                       \
   (add)->prev->next = (add);                                                                  \
 } else {                                                                                      \
   (add)->prev = (add);                                                                        \
   (add)->next = (add);                                                                        \
 }                                                                                             \
 (head) = (add);                                                                               \
} while (0)

#define CDL_INSERT_INORDER(head,add,cmp)                                                       \
    CDL_INSERT_INORDER2(head,add,cmp,prev,next)

#define CDL_INSERT_INORDER2(head,add,cmp,prev,next)                                            \
do {                                                                                           \
  LDECLTYPE(head) _tmp;                                                                        \
  if (head) {                                                                                  \
    CDL_LOWER_BOUND2(head, _tmp, add, cmp, next);                                              \
    CDL_APPEND_ELEM2(head, _tmp, add, prev, next);                                             \
  } else {                                                                                     \
    (head) = (add);                                                                            \
    (head)->next = (head);                                                                     \
    (head)->prev = (head);                                                                     \
  }                                                                                            \
} while (0)

#define CDL_LOWER_BOUND(head,elt,like,cmp)                                                     \
    CDL_LOWER_BOUND2(head,elt,like,cmp,next)

#define CDL_LOWER_BOUND2(head,elt,like,cmp,next)                                               \
do {                                                                                           \
  if ((head) == NULL || (cmp(head, like)) >= 0) {                                              \
    (elt) = NULL;                                                                              \
  } else {                                                                                     \
    for ((elt) = (head); (elt)->next != (head); (elt) = (elt)->next) {                         \
      if ((cmp((elt)->next, like)) >= 0) {                                                     \
        break;                                                                                 \
      }                                                                                        \
    }                                                                                          \
  }                                                                                            \
} while (0)

#define CDL_DELETE(head,del)                                                                   \
    CDL_DELETE2(head,del,prev,next)

#define CDL_DELETE2(head,del,prev,next)                                                        \
do {                                                                                           \
  if (((head)==(del)) && ((head)->next == (head))) {                                           \
      (head) = NULL;                                                                           \
  } else {                                                                                     \
     (del)->next->prev = (del)->prev;                                                          \
     (del)->prev->next = (del)->next;                                                          \
     if ((del) == (head)) (head)=(del)->next;                                                  \
  }                                                                                            \
} while (0)

#define CDL_COUNT(pos, head, counter)                                                             \
    CDL_COUNT2(pos , head,counter)

#define CDL_COUNT2(pos, head, counter)                                                     \
do {                                                                                           \
  (counter) = 0;                                                                               \
  CDL_FOREACH(pos, head) { ++(counter); }                                                  \
} while (0)

#define CDL_FOREACH(pos,head)                                                                   \
    for (pos = (head)->next; pos != (head); pos = pos->next)


#define CDL_FOREACH2(head,el,next)                                                             \
    for ((el)=(head);el;(el)=(((el)->next==(head)) ? NULL : (el)->next))

#define CDL_FOREACH_SAFE(head,el,tmp1,tmp2)                                                    \
    CDL_FOREACH_SAFE2(head,el,tmp1,tmp2,prev,next)

#define CDL_FOREACH_SAFE2(head,el,tmp1,tmp2,prev,next)                                         \
  for ((el) = (head), (tmp1) = (head) ? (head)->prev : NULL;                                   \
       (el) && ((tmp2) = (el)->next, 1);                                                       \
       (el) = ((el) == (tmp1) ? NULL : (tmp2)))

#define CDL_SEARCH_SCALAR(head,out,field,val)                                                  \
    CDL_SEARCH_SCALAR2(head,out,field,val,next)

#define CDL_SEARCH_SCALAR2(head,out,field,val,next)                                            \
do {                                                                                           \
    CDL_FOREACH2(head,out,next) {                                                              \
      if ((out)->field == (val)) break;                                                        \
    }                                                                                          \
} while (0)

#define CDL_SEARCH(head,out,elt,cmp)                                                           \
    CDL_SEARCH2(head,out,elt,cmp,next)

#define CDL_SEARCH2(head,out,elt,cmp,next)                                                     \
do {                                                                                           \
    CDL_FOREACH2(head,out,next) {                                                              \
      if ((cmp(out,elt))==0) break;                                                            \
    }                                                                                          \
} while (0)

#define CDL_REPLACE_ELEM2(head, el, add, prev, next)                                           \
do {                                                                                           \
 assert((head) != NULL);                                                                       \
 assert((el) != NULL);                                                                         \
 assert((add) != NULL);                                                                        \
 if ((el)->next == (el)) {                                                                     \
  (add)->next = (add);                                                                         \
  (add)->prev = (add);                                                                         \
  (head) = (add);                                                                              \
 } else {                                                                                      \
  (add)->next = (el)->next;                                                                    \
  (add)->prev = (el)->prev;                                                                    \
  (add)->next->prev = (add);                                                                   \
  (add)->prev->next = (add);                                                                   \
  if ((head) == (el)) {                                                                        \
   (head) = (add);                                                                             \
  }                                                                                            \
 }                                                                                             \
} while (0)

#define CDL_REPLACE_ELEM(head, el, add)                                                        \
    CDL_REPLACE_ELEM2(head, el, add, prev, next)

#define CDL_PREPEND_ELEM2(head, el, add, prev, next)                                           \
do {                                                                                           \
  if (el) {                                                                                    \
    assert((head) != NULL);                                                                    \
    assert((add) != NULL);                                                                     \
    (add)->next = (el);                                                                        \
    (add)->prev = (el)->prev;                                                                  \
    (el)->prev = (add);                                                                        \
    (add)->prev->next = (add);                                                                 \
    if ((head) == (el)) {                                                                      \
      (head) = (add);                                                                          \
    }                                                                                          \
  } else {                                                                                     \
    CDL_APPEND2(head, add, prev, next);                                                        \
  }                                                                                            \
} while (0)

#define CDL_PREPEND_ELEM(head, el, add)                                                        \
    CDL_PREPEND_ELEM2(head, el, add, prev, next)

#define CDL_APPEND_ELEM2(head, el, add, prev, next)                                            \
do {                                                                                           \
 if (el) {                                                                                     \
  assert((head) != NULL);                                                                      \
  assert((add) != NULL);                                                                       \
  (add)->next = (el)->next;                                                                    \
  (add)->prev = (el);                                                                          \
  (el)->next = (add);                                                                          \
  (add)->next->prev = (add);                                                                   \
 } else {                                                                                      \
  CDL_PREPEND2(head, add, prev, next);                                                         \
 }                                                                                             \
} while (0)

#define CDL_APPEND_ELEM(head, el, add)                                                         \
    CDL_APPEND_ELEM2(head, el, add, prev, next)

#ifdef NO_DECLTYPE
/* Here are VS2008 / NO_DECLTYPE replacements for a few functions */

#undef CDL_INSERT_INORDER2
#define CDL_INSERT_INORDER2(head,add,cmp,prev,next)                                            \
do {                                                                                           \
  if ((head) == NULL) {                                                                        \
    (add)->prev = (add);                                                                       \
    (add)->next = (add);                                                                       \
    (head) = (add);                                                                            \
  } else if ((cmp(head, add)) >= 0) {                                                          \
    (add)->prev = (head)->prev;                                                                \
    (add)->next = (head);                                                                      \
    (add)->prev->next = (add);                                                                 \
    (head)->prev = (add);                                                                      \
    (head) = (add);                                                                            \
  } else {                                                                                     \
    char *_tmp = (char*)(head);                                                                \
    while ((char*)(head)->next != _tmp && (cmp((head)->next, add)) < 0) {                      \
      (head) = (head)->next;                                                                   \
    }                                                                                          \
    (add)->prev = (head);                                                                      \
    (add)->next = (head)->next;                                                                \
    (add)->next->prev = (add);                                                                 \
    (head)->next = (add);                                                                      \
    UTLIST_RS(head);                                                                           \
  }                                                                                            \
} while (0)

#endif

// Taken from linux: but modified as per needs

/* Note on offset of: How it works:
#define offset_of(s,m) ((size_t)(&(((s*)(0))->m))) where s is the structure and m is the element
 *
 * (
  (size_t(       // 4.
    &( (         // 3.
      (s*)(0)    // 1.
     )->m )      // 2.
  )
)
Working from the inside out, this is ...

1. Casting the value zero to the struct pointer type s*
2. Getting the struct field m of this (illegally placed) struct object
3. Getting the address of this m field
4. Casting the address to an size_t
Conceptually this is placing a struct object at memory address zero and then finding out at what the address of a particular
field is. This could allow you to figure out the offsets in memory of each field in a struct so you could write your own
serializers and deserializers to convert structs to and from byte arrays.
Of course if you would actually dereference a zero pointer your program would crash, but actually everything happens in the compiler and no actual zero pointer is dereferenced at runtime.

*/

#define offset_of(s,m) ((size_t)(&(((s*)(0))->m)))

// This is taken from Linux, mptr points to the address passed :ptr (e.g say 0x1008 and offset_of could be 0x08), hence base addr of the struct would
// be 0x1008 - 0x08 = 0x1000
#define container_of(ptr, type, member) ({			\
	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (char *)__mptr - offset_of(type,member) );})

/**
 * list_entry - get the struct for this entry
 * @ptr:	the &struct list_head pointer.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the list_struct within the struct.
 */
#define list_entry(ptr, type, member) \
	container_of(ptr, type, member)


#define list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)

