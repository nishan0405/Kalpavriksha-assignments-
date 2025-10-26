#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LEN 50

struct Product {
    int id;
    char name[NAME_LEN];
    float price;
    int quantity;
};

void addProduct(struct Product **products, int *n);
void viewProducts(struct Product *products, int n);
void updateQuantity(struct Product *products, int n);
void searchByID(struct Product *products, int n);
void searchByName(struct Product *products, int n);
void searchByPriceRange(struct Product *products, int n);
void deleteProduct(struct Product **products, int *n);

int main() {
    struct Product *products = NULL;
    int n, choice;

    printf("Enter initial number of products: ");
    scanf("%d", &n);
    getchar();

    products = (struct Product *)calloc(n, sizeof(struct Product));
    if (products == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        printf("\nEnter details for product %d:\n", i + 1);
        printf("Product ID: ");
        scanf("%d", &products[i].id);
        getchar();
        printf("Product Name: ");
        fgets(products[i].name, NAME_LEN, stdin);
        products[i].name[strcspn(products[i].name, "\n")] = '\0';
        printf("Product Price: ");
        scanf("%f", &products[i].price);
        printf("Product Quantity: ");
        scanf("%d", &products[i].quantity);
    }

    while (1) {
        printf("\n========= INVENTORY MENU =========\n");
        printf("1. Add New Product\n");
        printf("2. View All Products\n");
        printf("3. Update Quantity\n");
        printf("4. Search Product by ID\n");
        printf("5. Search Product by Name\n");
        printf("6. Search Product by Price Range\n");
        printf("7. Delete Product\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                addProduct(&products, &n);
                break;
            case 2:
                viewProducts(products, n);
                break;
            case 3:
                updateQuantity(products, n);
                break;
            case 4:
                searchByID(products, n);
                break;
            case 5:
                searchByName(products, n);
                break;
            case 6:
                searchByPriceRange(products, n);
                break;
            case 7:
                deleteProduct(&products, &n);
                break;
            case 8:
                printf("Memory released successfully. Exiting program...\n");
                free(products);
                return 0;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
}

void addProduct(struct Product **products, int *n) {
    (*n)++;
    *products = (struct Product *)realloc(*products, (*n) * sizeof(struct Product));
    if (*products == NULL) {
        printf("Memory reallocation failed!\n");
        exit(1);
    }

    struct Product *newProduct = &(*products)[*n - 1];
    printf("Enter new product details:\n");
    printf("Product ID: ");
    scanf("%d", &newProduct->id);
    getchar();
    printf("Product Name: ");
    fgets(newProduct->name, NAME_LEN, stdin);
    newProduct->name[strcspn(newProduct->name, "\n")] = '\0';
    printf("Product Price: ");
    scanf("%f", &newProduct->price);
    printf("Product Quantity: ");
    scanf("%d", &newProduct->quantity);
    printf("Product added successfully!\n");
}

void viewProducts(struct Product *products, int n) {
    if (n == 0) {
        printf("No products available.\n");
        return;
    }
    printf("\n========= PRODUCT LIST =========\n");
    for (int i = 0; i < n; i++) {
        printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
               products[i].id, products[i].name, products[i].price, products[i].quantity);
    }
}

void updateQuantity(struct Product *products, int n) {
    int id, found = 0, newQty;
    printf("Enter Product ID to update quantity: ");
    scanf("%d", &id);
    for (int i = 0; i < n; i++) {
        if (products[i].id == id) {
            printf("Enter new Quantity: ");
            scanf("%d", &newQty);
            products[i].quantity = newQty;
            printf("Quantity updated successfully!\n");
            found = 1;
            break;
        }
    }
    if (!found)
        printf("Product not found!\n");
}

void searchByID(struct Product *products, int n) {
    int id, found = 0;
    printf("Enter Product ID to search: ");
    scanf("%d", &id);
    for (int i = 0; i < n; i++) {
        if (products[i].id == id) {
            printf("Product Found: Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
                   products[i].id, products[i].name, products[i].price, products[i].quantity);
            found = 1;
            break;
        }
    }
    if (!found)
        printf("Product not found!\n");
}

void searchByName(struct Product *products, int n) {
    char searchName[NAME_LEN];
    int found = 0;
    printf("Enter name to search: ");
    fgets(searchName, NAME_LEN, stdin);
    searchName[strcspn(searchName, "\n")] = '\0';
    printf("Products Found:\n");
    for (int i = 0; i < n; i++) {
        if (strstr(products[i].name, searchName)) {
            printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
                   products[i].id, products[i].name, products[i].price, products[i].quantity);
            found = 1;
        }
    }
    if (!found)
        printf("No matching products found!\n");
}

void searchByPriceRange(struct Product *products, int n) {
    float min, max;
    int found = 0;
    printf("Enter minimum price: ");
    scanf("%f", &min);
    printf("Enter maximum price: ");
    scanf("%f", &max);
    printf("Products in price range:\n");
    for (int i = 0; i < n; i++) {
        if (products[i].price >= min && products[i].price <= max) {
            printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
                   products[i].id, products[i].name, products[i].price, products[i].quantity);
            found = 1;
        }
    }
    if (!found)
        printf("No products found in this price range!\n");
}

void deleteProduct(struct Product **products, int *n) {
    int id, found = -1;
    printf("Enter Product ID to delete: ");
    scanf("%d", &id);
    for (int i = 0; i < *n; i++) {
        if ((*products)[i].id == id) {
            found = i;
            break;
        }
    }
    if (found == -1) {
        printf("Product not found!\n");
        return;
    }
    for (int i = found; i < *n - 1; i++) {
        (*products)[i] = (*products)[i + 1];
    }
    (*n)--;
    *products = (struct Product *)realloc(*products, (*n) * sizeof(struct Product));
    if (*products == NULL && *n > 0) {
        printf("Memory reallocation failed after deletion!\n");
        exit(1);
    }
    printf("Product deleted successfully!\n");
}
