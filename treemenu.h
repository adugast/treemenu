#ifndef __TREEMENU_H__
#define __TREEMENU_H__


#include "list.h"


typedef struct treemenu tm_t;

typedef void (*function_t)(void *ctx);


tm_t *tm_init(const char *module_name);

void tm_deinit(tm_t *root);

int tm_add_leaf(tm_t *node, const char *function_name, function_t fct, void *ctx);

tm_t *tm_add_branch(tm_t *node, const char *branch_name);

int tm_exec(tm_t *root, const char *function_name);

void tm_dump_tree(tm_t *root);


#endif /* __TREEMENU_H__ */

