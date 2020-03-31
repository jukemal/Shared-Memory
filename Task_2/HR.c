#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/file.h>

#define LOCK_SH 1 /* shared lock */
#define LOCK_EX 2 /* exclusive lock */
#define LOCK_NB 4 /* don't block when locking */
#define LOCK_UN 8 /* unlock */

char fileName[] = "db.txt";

char ele[20];

void viewEmployee(char[], char[]);
void addEmployee(char[], char[], char[]);
void deleteEmployee(char[]);
void editElement(char[], char[], char[]);
void deleteElement(char[], char[]);
int countEntries();

int main()
{

    char c[100];
    char id[20];
    char attendance[20];
    char salary[20];
    char value[20];

    printf("HR\n\n");

    printf("Choose one from the list or type 'q' to Quite.\n");
    printf("\t1. Add new employee.\n");
    printf("\t2. Delete employee.\n");
    printf("\t3. Edit employee.\n");
    printf("\t4. View total salary of employee.\n");
    printf("\t: ");
    scanf("%s", c);

    while (strcmp(c, "q") != 0)
    {
        if (strcmp(c, "1") == 0)
        {
            printf("\n\nAdding new employee : \n\n");
            printf("Enter employee ID : ");
            scanf("%s", &id);
            printf("Enter value for attendance : ");
            scanf("%s", &attendance);
            printf("Enter value for salary : ");
            scanf("%s", &salary);

            addEmployee(id, attendance, salary);

            printf("\n\nSuccessfully added employee %s\n\n", id);

            strcpy(c, "");
        }
        else if (strcmp(c, "2") == 0)
        {
            printf("\n\nDeleting employee : \n\n");
            printf("Enter employee ID : ");
            scanf("%s", &id);

            deleteEmployee(id);

            printf("\n\nSuccessfully deleted employee %s\n\n", id);

            strcpy(c, "");
        }
        else if (strcmp(c, "3") == 0)
        {
            printf("\n\nChoose one from the list or type 'q' to Quite.\n\n\n");
            printf("\t1.  Edit attendance of employee.\n");
            printf("\t2.  Delete attendance of employee.\n");
            printf("\t3.  Edit employees’ salary.\n");
            printf("\t4.  Delete employees’ salary\n");
            printf("\t: ");
            scanf("%s", &c);

            if (strcmp(c, "1") == 0)
            {
                printf("\n\nEdit attendance of employee : \n\n");
                printf("Enter employee ID : ");
                scanf("%s", &id);
                printf("Enter new value for attendance : ");
                scanf("%s", &value);

                editElement(id, "Attendance", value);

                printf("\n\nSuccessfully edited value for attendance for employee %s\n\n", id);

                strcpy(c, "");
            }
            else if (strcmp(c, "2") == 0)
            {
                printf("\n\nDelete attendance of employee : \n\n");
                printf("Enter employee ID : ");
                scanf("%s", &id);

                deleteElement(id, "Attendance");

                printf("\n\nSuccessfully deleted value for attendance for employee %s\n\n", id);

                strcpy(c, "");
            }
            else if (strcmp(c, "3") == 0)
            {
                printf("\n\nEdit employees’ salary : \n\n");
                printf("Enter employee ID : ");
                scanf("%s", &id);
                printf("Enter new value for salary : ");
                scanf("%s", &value);

                editElement(id, "Salary", value);

                printf("\n\nSuccessfully edited salary for employee %s\n\n", id);

                strcpy(c, "");
            }
            else if (strcmp(c, "4") == 0)
            {
                printf("\n\nDelete salary of employee : \n\n");
                printf("Enter employee ID : ");
                scanf("%s", &id);

                deleteElement(id, "Salary");

                printf("\n\nSuccessfully deleted salary for employee %s\n\n", id);

                strcpy(c, "");
            }
            else
            {
                printf("\n\nInvalid input. Try again\n\n");

                strcpy(c, "");
            }
        }
        else if (strcmp(c, "4") == 0)
        {
            printf("\n\nView total salary of employee : \n\n");
            printf("Enter employee ID : ");
            scanf("%s", &id);

            viewEmployee(id, "Total");

            printf("\n\nTotal salary for employee %s : %s\n\n", id, ele);

            strcpy(c, "");
        }

        printf("\n\nHR\n\n\n");

        printf("Choose one from the list or type 'q' to Quite.\n");
        printf("\t1. Add new employee.\n");
        printf("\t2. Delete employee.\n");
        printf("\t3. Edit employee.\n");
        printf("\t4. View total salary of employee.\n");
        printf("\t: ");
        scanf("%s", &c);
    }

    return 0;
}

void viewEmployee(char id[], char element[])
{
    FILE *fp;
    fp = fopen(fileName, "r");

    if (fp == NULL)
    {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }

    char line[100];

    while (fgets(line, sizeof(line), fp))
    {
        size_t ln = strlen(line) - 1;
        if (line[ln] == '\n')
            line[ln] = '\0';

        char *token = strtok(line, " ");

        int i = 0;
        char arr[4][50];

        while (token != NULL)
        {
            strcpy(arr[i], token);

            token = strtok(NULL, " ");

            i++;
        }

        if (strcmp(arr[0], id) == 0)
        {
            if (strcmp(element, "Attendance") == 0)
            {
                strcpy(ele, arr[1]);
            }
            else if (strcmp(element, "Salary") == 0)
            {
                strcpy(ele, arr[2]);
            }
            else if (strcmp(element, "Total") == 0)
            {
                strcpy(ele, arr[3]);
            }
        }
    }

    fclose(fp);
}

void addEmployee(char id[], char attendance[], char salary[])
{
    FILE *fp;
    fp = fopen(fileName, "a");

    if (flock(fileno(fp), LOCK_EX) == -1)
    {
        printf("File is locked");
        exit(1);
    }

    if (fp == NULL)
    {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }

    char temp[100];

    strcpy(temp, id);
    strcat(temp, " ");
    strcat(temp, attendance);
    strcat(temp, " ");
    strcat(temp, salary);
    strcat(temp, " ");

    int a = atoi(attendance);
    int b = atoi(salary);

    char buffer[20];

    sprintf(buffer, "%d", a * b);

    strcat(temp, buffer);
    strcat(temp, "\n");

    fputs(temp, fp);

    if (flock(fileno(fp), LOCK_UN) == -1)
    {
        exit(1);
    }

    fclose(fp);
}

void deleteEmployee(char id[])
{
    int count = countEntries();

    char employees[count][4][100];

    FILE *fp;
    fp = fopen(fileName, "r");

    if (fp == NULL)
    {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }

    char line[100];
    int i = 0;

    while (fgets(line, sizeof(line), fp))
    {
        size_t ln = strlen(line) - 1;
        if (line[ln] == '\n')
            line[ln] = '\0';

        char *token = strtok(line, " ");

        int j = 0;

        while (token != NULL)
        {
            strcpy(employees[i][j], token);

            token = strtok(NULL, " ");

            j++;
        }

        i++;
    }

    fclose(fp);

    FILE *fp1;
    fp1 = fopen(fileName, "w");

    if (flock(fileno(fp1), LOCK_EX) == -1)
    {
        printf("File is locked");
        exit(1);
    }

    if (fp1 == NULL)
    {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < count; i++)
    {
        if (strcmp(id, employees[i][0]) != 0)
        {
            char modified[100];

            strcpy(modified, employees[i][0]);
            strcat(modified, " ");
            strcat(modified, employees[i][1]);
            strcat(modified, " ");
            strcat(modified, employees[i][2]);
            strcat(modified, " ");
            strcat(modified, employees[i][3]);
            strcat(modified, "\n");

            fprintf(fp1, "%s", modified);
        }
    }

    if (flock(fileno(fp1), LOCK_UN) == -1)
    {
        exit(1);
    }

    fclose(fp1);
}

void editElement(char id[], char element[], char value[])
{
    int count = countEntries();

    char employees[count][4][100];

    FILE *fp;
    fp = fopen(fileName, "r");

    if (fp == NULL)
    {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }

    char line[100];
    int i = 0;

    while (fgets(line, sizeof(line), fp))
    {
        size_t ln = strlen(line) - 1;
        if (line[ln] == '\n')
            line[ln] = '\0';

        char *token = strtok(line, " ");

        int j = 0;

        while (token != NULL)
        {
            strcpy(employees[i][j], token);

            token = strtok(NULL, " ");

            j++;
        }

        i++;
    }

    fclose(fp);

    FILE *fp1;
    fp1 = fopen(fileName, "w");

    if (flock(fileno(fp1), LOCK_EX) == -1)
    {
        printf("File is locked");
        exit(1);
    }

    if (fp1 == NULL)
    {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < count; i++)
    {
        if (strcmp(id, employees[i][0]) == 0)
        {
            if (strcmp(element, "Attendance") == 0)
            {
                strcpy(employees[i][1], value);
            }
            else if (strcmp(element, "Salary") == 0)
            {
                strcpy(employees[i][2], value);
            }
            else if (strcmp(element, "Total") == 0)
            {
                strcpy(employees[i][3], value);
            }

            char modified[100];

            strcpy(modified, employees[i][0]);
            strcat(modified, " ");
            strcat(modified, employees[i][1]);
            strcat(modified, " ");
            strcat(modified, employees[i][2]);
            strcat(modified, " ");
            strcat(modified, employees[i][3]);
            strcat(modified, "\n");

            fprintf(fp1, "%s", modified);
        }
        else
        {
            char modified[100];

            strcpy(modified, employees[i][0]);
            strcat(modified, " ");
            strcat(modified, employees[i][1]);
            strcat(modified, " ");
            strcat(modified, employees[i][2]);
            strcat(modified, " ");
            strcat(modified, employees[i][3]);
            strcat(modified, "\n");

            fprintf(fp1, "%s", modified);
        }
    }

    if (flock(fileno(fp1), LOCK_UN) == -1)
    {
        exit(1);
    }

    fclose(fp1);
}

void deleteElement(char id[], char element[])
{
    int count = countEntries();

    char employees[count][4][100];

    FILE *fp;
    fp = fopen(fileName, "r");

    if (fp == NULL)
    {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }

    char line[100];
    int i = 0;

    while (fgets(line, sizeof(line), fp))
    {
        size_t ln = strlen(line) - 1;
        if (line[ln] == '\n')
            line[ln] = '\0';

        char *token = strtok(line, " ");

        int j = 0;

        while (token != NULL)
        {
            strcpy(employees[i][j], token);

            token = strtok(NULL, " ");

            j++;
        }

        i++;
    }

    fclose(fp);

    FILE *fp1;
    fp1 = fopen(fileName, "w");

    if (flock(fileno(fp1), LOCK_EX) == -1)
    {
        printf("File is locked");
        exit(1);
    }

    if (fp1 == NULL)
    {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < count; i++)
    {
        if (strcmp(id, employees[i][0]) == 0)
        {
            if (strcmp(element, "Attendance") == 0)
            {
                strcpy(employees[i][1], "x");
            }
            else if (strcmp(element, "Salary") == 0)
            {
                strcpy(employees[i][2], "x");
            }
            else if (strcmp(element, "Total") == 0)
            {
                strcpy(employees[i][3], "x");
            }

            char modified[100];

            strcpy(modified, employees[i][0]);
            strcat(modified, " ");
            strcat(modified, employees[i][1]);
            strcat(modified, " ");
            strcat(modified, employees[i][2]);
            strcat(modified, " ");
            strcat(modified, employees[i][3]);
            strcat(modified, "\n");

            fprintf(fp1, "%s", modified);
        }
        else
        {
            char modified[100];

            strcpy(modified, employees[i][0]);
            strcat(modified, " ");
            strcat(modified, employees[i][1]);
            strcat(modified, " ");
            strcat(modified, employees[i][2]);
            strcat(modified, " ");
            strcat(modified, employees[i][3]);
            strcat(modified, "\n");

            fprintf(fp1, "%s", modified);
        }
    }

    if (flock(fileno(fp1), LOCK_UN) == -1)
    {
        exit(1);
    }

    fclose(fp1);
}

int countEntries()
{
    FILE *fp;
    fp = fopen(fileName, "r");

    if (fp == NULL)
    {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }

    char line[100];
    int count = 0;

    while (fgets(line, sizeof(line), fp))
    {
        count++;
    }

    fclose(fp);

    return count;
}