/*!
 * \file utils.c
 * \brief Utility functions implementation — pure ANSI C99.
 */

#include "utils.h"
#include <stdio.h>

void print_message(const char *msg) {
    if (msg != NULL) {
        (void)printf("%s\n", msg);
    }
}
