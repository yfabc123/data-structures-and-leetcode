#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include "myThreadPool.h"

int main()
{
    TPool *tPool = createPool(5, 5);
    addJob(tPool);
    return 0;
}