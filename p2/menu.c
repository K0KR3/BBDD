#include <stdio.h>
#include <stdlib.h>
#include <sql.h>
#include <string.h>
#include <sqlext.h>
#include "odbc.h"
#include "customers.h"
#include "orders.h"
#include "products.h"

int ShowMainMenu(void) {
    int nSelected = 0;
    char buf[16];

    do {
        printf(" 1. Products\n"
               " 2. Orders\n"
               " 3. Customers\n"
               " 4. Exit\n\n"
               "Enter a number that corresponds to your choice > ");
        if (!fgets(buf, 16, stdin))
            nSelected = 0;
        else
            nSelected = atoi(buf);
        printf("\n");

        if ((nSelected < 1) || (nSelected > 4)) {
            printf("You have entered an invalid choice. Please try again\n\n\n");
        }
    } while ((nSelected < 1) || (nSelected > 4));

    return nSelected;
}

void ShowProductsMenu(SQLHDBC dbc) {
    int nSelected = 0;
    char buf[16];

    do {
        printf(" 1. Stock\n"
               " 2. Find\n"
               " 3. Back\n\n"
               "Enter a number that corresponds to your choice > ");
        if (!fgets(buf, 16, stdin))
            nSelected = 0;
        else
            nSelected = atoi(buf);
        printf("\n");

        switch (nSelected) {
            case 1:
                product_stock(dbc);
                break;
            case 2:
                product_find(dbc);
                break;
            case 3:
                break;
            default:
                printf("You have entered an invalid choice. Please try again\n\n\n");
                break;
        }
    } while (nSelected != 3);
}

void ShowOrdersMenu(SQLHDBC dbc) {
    int nSelected = 0;
    char buf[16];

    do {
        printf(" 1. Open\n"
               " 2. Range\n"
               " 3. Detail\n"
               " 4. Back\n\n"
               "Enter a number that corresponds to your choice > ");
        if (!fgets(buf, 16, stdin))
            nSelected = 0;
        else
            nSelected = atoi(buf);
        printf("\n");

        switch (nSelected) {
            case 1:
                order_open(dbc);
                break;
            case 2:
                order_range(dbc);
                break;
            case 3:
                order_detail(dbc);
                break;
            case 4:
                break;
            default:
                printf("You have entered an invalid choice. Please try again\n\n\n");
                break;
        }
    } while (nSelected != 4);
}

void ShowCustomersMenu(SQLHDBC dbc) {
    int nSelected = 0;
    char buf[16];

    do {
        printf(" 1. Find\n"
               " 2. List Products\n"
               " 3. Balance\n"
               " 4. Back\n\n"
               "Enter a number that corresponds to your choice > ");
        if (!fgets(buf, 16, stdin))
            nSelected = 0;
        else
            nSelected = atoi(buf);
        printf("\n");

        switch (nSelected) {
            case 1:
                customer_find(dbc);
                break;
            case 2:
                customer_list_products(dbc);
                break;
            case 3:
                customer_balance(dbc);
                break;
            case 4:
                break;
            default:
                printf("You have entered an invalid choice. Please try again\n\n\n");
                break;
        }
    } while (nSelected != 4);
}

int main(void) {
    SQLHENV env;
    SQLHDBC dbc;
    int nChoice = 0;

    if (!SQL_SUCCEEDED(odbc_connect(&env, &dbc))) {
        return EXIT_FAILURE;
    }

    do {
        nChoice = ShowMainMenu();
        switch (nChoice) {
            case 1:
                ShowProductsMenu(dbc);
                break;
            case 2:
                ShowOrdersMenu(dbc);
                break;
            case 3:
                ShowCustomersMenu(dbc);
                break;
            case 4:
                break;
        }
    } while (nChoice != 4);

    odbc_disconnect(env, dbc);
    return EXIT_SUCCESS;
}