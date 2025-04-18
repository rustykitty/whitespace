#include <stdio.h>

#include "exec_tree.c"

static struct WS_statement PROGRAM[] = {
/* INSERT STATEMENTS HERE */
};

int main() {
    struct wstree_err* err = wsexecute(PROGRAM, sizeof(PROGRAM) / sizeof(struct WS_statement));
    if (err) {
        fprintf(stderr, "ERROR at position %zu: %s", err->index, err->message);
        return 1;
    }
    return 0;
}
