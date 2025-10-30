#ifndef INVENTORY_MANAGEMENT_H
#define INVENTORY_MANAGEMENT_H

#include <stdbool.h>

#define NAME_LEN 50

typedef struct {
    int id;
    char name[NAME_LEN];
    float price;
    int quantity;
} Product;

Product* inputProductDetails();
void printProduct(const Product *p);

bool addProduct(Product **products, int *productCount);
void viewProducts(Product *products, int productCount);
bool updateQuantity(Product *products, int productCount);
Product* searchByID(Product *products, int productCount, int id);
Product* searchByName(Product *products, int productCount, const char *name);
void searchByPriceRange(Product *products, int productCount);

bool deleteProduct(Product **products, int *productCount);

#endif
