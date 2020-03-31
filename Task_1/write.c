#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

char name[5][10];
char city[5][10];
char age[5][10];

int count = 0;

void *thread(void *arg)
{
    FILE *fp;
    fp = fopen("SOS_Assignment.txt", "r");

    if (fp == NULL)
    {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }

    char line[10];

    while (fgets(line, sizeof(line), fp))
    {
        size_t ln = strlen(line) - 1;
        if (line[ln] == '\n')
            line[ln] = '\0';

        if (count <= 4)
        {
            strcpy(name[count], line);
        }
        else if (count >= 5 && count <= 9)
        {
            strcpy(city[count - 5], line);
        }
        else if (count >= 10 && count <= 14)
        {
            strcpy(age[count - 10], line);
        }

        count++;
    }

    fclose(fp);

    int id = *((int *)arg);

    switch (id)
    {
    case 0:
        printf("Thread 1 : Reading Names\n");
        break;
    case 1:
        printf("Thread 2 : Reading Cities\n");
        break;
    case 2:
        printf("Thread 3 : Reading Ages\n");
        break;
    }
}

int main()
{
    pthread_t threads[3];

    int tids[3];

    for (int i = 0; i < 3; i++)
    {
        tids[i] = i;

        if (pthread_create(&threads[i], NULL, thread, &tids[i]) != 0)
        {
            fprintf(stderr, "pthread_create failed!\n");
            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < 3; i++)
    {
        if (pthread_join(threads[i], NULL) != 0)
        {
            fprintf(stderr, "\nError in joining thread\n");
        }
    }

    printf("\nAppending\n\n");

    char str[5][100];

    for (int i = 0; i < 5; i++)
    {
        char temp[100];

        strcpy(temp, name[i]);
        strcat(temp, " from ");
        strcat(temp, city[i]);
        strcat(temp, ". Age : ");
        strcat(temp, age[i]);
        strcat(temp, "\n");

        printf("%s", temp);

        strcpy(str[i], temp);
    }

    int shmid;
    key_t key = 123456;
    char(*array)[5][100];

    printf("\n\nCreating Shared Memory\n\n");

    shmid = shmget(key, 5 * 100, IPC_CREAT | 0666);

    if (shmid == -1)
    {
        perror("shmget");
        exit(1);
    }

    printf("Writing to Shared Memory\n\n");

    array = shmat(shmid, 0, 0);

    for (int i = 0; i < 5; i++)
    {
        strcpy((*array)[i], str[i]);
    }

    printf("Done\n\n");

    return 0;
}