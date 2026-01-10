#include <assert.h>
#include <stdalign.h>
#include <stdint.h>
#include <stdio.h>

#include "lib.h"

size_t list_length(const list_t* list) {
  // base case: empty list
  if (list_empty(list)) return 0;

  size_t counter = 0;
  list_node_t* node = list->head;
  while (node != NULL) {
    node = node->next;
    ++counter;
  }
  return counter;
}

bool list_empty(const list_t* list) {
  // list empty if head == tail == NULL
  return list->head == NULL && list->tail == NULL;
}

void list_insert_after(list_t* list, list_node_t* after, list_node_t* node) {
  list_node_t* tmp = list->head;
  // no check required, since node "after" exists
  while (tmp != after) tmp = tmp->next;

  node->prev = tmp;
  node->next = tmp->next;
  if (tmp->next) tmp->next->prev = node;
  tmp->next = node;
  if (tmp == list->tail) list->tail = node;

  list_sanity_check(list);
}

void list_insert_before(list_t* list, list_node_t* before, list_node_t* node) {
  list_node_t* tmp = list->tail;
  // no check required, since node "before" exists;
  while (tmp != before) tmp = tmp->prev;

  node->next = tmp;
  node->prev = tmp->prev;
  if (tmp->prev) tmp->prev->next = node;
  tmp->prev = node;
  if (tmp == list->head) list->head = node;

  list_sanity_check(list);
}

void list_push_front(list_t* list, list_node_t* node) {
  list_node_t* tmp = list->head;
  list->head = node;

  node->next = tmp;
  // case: listy empty
  if (tmp)
    tmp->prev = node;
  else
    list->tail = node;

  list_sanity_check(list);
}

void list_push_back(list_t* list, list_node_t* node) {
  list_node_t* tmp = list->tail;
  list->tail = node;

  node->prev = tmp;
  // case: list empty
  if (tmp)
    tmp->next = node;
  else
    list->head = node;

  list_sanity_check(list);
}

list_node_t* list_front(list_t* list) {
  return list->head;
}
list_node_t* list_back(list_t* list) {
  return list->tail;
}

void list_pop_front(list_t* list) {
  // case: head == tail
  if (list->head == list->tail) {
    free(list->head);
    list->head = NULL;
    list->tail = NULL;
    return;
  }

  list->head = list->head->next;
  free(list->head->prev);
  list->head->prev = NULL;

  list_sanity_check(list);
}

void list_pop_back(list_t* list) {
  // case: head == tail
  if (list->head == list->tail) {
    free(list->head);
    list->head = NULL;
    list->tail = NULL;
    return;
  }

  list->tail = list->tail->prev;
  free(list->tail->next);
  list->tail->next = NULL;

  list_sanity_check(list);
}

void list_remove(list_t* list, list_node_t* node) {
  // special cases
  if (node == list->head) {
    list_pop_front(list);
    return;
  } else if (node == list->tail) {
    list_pop_back(list);
    return;
  }

  // no check required, since node exists and is surrounded by nodes
  node->prev->next = node->next;
  node->next->prev = node->prev;
  free(node);

  list_sanity_check(list);
}

void list_merge(list_t* dest, list_t* src) {
  // special case: list empty
  if (list_empty(src)) return;
  if (list_empty(dest)) {
    dest->head = src->head;
    dest->tail = src->tail;
    src->head = src->tail = NULL;  // clear source list
    return;
  }

  // combine: tail(dest) -> head(src)
  dest->tail->next = src->head;
  src->head->prev = dest->tail;
  dest->tail = src->tail;
  src->head = src->tail = NULL;

  list_sort(dest);

  list_sanity_check(dest);
}

void list_sort(list_t* list) {
  // special case: list empty or a single element
  if (list_empty(list) || list->head == list->tail) return;

  // selection sort, but with a double linked list (O(n^2))
  // NOTE: I've chose selection sort, because it minimizes the number of swaps
  // per pass

  // for(int i = 0; i < len; ++i) {
  //   int min = i;
  //   for(int j = i + 1; j < len; ++j)
  //     if(list[j] < list[min])
  //       min = j;
  //   if(min != i)
  //     swap(&list[i], &list[min]);
  // }

  list_node_t* curr = list->head;
  while (curr) {
    list_node_t* min = curr;
    list_node_t* tmp = curr->next;
    while (tmp) {
      if (tmp->value < min->value) {
        min = tmp;
      }
      tmp = tmp->next;
    }
    if (min != curr) {
      int t = curr->value;
      curr->value = min->value;
      min->value = t;
    }
    curr = curr->next;
  }
}

void list_reverse(list_t* list) {
  // special case: list empty or a single element
  if (list_empty(list) || list->head == list->tail) return;

  list_node_t* curr = list->head;
  list_node_t* tmp;
  while (curr) {
    tmp = curr->next;
    // switch pointers
    curr->next = curr->prev;
    curr->prev = tmp;
    curr = tmp;
  }
  // swap head and tail
  tmp = list->head;
  list->head = list->tail;
  list->tail = tmp;

  list_sanity_check(list);
}

void list_unique(list_t* list) {
  // special case: list empty or a single element
  if (list_empty(list) || list->head == list->tail) return;

  list_node_t* curr = list->head;
  while (curr && curr->next) {
    if (curr->value == curr->next->value)
      list_remove(list, curr->next);
    else
      curr = curr->next;
  }
}
