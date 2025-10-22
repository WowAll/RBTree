#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed

  p->nil = (node_t *)calloc(1, sizeof(node_t));
  p->nil->color = RBTREE_BLACK;
  p->nil->left = p->nil;
  p->nil->right = p->nil;
  p->nil->parent = p->nil;
  p->root = p->nil;

  return p;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  delete_all_node(t->root, t->nil);
  free(t->nil);
  free(t);
}

void delete_all_node(node_t *p, node_t *nil) {
  if (p == NULL || p == nil)
    return;
  delete_node(p->left, nil);
  delete_node(p->right, nil);
  free(p);
}

node_t *rbtree_min(const rbtree *t) {
  return subtree_min(t, t->root);
}

node_t *subtree_min(const rbtree *t, node_t *p) {
  node_t *r = p;
  if (r == t->nil)
    return r;
  while (r->left != t->nil)
    r = r->left;
  return r;
}

node_t *rbtree_max(const rbtree *t) {
  node_t *p = t->root;
  while (p->right != t->nil)
    p = p->right;
  return p;
}

void left_rotate(rbtree *t, node_t *p) {
  node_t *r = p->right;
  p->right = r->left;
  if (r->left != t->nil)
    r->left->parent = p;
  r->parent = p->parent;
  if (p->parent == t->nil)
    t->root = r;
  else if (p == p->parent->left)
    p->parent->left = r;
  else
    p->parent->right = r;
  r->left = p;
  p->parent = r;
}

void right_rotate(rbtree *t, node_t *p) {
  node_t *l = p->left;
  p->left = l->right;
  if (l->right != t->nil)
    l->right->parent = p;
  l->parent = p->parent;
  if (p->parent == t->nil)
    t->root = l;
  else if (p == p->parent->right)
    p->parent->right = l;
  else
    p->parent->left = l;
  l->right = p;
  p->parent = l;
}

void insert_fix_up(rbtree *t, node_t *z) {
  while (z->parent->color == RBTREE_RED) {
    node_t *parent = z->parent;
    node_t *grandparent = parent->parent;
    if (parent == grandparent->left) {
      node_t *uncle = grandparent->right;
      if (uncle->color == RBTREE_RED) {
        parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        grandparent->color = RBTREE_RED;
        z = grandparent;
      }
      else {
        if (z == parent->right) {
          z = parent;
          left_rotate(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        grandparent->color = RBTREE_RED;
        right_rotate(t, grandparent);
      }
    }
    else {
      node_t *uncle = grandparent->left;
      if (uncle->color == RBTREE_RED) {
        parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        grandparent->color = RBTREE_RED;
        z = grandparent;
      }
      else {
        if (z == parent->left) {
          z = parent;
          right_rotate(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        grandparent->color = RBTREE_RED;
        left_rotate(t, grandparent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *z = (node_t *)calloc(1, sizeof(node_t));
  z->key = key;
  z->color = RBTREE_RED;
  z->left = t->nil;
  z->right = t->nil;

  node_t *y = t->nil;
  node_t *x = t->root;

  while (x != t->nil) {
    y = x;
    if (key < x->key)
      x = x->left;
    else
      x = x->right;
  }
  z->parent = y;
  if (y == t->nil)
    t->root = z;
  else if (key < y->key)
    y->left = z;
  else
    y->right = z;
  insert_fix_up(t, z);
  return z;
}

void transplant(rbtree *t, node_t *u, node_t *v) {
  if (u->parent == t->nil)
    t->root = v;
  else if (u == u->parent->left)
    u->parent->left = v;
  else
    u->parent->right = v;
  v->parent = u->parent;
}

void delete_fix_up(rbtree *t, node_t *x) {
  node_t *w;
  while ((x != t->root) && (x->color == RBTREE_BLACK)) {
    if (x == x->parent->left) {
      w = x->parent->right;
      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t, x->parent);
        w = x->parent->right;
      }
      if ((w->left->color == RBTREE_BLACK) && (w->right->color == RBTREE_BLACK)) {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else {
        if (w->right->color == RBTREE_BLACK) {
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rotate(t, w);
          w = x->parent->right;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotate(t, x->parent);
        x = t->root;
      }
    }
    else {
      w = x->parent->left;
      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t, x->parent);
        w = x->parent->left;
      }
      if ((w->left->color == RBTREE_BLACK) && (w->right->color == RBTREE_BLACK)) {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else {
        if (w->left->color == RBTREE_BLACK) {
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotate(t, w);
          w = x->parent->left;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotate(t, x->parent);
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  node_t *x;
  node_t *y = p;
  color_t y_original_color = y->color;
  if (p->left == t->nil) {
    x = p->right;
    transplant(t, p, x);
  }
  else if (p->right == t->nil) {
    x = p->left;
    transplant(t, p, x);
  }
  else {
    y = subtree_min(t, p->right);
    y_original_color = y->color;
    x = y->right;
    if (y->parent == p)
        x->parent = y;
    else {
      transplant(t, y, y->right);
      y->right = p->right;
      y->right->parent = y;
    }
    transplant(t, p, y);
    y->left = p->left;
    y->left->parent = y;
    y->color = p->color;
  }

  if (y_original_color == RBTREE_BLACK)
    delete_fix_up(t, x);
  
  free(p);

  return 0;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t *p = t->root;
  while (p != t->nil) {
    if (p->key == key)
      return p;
    else if (p->key < key)
      p = p->right;
    else
      p = p->left;
  }
  return NULL;
}

int inorder_rbtree(node_t *root, key_t *res,const rbtree *t, int i){
  if (root == t -> nil)
      return i;

  i = inorder_rbtree(root->left, res, t, i);
  res[i++] = root->key;
  i = inorder_rbtree(root->right, res, t, i);
  return i;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n){
  if (t->root == t->nil)
      return -1;
  inorder_rbtree(t->root, arr, t, 0);
  return 0;
}