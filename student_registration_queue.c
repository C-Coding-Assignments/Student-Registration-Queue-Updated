#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//////////////////////
// data definitions //
//////////////////////
#define NAME_LEN 101
#define NETID_LEN 41

//structure definition for a student
struct student {
    char name[NAME_LEN+1], netid[NETID_LEN+1], cop2510_grade;
    double gpa;
    int attempts;
    struct student *next;
};

/////////////////////////
// function prototypes //
/////////////////////////

//function prototype for help which prints the menu to the user
void help();
//function prototype for read which reads in information from the user for certain functions and for creating instances of student
void read(char *name, char *netid, char *cop2510_grade, double *gpa, int *attempts);
//function prototype for add_student which adds a student that the user creates to the linked list
struct student * add_student(struct student *registration, char *name, char *netid, char cop2510_grade, double gpa, int attempts);
//function prototype for pop_student which removes the first student within the linked list
struct student * pop_student(struct student *registration);
//function prototype for list_students which prints all the students within the linked list
void list_students(struct student *registration);
//function prototype for list_gpa_min which prints the students within the linked list that have a gpa greater than or equal to the minimum gpa that the user enters
void list_gpa_min(struct student *registration, double gpa);
//function prototype for list_cop2510_min which prints the students within the linked list that have a greater COP 2510 letter grade greater than or equal to the minimum letter grade that the user enters
void list_cop2510_min(struct student *registration, int cop2510_grade);
//function prototype for clear_queue which clears the dynamically allocated memory
struct student * clear_queue(struct student *registration);

///////////////////
// main function //
///////////////////

int main() 
{
    //variable declarations and initialization
    char code;
    char name[NAME_LEN+1], netid[NETID_LEN+1], cop2510_grade;
    double gpa;
    int attempts;
    struct student *registration = NULL;

    //print the menu to the user
    help();
    printf("\n");

    //for loop which iterates until the user terminates the program
    for (;;) {
        // read operation code
        printf("Enter operation code: ");
        scanf(" %c", &code);

        //while loop which iterates until the user enters the new-line character
        while(getchar() != '\n'); /* skips to end of line */

        // run operation
        switch (code) {
            //scenario if the user enters an h    
            case 'h':
                help();
                break;
            //scenario if the user enters an a    
            case 'a':
                read(name, netid, &cop2510_grade, &gpa, &attempts);
                registration = add_student(registration, name, netid, cop2510_grade, gpa, attempts);
                break;
            //scenario if the user enters a p    
            case 'p':
                registration = pop_student(registration);
                break;
            //scenario if the user enters a l
            case 'l':
                list_students(registration);
                break;
            //scenario if the user enters a g    
            case 'g':
                read(NULL, NULL, NULL, &gpa, NULL);
                list_gpa_min(registration, gpa);
                break;
            //scenario if the user enters a c
            case 'c':
                read(NULL, NULL, &cop2510_grade, NULL, NULL);
                list_cop2510_min(registration, cop2510_grade);
                break;
            //scenario if the user enters a q
            case 'q':
                registration = clear_queue(registration);
                return 0;
            //scenario if the user enters anything other than the letters above
            default:
                printf("Illegal operation code!\n");
        }
        printf("\n");
    }

    return 0;
}

//////////////////////////
// function definitions //
//////////////////////////

//function definition for help which prints the menu to the screen
void help() 
{
    printf("List of operation codes:\n");
    printf("\t'h' for help;\n");
    printf("\t'a' for adding a student to the queue;\n");
    printf("\t'p' for removing a student from the queue;\n");
    printf("\t'l' for listing all students in the queue;\n");
    printf("\t'g' for searching students with a minimum GPA;\n");
    printf("\t'c' for searching students with a minimum grade in COP2510;\n");
    printf("\t'q' to quit.\n");
}

//function definition for read which reads in information from the user for certain functions and for creating instances of student
void read(char *name, char *netid, char *cop2510_grade, double *gpa, int *attempts)
{
    //checking if name is not NULL
    if (name != NULL) {
    printf("Enter the name of the student: ");
    scanf("%[^\n]", name);
    }
    //checking if netid is not NULL
    if (netid != NULL) {
    printf("Enter the NetID of the student: ");
    scanf("%s", netid);
    }
    //checking if cop2510_grade is not NULL
    if (cop2510_grade != NULL) {
    printf("Enter the COP2510 letter grade: ");
    scanf(" %c", cop2510_grade);
    }
    //checking if gpa is not NULL
    if (gpa != NULL) {
    printf("Enter the GPA: ");
    scanf("%lf", gpa);
    }
    //checking if attempts is not NULL
    if (attempts != NULL) {
    printf("Enter the number of previous attempts: ");
    scanf("%d", attempts);
    }
}   

//function definition for list_students which prints the students within the linked list
struct student *add_student(struct student *registration, char *name, char *netid, char cop2510_grade, double gpa, int attempts)
{
    //creating a new student and allocating memory for it
    struct student *newStudent = malloc(sizeof(struct student));

    //checking if memory allocation was successful
    if (newStudent == NULL) {
        printf("Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }

    //copying the input information into the new student
    strcpy(newStudent->name, name);
    strcpy(newStudent->netid, netid);
    newStudent->cop2510_grade = cop2510_grade;
    newStudent->gpa = gpa;
    newStudent->attempts = attempts;
    newStudent->next = NULL;

    //adding the new student to the end of the linked list   
    if (registration == NULL)
        return newStudent;

    //variable declarations and initializations
    struct student *currentStudent = registration;  
    int nodeCount = 1;

    //for loop which brings currentStudent to end of linked list and counts the number of nodes
    for (; currentStudent->next!= NULL; currentStudent = currentStudent->next, nodeCount++);
    //adding new student to end of linked list
    currentStudent->next = newStudent;
    //making currentStudent point to head of linked list
    currentStudent = registration;

    //variable declaration
    char netidArray[nodeCount][NETID_LEN];
    //for loop which iterates through the linked list, counting the number of nodes, and copies the netid of each student into netidArray
    for (int i = 0; currentStudent != NULL && i < nodeCount; currentStudent = currentStudent->next, i++)
        strcpy(netidArray[i], currentStudent->netid);

    //variable declarations and initializations
    struct student *ptr = registration, *ptr2;
    int shouldSwap = 0;

    //for loop which iterates through the linked list to sort it in decreasing order according to the number of attempts the student has made
    for (; ptr != NULL; ptr = ptr->next)
        //for loop which iterates through the linked list
        for (ptr2 = ptr->next; ptr2 != NULL;)
        {
            //selection statement which checks if the attempts of student at ptr2 is greater than the attempts of student at ptr
            if (ptr2->attempts > ptr->attempts)
                shouldSwap = 1;

            //selection statement which checks if the attempts of student at ptr2 is equal to the attempts of student at ptr
            else if (ptr2->attempts == ptr->attempts)
                //for loop which iterates through all the students
                for (int i = 0; i < nodeCount; i++) 
                {
                    //selection statement which checks if student at ptr2 comes before student at ptr
                    if (strcmp(netidArray[i], ptr2->netid) == 0) 
                    {
                        shouldSwap = 1;
                        break;
                    }

                    //selection statement which checks if student at ptr comes before student at ptr2
                    if (strcmp(netidArray[i], ptr->netid) == 0)
                        break;
                }  

            //selection statement which checks if the students at ptr and ptr2 need to be swapped
            if (shouldSwap) 
            {
                //variable declarations and initializations
                char tempName[NAME_LEN], tempNetid[NETID_LEN], tempCop2510_grade;
                double tempGPA;
                int tempAttempts;
                shouldSwap = 0;

                //swapping the nodes
                strcpy(tempName, ptr->name);
                strcpy(tempNetid, ptr->netid);
                tempCop2510_grade = ptr->cop2510_grade;
                tempGPA = ptr->gpa;
                tempAttempts = ptr->attempts;

                //swapping the nodes
                strcpy(ptr->name, ptr2->name);
                strcpy(ptr->netid, ptr2->netid);
                ptr->cop2510_grade = ptr2->cop2510_grade;
                ptr->gpa = ptr2->gpa;
                ptr->attempts = ptr2->attempts;
        
                //swapping the nodes
                strcpy(ptr2->name, tempName);
                strcpy(ptr2->netid, tempNetid);
                ptr2->cop2510_grade = tempCop2510_grade;
                ptr2->gpa = tempGPA;
                ptr2->attempts = tempAttempts;

                ptr = registration;
                ptr2 = ptr->next;
            }     

            //selection statement which checks if the student should not be swapped; if so, make ptr2 point to the next element in the linked list
            else
                ptr2 = ptr2->next; 
        }

    return registration;
}

//function definition for pop_student which removes and prints the first student in the linked list
struct student *pop_student(struct student *registration)
{
    //checking if linked list has no elements
    if (registration == NULL)
        return registration;

    //removing the first student from the linked list
    printf("|----------------------|----------------------|---------|-----|----------|\n");
    printf("| Name | NetID | COP2510 | GPA | Attempts |\n");
    printf("|----------------------|----------------------|---------|-----|----------|\n");
    printf("| %-20s | %-20s | %c | %1.1f | %8d |\n", registration->name, registration->netid, registration->cop2510_grade, registration->gpa, registration->attempts);
    printf("|----------------------|----------------------|---------|-----|----------|\n");

    return registration->next;
}

//function definition for list_cop2510_min which prints the students with a given minimum grade in COP2510
void list_students(struct student *registration) 
{
    //checking if the linked list has at least one element    
    if (registration!= NULL)
    {
        //creating a new instance of student
        struct student *ptr = registration;

        printf("|----------------------|----------------------|---------|-----|----------|\n");
        printf("| Name | NetID | COP2510 | GPA | Attempts |\n");
        printf("|----------------------|----------------------|---------|-----|----------|\n");

        //iterating through linked list and printing each student
        for (; ptr != NULL; ptr = ptr->next)
        {
            printf("| %-20s | %-20s | %c | %1.1f | %8d |\n", ptr->name, ptr->netid, ptr->cop2510_grade, ptr->gpa, ptr->attempts);
            printf("|----------------------|----------------------|---------|-----|----------|\n");
        }
    }
}

//function definition for list_gpa_min which prints the students with a given minimum GPA
void list_gpa_min(struct student *registration, double gpa) 
{
    //checking if the linked list has at least one item
    if (registration != NULL)
    {
        //creating a new instance of student
        struct student *ptr = registration;
        int count = 0;

        //counting how many students have earned the minimum GPA
        for (; ptr != NULL; ptr = ptr->next)
            //checking if the current student has earned the minimum GPA
            if (ptr->gpa >= gpa)
                count++;

        //reseting ptr
        ptr = registration;        

        //printing the students with the minimum GPA
        if (count > 0)
        {
            printf("|----------------------|----------------------|---------|-----|----------|\n");
            printf("| Name | NetID | COP2510 | GPA | Attempts |\n");
            printf("|----------------------|----------------------|---------|-----|----------|\n");

            //for loop which iterates over the linked list
            for (; ptr != NULL; ptr = ptr->next)
                //selection statement which checks if the student earned the minimum GPA
                if (ptr->gpa >= gpa)
                {
                    printf("| %-20s | %-20s | %c | %1.1f | %8d |\n", ptr->name, ptr->netid, ptr->cop2510_grade, ptr->gpa, ptr->attempts);
                    printf("|----------------------|----------------------|---------|-----|----------|\n");
                }
        } 
    }
}

//function definition for list_cop2510_max which prints the students with a given maximum grade in COP2510
void list_cop2510_min(struct student *registration, int cop2510_grade) 
{
    //checking if the linked list has at least one item
    if (registration!= NULL)
    {
        //creating a new instance of student
        struct student *ptr = registration;
        int count = 0;

        //counting how many students have earned the minimum grade in COP2510
        for (; ptr != NULL; ptr = ptr->next)
            //checking if the student has earned the minimum grade in COP2510
            if (ptr->cop2510_grade <= cop2510_grade)
                count++;

        //reseting ptr    
        ptr = registration;           

        //selection statement which checks if at least one student has earned the minimum grade in COP2510 
        if (count > 0)
        {
            printf("|----------------------|----------------------|---------|-----|----------|\n");
            printf("| Name | NetID | COP2510 | GPA | Attempts |\n");
            printf("|----------------------|----------------------|---------|-----|----------|\n");

            //for loop which iterates over the linked list
            for (; ptr != NULL; ptr = ptr->next)
                //selection statement which checks if the current student has earned the minimum grade
                if (ptr->cop2510_grade <= cop2510_grade)
                {
                    printf("| %-20s | %-20s | %c | %1.1f | %8d |\n", ptr->name, ptr->netid, ptr->cop2510_grade, ptr->gpa, ptr->attempts);
                    printf("|----------------------|----------------------|---------|-----|----------|\n");
                }
        }
    }
}

//function definition for list_attempts_max which prints the students with the maximum number of attempts
struct student * clear_queue(struct student *registration) 
{
    //freeing memory
    free(registration);

    return registration;
}
