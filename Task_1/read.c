#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int shmid;
    key_t key = 123456;
    char(*array)[5][100];

    printf("\nReading Shared Memory Content\n\n");

    shmid = shmget(key, 5 * 100, IPC_CREAT | 0666);

    if (shmid == -1)
    {
        perror("shmget");
        exit(1);
    }

    array = shmat(shmid, 0, 0);

    printf("\nPrinting\n\n");

    for (int i = 0; i < 5; i++)
    {
        printf("%s", (*array)[i]);
    }

    printf("\nDone\n\n");

    return 0;
}
