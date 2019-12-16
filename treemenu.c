#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "treemenu.h"


struct treemenu {
    char *module_name;
    char *str;
    function_t fct;
    void *ctx;
    unsigned int level;
    struct list_head head;
    struct list_head node;
};


tm_t *tm_init(const char *module_name)
{
    tm_t *root = calloc(1, sizeof(tm_t));
    if (!root)
        return NULL;

    root->module_name = strdup(module_name);
    root->fct = NULL;
    root->level = 0;
    init_list(&(root->head));
    init_list(&(root->node));

    return root;
}


void tm_deinit(tm_t *root)
{
    if (!root)
        return;

    tm_t *ite, *save;
    list_for_each_entry_safe(ite, save, &(root->head), node) {
        list_del(&(ite->node));
        free(ite->str);
        tm_deinit(ite);
    }

    free(root->module_name);
    free(root);
}


int tm_add_leaf(tm_t *node, const char *function_name, function_t fct, void *ctx)
{
    tm_t *leaf = calloc(1, sizeof(tm_t));
    if (!leaf)
        return -1;

    leaf->str = strdup(function_name);
    leaf->fct = fct;
    leaf->ctx = ctx;
    leaf->level = node->level + 1;
    init_list(&(leaf->head));
    init_list(&(leaf->node));

    list_add_tail(&(leaf->node), &(node->head));

    return 0;
}


tm_t *tm_add_branch(tm_t *node, const char *branch_name)
{
    tm_t *branch = calloc(1, sizeof(tm_t));
    if (!branch)
        return NULL;

    branch->str = strdup(branch_name);
    branch->level = node->level + 1;
    init_list(&(branch->head));
    init_list(&(branch->node));

    list_add_tail(&(branch->node), &(node->head));

    return branch;
}


int tm_exec(tm_t *root, const char *function_name)
{
    tm_t *ite;
    list_for_each_entry(ite, &(root->head), node) {
        if (strncmp(ite->str, function_name, 128) == 0) {
            if (ite->fct)
                ite->fct(ite->ctx);
        }

        tm_exec(ite, function_name);
    }

    return -1;
}


static void _tm_dump_tree(tm_t *root)
{
    tm_t *ite;
    list_for_each_entry(ite, &(root->head), node) {

        printf("|");

        for (int i = 0; i < ite->level - 1; i++)
            printf("-");

        if (ite->fct)
            printf("-[%s], %d\n", ite->str, ite->level);
        else
            printf("*[%s], %d\n", ite->str, ite->level);

        _tm_dump_tree(ite);
    }
}


void tm_dump_tree(tm_t *root)
{
    if (!root)
        return;

    printf("|o[%s]\n", root->module_name);
    _tm_dump_tree(root);
}

