#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inventory_management.h"

Product* inputProductDetails() {
    Product *newProduct = malloc(sizeof(Product));
    if (!newProduct) return NULL;

    do {
        printf("Enter Product ID (positive number): ");
        scanf("%d", &newProduct->id);
        if (newProduct->id <= 0)
            printf("Invalid ID! Try again.\n");
    } while (newProduct->id <= 0);
    getchar();

    do {
        printf("Enter Product Name: ");
        fgets(newProduct->name, NAME_LEN, stdin);
        newProduct->name[strcspn(newProduct->name, "\n")] = '\0';

        if (strlen(newProduct->name) == 0)
            printf("Name cannot be empty! Try again.\n");

    } while (strlen(newProduct->name) == 0);

    do {
        printf("Enter Product Price (>0): ");
        scanf("%f", &newProduct->price);
        if (newProduct->price <= 0)
            printf("Invalid price! Try again.\n");
    } while (newProduct->price <= 0);

    do {
        printf("Enter Product Quantity (>=0): ");
        scanf("%d", &newProduct->quantity);
        if (newProduct->quantity < 0)
            printf("Invalid quantity! Try again.\n");
    } while (newProduct->quantity < 0);

    return newProduct;
}

void printProduct(const Product *newProduct) {
    printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
           newProduct->id, newProduct->name, newProduct->price, newProduct->quantity);
}

bool addProduct(Product **products, int *productCount) {
    Product *newProduct = inputProductDetails();
    if (!newProduct) return false;

    (*productCount)++;
    *products = realloc(*products, (*productCount) * sizeof(Product));
    if (*products == NULL) {
        printf("Memory reallocation failed!\n");
        exit(1);
    }

    (*products)[*productCount - 1] = *newProduct;
    free(newProduct);

    return true;
}

void viewProducts(Product *products, int productCount) {
    if (productCount == 0) {
        printf("No products available.\n");
        return;
    }

    printf("\n========= PRODUCT LIST =========\n");
    for (int i = 0; i < productCount; i++)
        printProduct(&products[i]);
}

Product* searchByID(Product *products, int productCount, int id) {
    for (int i = 0; i < productCount; i++)
        if (products[i].id == id)
            return &products[i];
    return NULL;
}

Product* searchByName(Product *products, int productCount, const char *name) {
    for (int i = 0; i < productCount; i++)
        if (strstr(products[i].name, name))
            return &products[i];
    return NULL;
}

void searchByPriceRange(Product *products, int productCount) {
    float min, max;
    printf("Enter minimum price: ");
    scanf("%f", &min);
    printf("Enter maximum price: ");
    scanf("%f", &max);

    if (min > max) {
        printf("Invalid range! Max should be >= Min.\n");
        return;
    }

    printf("\nProducts in price range:\n");
    for (int i = 0; i < productCount; i++) {
        if (products[i].price >= min && products[i].price <= max)
            printProduct(&products[i]);
    }
}

bool updateQuantity(Product *products, int productCount) {
    int id, newQty;
    printf("Enter Product ID to update quantity: ");
    scanf("%d", &id);

    Product *newProduct = searchByID(products, productCount, id);
    if (!newProduct) return false;

    printf("Enter new Quantity: ");
    scanf("%d", &newQty);

    if (newQty < 0)
        printf("Invalid quantity! Try again.\n");

    newProduct->quantity = newQty;
    return true;
}

bool deleteProduct(Product **products, int *productCount) {
    int id;
    printf("Enter Product ID to delete: ");
    scanf("%d", &id);

    int index = -1;
    for (int i = 0; i < *productCount; i++)
        if ((*products)[i].id == id)
            index = i;

    if (index == -1)
        return false;

    for (int i = index; i < *productCount - 1; i++)
        (*products)[i] = (*products)[i + 1];

    (*productCount)--;
    *products = realloc(*products, (*productCount) * sizeof(Product));

    return true;
}

int main() {
    Product *products = NULL;
    int productCount = 0, choice;
    int id;
    char name[NAME_LEN];

    while (1) {
        printf("\n========= INVENTORY MENU =========\n");
        printf("1. Add Product\n");
        printf("2. View All Products\n");
        printf("3. Update Quantity\n");
        printf("4. Search By ID\n");
        printf("5. Search By Name\n");
        printf("6. Search By Price Range\n");
        printf("7. Delete Product\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                addProduct(&products, &productCount);
                break;
            case 2:
                viewProducts(products, productCount);
                break;
            case 3:
                if (updateQuantity(products, productCount))
                    printf("Quantity updated.\n");
                else
                    printf("Product not found!\n");
                break;
            case 4:
                printf("Enter Product ID: ");
                scanf("%d", &id);
                if (searchByID(products, productCount, id))
                    printProduct(searchByID(products, productCount, id));
                else
                    printf("Product not found!\n");
                break;
            case 5:
                printf("Enter product name: ");
                fgets(name, NAME_LEN, stdin);
                name[strcspn(name, "\n")] = '\0';
                if (searchByName(products, productCount, name))
                    printProduct(searchByName(products, productCount, name));
                else
                    printf("Product not found!\n");
                break;
            case 6:
                searchByPriceRange(products, productCount);
                break;
            case 7:
                if (deleteProduct(&products, &productCount))
                    printf("Product deleted.\n");
                else
                    printf("Product not found!\n");
                break;
            case 8:
                free(products);
                products = NULL;
                return 0;
            default:
                printf("Invalid choice!\n");
        }
    }
}
