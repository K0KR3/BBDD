#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include "products.h"
#include "odbc.h"

#define STR_LEN 128


void product_stock(SQLHDBC dbc) {
    SQLHSTMT stmt = NULL;
    SQLRETURN ret;
    SQLCHAR productcode[16];
    SQLINTEGER quantityinstock;
    SQLLEN indicator;

    printf("Enter productcode > ");
    (void) fflush(stdout);

    if (scanf("%15s", (char *)productcode) == EOF) {
        return;
    }
    while (getchar() != '\n');

    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("SQLAllocHandle", dbc, SQL_HANDLE_DBC);
        return;
    }

    ret = SQLPrepare(stmt, (SQLCHAR *)"SELECT quantityinstock FROM products WHERE productcode = ?", SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("SQLPrepare", stmt, SQL_HANDLE_STMT);
        (void) SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return;
    }

    ret = SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 15, 0, productcode, 0, NULL);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("SQLBindParameter", stmt, SQL_HANDLE_STMT);
        (void) SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return;
    }

    ret = SQLExecute(stmt);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("SQLExecute", stmt, SQL_HANDLE_STMT);
        (void) SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return;
    }

    ret = SQLBindCol(stmt, 1, SQL_C_SLONG, &quantityinstock, sizeof(SQLINTEGER), &indicator);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("SQLBindCol", stmt, SQL_HANDLE_STMT);
        (void) SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return;
    }

    ret = SQLFetch(stmt);
    if (SQL_SUCCEEDED(ret)) {
        printf("%d\n", (int)quantityinstock);
    } else if (ret == SQL_NO_DATA) {
        printf("Product not found.\n");
    } else {
        odbc_extract_error("SQLFetch", stmt, SQL_HANDLE_STMT);
    }

    (void) SQLCloseCursor(stmt);
    (void) SQLFreeHandle(SQL_HANDLE_STMT, stmt);
}

void product_find(SQLHDBC dbc) {
    SQLHSTMT stmt = NULL;
    SQLRETURN ret;
    SQLCHAR search_input[64];
    SQLCHAR like_pattern[70];
    SQLCHAR productcode[50];
    SQLCHAR productname[100];
    SQLLEN ind_code, ind_name;

    printf("Enter productname > ");
    (void) fflush(stdout);

    if (scanf(" %63[^\n]", (char *)search_input) == EOF) {
        return;
    }
    while (getchar() != '\n');

    (void) sprintf((char *)like_pattern, "%%%s%%", (char *)search_input);

    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("SQLAllocHandle", dbc, SQL_HANDLE_DBC);
        return;
    }

    ret = SQLPrepare(stmt, (SQLCHAR *)"SELECT productcode, productname FROM products WHERE productname LIKE ? ORDER BY productcode", SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("SQLPrepare", stmt, SQL_HANDLE_STMT);
        (void) SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return;
    }

    ret = SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 70, 0, like_pattern, 0, NULL);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("SQLBindParameter", stmt, SQL_HANDLE_STMT);
        (void) SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return;
    }

    ret = SQLExecute(stmt);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("SQLExecute", stmt, SQL_HANDLE_STMT);
        (void) SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return;
    }

    (void) SQLBindCol(stmt, 1, SQL_C_CHAR, productcode, sizeof(productcode), &ind_code);
    (void) SQLBindCol(stmt, 2, SQL_C_CHAR, productname, sizeof(productname), &ind_name);

    while (SQL_SUCCEEDED(SQLFetch(stmt))) {
        printf("%s %s\n", (char *)productcode, (char *)productname);
    }

    (void) SQLCloseCursor(stmt);
    (void) SQLFreeHandle(SQL_HANDLE_STMT, stmt);
}
