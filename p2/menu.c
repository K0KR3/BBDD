#include <stdio.h>
#include <stdlib.h>
#include <sql.h>
#include <string.h>
#include <sqlext.h>
#include "odbc.h"
#include "customers.h"

void product_stock(SQLHDBC dbc);
void product_find(SQLHDBC dbc);

void order_open(SQLHDBC dbc);
void order_range(SQLHDBC dbc);
void order_detail(SQLHDBC dbc);

void customer_list_products(SQLHDBC dbc);
void customer_balance(SQLHDBC dbc);

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

void product_stock(SQLHDBC dbc) {
    SQLHSTMT stmt = NULL;
    SQLRETURN ret;
    SQLCHAR productcode[16];
    SQLINTEGER quantityinstock;
    SQLLEN indicator;

    printf("Enter product code > ");
    if (fgets((char*)productcode, sizeof(productcode), stdin)) {
        size_t len = strlen((char*)productcode);
        if (len > 0 && productcode[len-1] == '\n') {
            productcode[len-1] = '\0';
        }
    } else {
        return;
    }

    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("SQLAllocHandle", dbc, SQL_HANDLE_DBC);
        return;
    }

    ret = SQLPrepare(stmt, (SQLCHAR*)"SELECT quantityinstock FROM products WHERE productcode = ?", SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("SQLPrepare", stmt, SQL_HANDLE_STMT);
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return;
    }

    ret = SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 15, 0, productcode, 0, NULL);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("SQLBindParameter", stmt, SQL_HANDLE_STMT);
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return;
    }

    ret = SQLExecute(stmt);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("SQLExecute", stmt, SQL_HANDLE_STMT);
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return;
    }

    ret = SQLBindCol(stmt, 1, SQL_C_SLONG, &quantityinstock, sizeof(SQLINTEGER), &indicator);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("SQLBindCol", stmt, SQL_HANDLE_STMT);
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return;
    }

    ret = SQLFetch(stmt);
    if (SQL_SUCCEEDED(ret)) {
        printf("%d -> stock for product with id = %s\n", quantityinstock, productcode);
    } else if (ret == SQL_NO_DATA) {
        printf("Product not found.\n");
    } else {
        odbc_extract_error("SQLFetch", stmt, SQL_HANDLE_STMT);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
}

void product_find(SQLHDBC dbc) {}
void order_open(SQLHDBC dbc) {}
void order_range(SQLHDBC dbc) {}
void order_detail(SQLHDBC dbc) {}
void customer_list_products(SQLHDBC dbc) {}
void customer_balance(SQLHDBC dbc) {}

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