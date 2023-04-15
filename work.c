#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORKERS 10
#define MAX_DAYS 7

typedef struct
{
    char name[50];
    char availableDays[50];
} Applicant;

void addApplicant();
void modifyApplicant();
void deleteApplicant();
void listApplicants();
void listApplicantsByDay();
int isDayFull();
void loadApplicants();

int main()
{
    Applicant applicants[MAX_WORKERS * MAX_DAYS] = {0};
    int count = 0;
    loadApplicants(applicants, &count);                         // Add this line to load applicants from the file
    int workersNeeded[MAX_DAYS] = {10, 10, 10, 10, 10, 10, 10}; // Update these values according to your needs.
    char availableDays[MAX_DAYS][20] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

    // Main program loop, handle user input and utilize functions.
    printf("\033[0;33m");
    printf("======================================================================\n");
    printf("========= Welcome to 'Drink My Friend' Vineyard Application! =========\n");
    printf("======================================================================\n");
    printf("\033[0;37m");

    // Menu program here:
    int choice;
    do
    {
        printf("1. Add a new applicant\n");
        printf("2. Modify an applicant\n");
        printf("3. Delete an applicant\n");
        printf("4. List all applicants\n");
        printf("5. List applicants by day\n");
        printf("6. Exit\n");
        printf("======================================================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        printf("======================================================================\n");

        switch (choice)
        {
        case 1:
            printf("Enter the name of the applicant: ");
            getchar();
            char name[50];
            printf("\033[0;33m");

            fgets(name, sizeof(name), stdin);
            strtok(name, "\n");

            printf("\033[0;37m");
            printf("Enter the available days of the applicant: ");
            char availableDays[100];
            printf("\033[0;33m");

            fgets(availableDays, sizeof(availableDays), stdin);
            strtok(availableDays, "\n");

            printf("\033[0;37m");
            printf("======================================================================\n");

            addApplicant(applicants, &count, name, availableDays, workersNeeded);
            break;
        case 2:
            printf("Enter the name of the applicant: ");
            getchar();
            printf("\033[0;33m");

            fgets(name, sizeof(name), stdin);
            strtok(name, "\n");

            printf("\033[0;37m");
            printf("Enter the new available days of the applicant: ");
            printf("\033[0;33m");

            fgets(availableDays, sizeof(availableDays), stdin);
            strtok(availableDays, "\n");

            printf("\033[0;37m");
            printf("======================================================================\n");

            modifyApplicant(applicants, &count, name, availableDays, workersNeeded);
            break;
        case 3:
            printf("Enter the name of the applicant: ");
            getchar();
            printf("\033[0;33m");

            fgets(name, sizeof(name), stdin);
            strtok(name, "\n");

            printf("\033[0;37m");
            printf("======================================================================\n");

            deleteApplicant(applicants, &count, name);
            break;
        case 4:
            printf("\033[0;33m");
            listApplicants(applicants, count);
            printf("\033[0;37m");
            printf("======================================================================\n");
            break;
        case 5:
            printf("Enter the day: ");
            printf("\033[0;33m");
            char day[100];

            scanf("%s", day);

            printf("\033[0;37m");
            printf("======================================================================\n");
            printf("\033[0;33m");
            listApplicantsByDay(applicants, count, day);
            printf("\033[0;37m");
            printf("======================================================================\n");
            break;
        case 6:
            printf("\033[0;33m");

            printf(R"EOF(  ______     ________    ______     ________   _
 |  _ \ \   / /  ____|  |  _ \ \   / /  ____| | |
 | |_) \ \_/ /| |__     | |_) \ \_/ /| |__    | |
 |  _ < \   / |  __|    |  _ < \   / |  __|   | |
 | |_) | | |  | |____   | |_) | | |  | |____  |_|
 |____/  |_|  |______|  |____/  |_|  |______| (_)

                                                 )EOF");

            printf("\n");
            printf("\033[0;37m");
            printf("======================================================================\n");
            break;
        default:
            printf("\033[0;33m");
            printf("Invalid choice!\n");
            printf("\033[0;37m");
            printf("======================================================================\n");
            break;
        }
    } while (choice != 6);

    return 0;
}

// Add a new applicant to the list.
void addApplicant(Applicant *applicants, int *count, const char *name, const char *availableDaysInput, const int *workersNeeded)
{
    char availableDays[100];
    strncpy(availableDays, availableDaysInput, sizeof(availableDays));

    char *day = strtok(availableDays, " ");
    while (day != NULL)
    {
        if (isDayFull(applicants, *count, day, workersNeeded))
        {
            printf("\033[0;33m");
            printf("Day %s is already full.\n", day);
            printf("\033[0;37m");
            printf("======================================================================\n");
            day = strtok(NULL, " ");
            continue;
        }

        strcpy(applicants[*count].name, name);
        strcpy(applicants[*count].availableDays, day);
        (*count)++;

        FILE *file = fopen("applicants.txt", "a");
        if (file != NULL)
        {
            fprintf(file, "%s - %s\n", name, day);
            fclose(file);
        }
        day = strtok(NULL, " ");
    }
}

// Modify an applicant
void modifyApplicant(Applicant *applicants, int *count, const char *name, const char *availableDaysInput, const int *workersNeeded)
{
    int found = 0;
    for (int i = 0; i < *count; i++)
    {
        if (strcmp(applicants[i].name, name) == 0)
        {
            found = 1;
            break;
        }
    }

    if (!found)
    {
        printf("\033[0;33m");
        printf("Applicant does not exist.\n");
        printf("\033[0;37m");
        printf("======================================================================\n");
        return;
    }

    // Delete the old available days for the applicant
    deleteApplicant(applicants, count, name);

    // Add the new available days for the applicant
    addApplicant(applicants, count, name, availableDaysInput, workersNeeded);

    // Update file contents.
    FILE *file = fopen("applicants.txt", "w");
    if (file != NULL)
    {
        for (int i = 0; i < *count; i++)
        {
            fprintf(file, "%s - %s\n", applicants[i].name, applicants[i].availableDays);
        }
        fclose(file);
    }

    printf("\033[0;33m");
    printf("Applicant '%s' modified days of work successfully!\n", name);
    printf("\033[0;37m");
    printf("======================================================================\n");
}

// Delete an applicant from the list.
void deleteApplicant(Applicant *applicants, int *count, const char *name)
{
    int removed = 0;
    for (int i = 0; i < *count;)
    {
        if (strcmp(applicants[i].name, name) == 0)
        {
            for (int j = i; j < *count - 1; j++)
            {
                applicants[j] = applicants[j + 1];
            }
            (*count)--;
            removed++;
        }
        else
        {
            i++;
        }
    }

    // Update file contents.
    FILE *file = fopen("applicants.txt", "w");
    if (file != NULL)
    {
        for (int i = 0; i < *count; i++)
        {
            fprintf(file, "%s - %s\n", applicants[i].name, applicants[i].availableDays);
        }
        fclose(file);
    }

    if (removed > 0)
    {
        printf("\033[0;33m");
        printf("Applicant '%s' and all registered days removed successfully!\n", name);
        printf("\033[0;37m");
        printf("===================================================================\n");
    }
    else
    {
        printf("\033[0;33m");
        printf("Applicant not found.\n");
        printf("\033[0;37m");
        printf("===================================================================\n");
    }
}

// List all applicants
void listApplicants(Applicant *applicants, int count)
{
    for (int i = 0; i < count; i++)
    {
        printf("%s - %s\n", applicants[i].name, applicants[i].availableDays);
    }
}

// List applicants by day
void listApplicantsByDay(Applicant *applicants, int count, const char *day)
{
    for (int i = 0; i < count; i++)
    {
        if (strstr(applicants[i].availableDays, day) != NULL)
        {
            printf("%s - %s\n", applicants[i].name, applicants[i].availableDays);
        }
    }
}

// Check if a day is full
int isDayFull(Applicant *applicants, int count, const char *day, const int *workersNeeded)
{
    int workers = 0;
    int dayIndex = -1;

    const char *days[MAX_DAYS] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
    for (int i = 0; i < MAX_DAYS; i++)
    {
        if (strcmp(days[i], day) == 0)
        {
            dayIndex = i;
            break;
        }
    }

    if (dayIndex == -1)
    {
        printf("Invalid day.\n");
        printf("======================================================================\n");
        return 1;
    }

    for (int i = 0; i < count; i++)
    {
        if (strstr(applicants[i].availableDays, day) != NULL)
        {
            workers++;
        }
    }

    return workers >= workersNeeded[dayIndex];
}

// Load applicants from the file
void loadApplicants(Applicant *applicants, int *count)
{
    FILE *file = fopen("applicants.txt", "r");
    if (file != NULL)
    {
        char line[150];
        while (fgets(line, sizeof(line), file) != NULL)
        {
            char name[50], availableDays[50];
            if (sscanf(line, "%[^-] - %s", name, availableDays) == 2)
            {
                strncpy(applicants[*count].name, name, sizeof(applicants[*count].name) - 1);
                applicants[*count].name[sizeof(applicants[*count].name) - 1] = '\0';
                strncpy(applicants[*count].availableDays, availableDays, sizeof(applicants[*count].availableDays) - 1);
                applicants[*count].availableDays[sizeof(applicants[*count].availableDays) - 1] = '\0';
                (*count)++;
            }
        }
        fclose(file);
    }
}