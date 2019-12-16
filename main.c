#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "treemenu.h"


void print_a(void *ctx)
{
    ctx = ctx;
    printf("print_a\n");
}


void print_b(void *ctx)
{
    printf("print_b, ctx[%s]\n", (char *)ctx);
}


int main()
{
    tm_t *root = tm_init("MODULE_TEST");
    if (!root)
        return -1;

    tm_add_leaf(root, "print_a", print_a, NULL);
    tm_add_leaf(root, "print_b", print_b, "ctx_print_b");

    tm_t *branch1 = tm_add_branch(root, "branch1");
    tm_add_leaf(branch1, "print_c", print_a, NULL);
    tm_add_leaf(branch1, "print_d", print_b, "ctx_print_d");

    tm_t *branch3 = tm_add_branch(branch1, "branch3");
    tm_add_leaf(branch3, "print_g", print_a, NULL);
    tm_add_leaf(branch3, "print_h", print_b, "ctx_print_h");

    tm_t *branch4 = tm_add_branch(branch3, "branch4");
    tm_add_leaf(branch4, "branch4", print_a, NULL);
    tm_add_leaf(branch4, "branch4", print_b, "ctx_print_b");
    tm_add_leaf(branch1, "branch2", print_b, "ctx_print_b");

    tm_t *branch2 = tm_add_branch(branch4, "branch2");
    tm_add_leaf(branch2, "print_e", print_a, NULL);
    tm_add_leaf(branch2, "print_f", print_b, "ctx_print_f");

    while (1) {
        char buf[256] = {0};
        size_t sread = read(0, buf, 256);
        buf[sread-1] = '\0';
        printf("[%ld][%s]\n", sread, buf);
        if (strcmp(buf , "ls") == 0)
            tm_dump_tree(root);
        else if (strcmp(buf, "exit") == 0)
            break;
        tm_exec(root, buf);
    }

//  tm_dump_tree(root);

    tm_exec(root, "print_b");
    tm_exec(root, "print_a");
    tm_exec(root, "branch4");

    tm_deinit(root);

    return 0;
}

