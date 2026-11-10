#include <stdio.h>
#include <stdlib.h>
#include <sql.h>
#include <string.h>
#include <sqlext.h>
#include "odbc.h"

void product_stock(SQLHDBC dbc);
void product_find(SQLHDBC dbc);

void order_open(SQLHDBC dbc);
void order_range(SQLHDBC dbc);
void order_detail(SQLHDBC dbc);

void customer_find(SQLHDBC dbc);
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

void product_find(SQLHDBC dbc) {
    SQLHSTMT stmt = NULL;
    SQLRETURN ret;
    SQLCHAR search_input[64];
    SQLCHAR like_pattern[70]; 
    
    SQLCHAR productcode[50];
    SQLCHAR productname[100];
    SQLLEN ind_code, ind_name;

    printf("Enter product name > ");
    if (fgets((char*)search_input, sizeof(search_input), stdin)) {
        size_t len = strlen((char*)search_input);
        if (len > 0 && search_input[len-1] == '\n') {
            search_input[len-1] = '\0';
        }
    } else {
        return;
    }

    snprintf((char*)like_pattern, sizeof(like_pattern), "%%%s%%", search_input);

    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("SQLAllocHandle", dbc, SQL_HANDLE_DBC);
        return;
    }

    ret = SQLPrepare(stmt, (SQLCHAR*)"SELECT productcode, productname FROM products WHERE productname LIKE ? ORDER BY productcode", SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("SQLPrepare", stmt, SQL_HANDLE_STMT);
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return;
    }

    ret = SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 70, 0, like_pattern, 0, NULL);
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

    SQLBindCol(stmt, 1, SQL_C_CHAR, productcode, sizeof(productcode), &ind_code);
    SQLBindCol(stmt, 2, SQL_C_CHAR, productname, sizeof(productname), &ind_name);

    while (SQLFetch(stmt) == SQL_SUCCESS) {
        printf("%s %s\n", productcode, productname);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
}
void order_open(SQLHDBC dbc) {
    SQLHSTMT stmt = NULL;
    SQLRETURN ret;
    SQLINTEGER ordernumber;
    SQLLEN ind_ordernumber;

    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("SQLAllocHandle", dbc, SQL_HANDLE_DBC);
        return;
    }

    ret = SQLPrepare(stmt, (SQLCHAR*)"SELECT ordernumber FROM orders WHERE shippeddate IS NULL ORDER BY ordernumber", SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("SQLPrepare", stmt, SQL_HANDLE_STMT);
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return;
    }

    ret = SQLExecute(stmt);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("SQLExecute", stmt, SQL_HANDLE_STMT);
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return;
    }

    SQLBindCol(stmt, 1, SQL_C_SLONG, &ordernumber, sizeof(SQLINTEGER), &ind_ordernumber);

    while (SQLFetch(stmt) == SQL_SUCCESS) {
        printf("%d\n", ordernumber);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
}
void order_range(SQLHDBC dbc) {
    SQLHSTMT stmt = NULL;
    SQLRETURN ret;
    char input[100];
    SQLCHAR date1[20], date2[20];
    
    SQLINTEGER ordernumber;
    SQLCHAR orderdate[20];
    SQLCHAR shippeddate[20];
    SQLLEN ind_ordernumber, ind_orderdate, ind_shippeddate;

    printf("Enter dates (YYYY-MM-DD - YYYY-MM-DD) > ");
    if (fgets(input, sizeof(input), stdin)) {
        /* sscanf lee el formato exacto y guarda cada fecha en su variable */
        if (sscanf(input, "%19s - %19s", date1, date2) != 2) {
            printf("Invalid date format. Use YYYY-MM-DD - YYYY-MM-DD\n");
            return;
        }
    } else {
        return;
    }

    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("SQLAllocHandle", dbc, SQL_HANDLE_DBC);
        return;
    }

    ret = SQLPrepare(stmt, (SQLCHAR*)"SELECT ordernumber, orderdate, shippeddate FROM orders WHERE orderdate BETWEEN ? AND ? ORDER BY ordernumber", SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("SQLPrepare", stmt, SQL_HANDLE_STMT);
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return;
    }

    SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 10, 0, date1, 0, NULL);
    SQLBindParameter(stmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 10, 0, date2, 0, NULL);

    ret = SQLExecute(stmt);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("SQLExecute", stmt, SQL_HANDLE_STMT);
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return;
    }

    SQLBindCol(stmt, 1, SQL_C_SLONG, &ordernumber, sizeof(SQLINTEGER), &ind_ordernumber);
    SQLBindCol(stmt, 2, SQL_C_CHAR, orderdate, sizeof(orderdate), &ind_orderdate);
    SQLBindCol(stmt, 3, SQL_C_CHAR, shippeddate, sizeof(shippeddate), &ind_shippeddate);

    while (SQLFetch(stmt) == SQL_SUCCESS) {
        if (ind_shippeddate == SQL_NULL_DATA) {
            printf("%d %s NULL\n", ordernumber, orderdate);
        } else {
            printf("%d %s %s\n", ordernumber, orderdate, shippeddate);
        }
    }

    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
}
void order_detail(SQLHDBC dbc) {
    SQLHSTMT stmt = NULL;
    SQLRETURN ret;
    SQLCHAR ordernumber_str[20];
    
    SQLCHAR orderdate[20];
    SQLCHAR status[20];
    double total_cost = 0;
    SQLLEN ind_date, ind_status, ind_cost;

    SQLCHAR productcode[50];
    SQLINTEGER quantity;
    double price;
    SQLLEN ind_code, ind_qty, ind_price;

    printf("Enter order number > ");
    if (fgets((char*)ordernumber_str, sizeof(ordernumber_str), stdin)) {
        size_t len = strlen((char*)ordernumber_str);
        if (len > 0 && ordernumber_str[len-1] == '\n') {
            ordernumber_str[len-1] = '\0';
        }
    } else {
        return;
    }

    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("SQLAllocHandle", dbc, SQL_HANDLE_DBC);
        return;
    }

    ret = SQLPrepare(stmt, (SQLCHAR*)
        "SELECT o.orderdate, o.status, SUM(d.quantityordered * d.priceeach) "
        "FROM orders o JOIN orderdetails d ON o.ordernumber = d.ordernumber "
        "WHERE o.ordernumber = ? "
        "GROUP BY o.orderdate, o.status", SQL_NTS);
        
    SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_INTEGER, 15, 0, ordernumber_str, 0, NULL);
    SQLExecute(stmt);

    SQLBindCol(stmt, 1, SQL_C_CHAR, orderdate, sizeof(orderdate), &ind_date);
    SQLBindCol(stmt, 2, SQL_C_CHAR, status, sizeof(status), &ind_status);
    SQLBindCol(stmt, 3, SQL_C_DOUBLE, &total_cost, 0, &ind_cost);

    if (SQLFetch(stmt) == SQL_SUCCESS) {
        printf("%s %s\n", orderdate, status);
        printf("%.2f\n", total_cost);
    } else {
        printf("Order not found.\n");
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return;
    }
    
    SQLCloseCursor(stmt);

    ret = SQLPrepare(stmt, (SQLCHAR*)
        "SELECT productcode, quantityordered, priceeach "
        "FROM orderdetails "
        "WHERE ordernumber = ? "
        "ORDER BY orderlinenumber", SQL_NTS);
        
    SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_INTEGER, 15, 0, ordernumber_str, 0, NULL);
    SQLExecute(stmt);

    SQLBindCol(stmt, 1, SQL_C_CHAR, productcode, sizeof(productcode), &ind_code);
    SQLBindCol(stmt, 2, SQL_C_SLONG, &quantity, sizeof(SQLINTEGER), &ind_qty);
    SQLBindCol(stmt, 3, SQL_C_DOUBLE, &price, 0, &ind_price);

    while (SQLFetch(stmt) == SQL_SUCCESS) {
        printf("%s %d %.2f\n", productcode, quantity, price);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
}
void customer_find(SQLHDBC dbc) {}
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