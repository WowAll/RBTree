#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed

  p->nil = (node_t *)calloc(1, sizeof(node_t));
  p->nil->color = RBTREE_BLACK;
  p->nil->left = NULL;
  p->nil->right = NULL;
  p->nil->parent = NULL;
  p->root = p->nil;

  return p;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  delete_node(t->root);
  free(t->nil);
  free(t);
}

void delete_node(node_t *p) {
  if (p == NULL) {
    return;
  }
  delete_node(p->left);
  delete_node(p->right);
  free(p);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
