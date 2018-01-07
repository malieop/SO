#include "unix.h"
#include <stdio.h>
#include <errno.h>

/* Mensagem de erro */
void err_dump(char *msg)
{
        perror(msg);
        exit(1);
}
