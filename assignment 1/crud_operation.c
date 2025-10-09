#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char FILE_NAME[] = "users.txt"; 

struct User {
    int id;
    char name[50];
    int age;
};

struct User getUserInput() {
    struct User u;
    printf("Enter ID: ");
    scanf("%d", &u.id);
    printf("Enter Name: ");
    scanf("%s", u.name);
    printf("Enter Age: ");
    scanf("%d", &u.age);
    return u;
}

void createUser() {
    struct User u = getUserInput();
    FILE *fp = fopen(FILE_NAME, "a");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }
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

int updateUser() {
    struct User u;
    int id, found = 0;
    FILE *fp = fopen(FILE_NAME, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (fp == NULL || temp == NULL) {
        printf("Error opening file.\n");
        return -1;
    }

    printf("Enter ID to update: ");
    scanf("%d", &id);

    while (fscanf(fp, "%d %s %d", &u.id, u.name, &u.age) == 3) {
        if (u.id == id) {
            found = 1;
            printf("Enter new details:\n");
            struct User newU = getUserInput();
            u = newU;
        }
        fprintf(temp, "%d %s %d\n", u.id, u.name, u.age);
    }

    fclose(fp);
    fclose(temp);
    remove(FILE_NAME);
    rename("temp.txt", FILE_NAME);

    return found;
}

int deleteUser() {
    struct User u;
    int id, found = 0;
    FILE *fp = fopen(FILE_NAME, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (fp == NULL || temp == NULL) {
        printf("Error opening file.\n");
        return -1;
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

    return found;
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
    int choice, status;
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
            case 1:
                createUser();
                break;
            case 2:
                readUsers();
                break;
            case 3:
                status = updateUser();
                if (status == 1)
                    printf("User updated successfully.\n");
                else if (status == 0)
                    printf("User not found.\n");
                else
                    printf("Error updating user.\n");
                break;
            case 4:
                status = deleteUser();
                if (status == 1)
                    printf("User deleted successfully.\n");
                else if (status == 0)
                    printf("User not found.\n");
                else
                    printf("Error deleting user.\n");
                break;
            case 5:
                exit(0);
            default:
                printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}
