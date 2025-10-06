
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "users.txt"

struct User {
    int id;
    char name[50];
    int age;
};

void createUser() {
    struct User u;
    FILE *fp = fopen(FILE_NAME, "a");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    printf("Enter ID: ");
    scanf("%d", &u.id);
    printf("Enter Name: ");
    scanf("%s", u.name);
    printf("Enter Age: ");
    scanf("%d", &u.age);

    fprintf(fp, "%d %s %d\n", u.id, u.name, u.age);
    fclose(fp);
    printf("User added successfully.\n");
}

void readUsers() {
    struct User u;
    FILE *fp = fopen(FILE_NAME, "r");
    if (fp == NULL) {
        printf("No users found. File may not exist.\n");
        return;
    }

    printf("\n--- User Records ---\n");
    while (fscanf(fp, "%d %s %d", &u.id, u.name, &u.age) == 3) {
        printf("ID: %d  Name: %s  Age: %d\n", u.id, u.name, u.age);
    }
    fclose(fp);
}

void updateUser() {
    struct User u;
    int id, found = 0;
    FILE *fp = fopen(FILE_NAME, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (fp == NULL || temp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    printf("Enter ID to update: ");
    scanf("%d", &id);

    while (fscanf(fp, "%d %s %d", &u.id, u.name, &u.age) == 3) {
        if (u.id == id) {
            found = 1;
            printf("Enter new Name: ");
            scanf("%s", u.name);
            printf("Enter new Age: ");
            scanf("%d", &u.age);
        }
        fprintf(temp, "%d %s %d\n", u.id, u.name, u.age);
    }

    fclose(fp);
    fclose(temp);
    remove(FILE_NAME);
    rename("temp.txt", FILE_NAME);

    if (found)
        printf("User updated successfully.\n");
    else
        printf("User with ID %d not found.\n", id);
}

void deleteUser() {
    struct User u;
    int id, found = 0;
    FILE *fp = fopen(FILE_NAME, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (fp == NULL || temp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    printf("Enter ID to delete: ");
    scanf("%d", &id);

    while (fscanf(fp, "%d %s %d", &u.id, u.name, &u.age) == 3) {
        if (u.id == id) {
            found = 1;
            continue;
        }
        fprintf(temp, "%d %s %d\n", u.id, u.name, u.age);
    }

    fclose(fp);
    fclose(temp);
    remove(FILE_NAME);
    rename("temp.txt", FILE_NAME);

    if (found)
        printf("User deleted successfully.\n");
    else
        printf("User with ID %d not found.\n", id);
}

void ensureFileExists() {
    FILE *fp = fopen(FILE_NAME, "a");
    if (fp == NULL) {
        printf("Error creating file.\n");
        exit(1);
    }
    fclose(fp);
}

int main() {
    int choice;
    ensureFileExists();

    while (1) {
        printf("\nUSER CRUD SYSTEM : \n");
        printf("1. Create User\n");
        printf("2. Read Users\n");
        printf("3. Update User\n");
        printf("4. Delete User\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: createUser(); break;
            case 2: readUsers(); break;
            case 3: updateUser(); break;
            case 4: deleteUser(); break;
            case 5: exit(0);
            default: printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}
