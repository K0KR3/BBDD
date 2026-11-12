#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include "orders.h"
#include "odbc.h"

#define STR_LEN 128

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

    ret = SQLPrepare(stmt, (SQLCHAR *)"SELECT ordernumber FROM orders WHERE shippeddate IS NULL ORDER BY ordernumber", SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("SQLPrepare", stmt, SQL_HANDLE_STMT);
        (void) SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return;
    }

    ret = SQLExecute(stmt);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("SQLExecute", stmt, SQL_HANDLE_STMT);
        (void) SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return;
    }

    (void) SQLBindCol(stmt, 1, SQL_C_SLONG, &ordernumber, sizeof(SQLINTEGER), &ind_ordernumber);

    while (SQL_SUCCEEDED(SQLFetch(stmt))) {
        printf("%d\n", (int)ordernumber);
    }

    (void) SQLCloseCursor(stmt);
    (void) SQLFreeHandle(SQL_HANDLE_STMT, stmt);
}

void order_range(SQLHDBC dbc) {
    SQLHSTMT stmt = NULL;
    SQLRETURN ret;
    SQLCHAR date1[20], date2[20];
    SQLINTEGER ordernumber;
    SQLCHAR orderdate[20];
    SQLCHAR shippeddate[20];
    SQLLEN ind_ordernumber, ind_orderdate, ind_shippeddate;

    printf("Enter dates (YYYY-MM-DD - YYYY-MM-DD) > ");
    (void) fflush(stdout);

    if (scanf("%19s - %19s", (char *)date1, (char *)date2) != 2) {
        while (getchar() != '\n');
        printf("Invalid date format. Use YYYY-MM-DD - YYYY-MM-DD\n");
        return;
    }
    while (getchar() != '\n');

    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("SQLAllocHandle", dbc, SQL_HANDLE_DBC);
        return;
    }

    ret = SQLPrepare(stmt, (SQLCHAR *)
        "SELECT ordernumber, orderdate, shippeddate "
        "FROM orders "
        "WHERE orderdate BETWEEN ? AND ? "
        "ORDER BY ordernumber",
        SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("SQLPrepare", stmt, SQL_HANDLE_STMT);
        (void) SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return;
    }

    (void) SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_TYPE_DATE, 10, 0, date1, 0, NULL);
    (void) SQLBindParameter(stmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_TYPE_DATE, 10, 0, date2, 0, NULL);

    ret = SQLExecute(stmt);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("SQLExecute", stmt, SQL_HANDLE_STMT);
        (void) SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return;
    }

    (void) SQLBindCol(stmt, 1, SQL_C_SLONG, &ordernumber, sizeof(SQLINTEGER), &ind_ordernumber);
    (void) SQLBindCol(stmt, 2, SQL_C_CHAR, orderdate, sizeof(orderdate), &ind_orderdate);
    (void) SQLBindCol(stmt, 3, SQL_C_CHAR, shippeddate, sizeof(shippeddate), &ind_shippeddate);

    while (SQL_SUCCEEDED(SQLFetch(stmt))) {
        if (ind_shippeddate == SQL_NULL_DATA) {
            printf("%d %s NULL\n", (int)ordernumber, (char *)orderdate);
        } else {
            printf("%d %s %s\n", (int)ordernumber, (char *)orderdate, (char *)shippeddate);
        }
    }

    (void) SQLCloseCursor(stmt);
    (void) SQLFreeHandle(SQL_HANDLE_STMT, stmt);
}

void order_detail(SQLHDBC dbc) {

    SQLHSTMT stmt = NULL;
    SQLRETURN ret;
    int ordernumber = 0;
    SQLCHAR orderdate[20];
    SQLCHAR status[20];
    double total_cost = 0;
    SQLLEN ind_date, ind_status, ind_cost;
    SQLCHAR productcode[50];
    SQLINTEGER quantity;
    double price;
    SQLLEN ind_code, ind_qty, ind_price;

    printf("Enter ordernumber > ");
    (void) fflush(stdout);

    if (scanf("%d", &ordernumber) == EOF) {
        return;
    }
    while (getchar() != '\n');

    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("SQLAllocHandle", dbc, SQL_HANDLE_DBC);
        return;
    }

    ret = SQLPrepare(stmt, (SQLCHAR*)
        "SELECT o.orderdate, o.status, SUM(d.quantityordered * d.priceeach) "
        "FROM orders o JOIN orderdetails d ON o.ordernumber = d.ordernumber "
        "WHERE o.ordernumber = ? "
        "GROUP BY o.orderdate, o.status",
        SQL_NTS);
    (void) SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &ordernumber, 0, NULL);
    (void) SQLExecute(stmt);

    (void) SQLBindCol(stmt, 1, SQL_C_CHAR, orderdate, sizeof(orderdate), &ind_date);
    (void) SQLBindCol(stmt, 2, SQL_C_CHAR, status, sizeof(status), &ind_status);
    (void) SQLBindCol(stmt, 3, SQL_C_DOUBLE, &total_cost, 0, &ind_cost);

    if (SQL_SUCCEEDED(SQLFetch(stmt))) {
        printf("%s %s\n", orderdate, status);
        printf("%.2f\n", total_cost);
    } else {
        printf("Order not found.\n");
        (void) SQLCloseCursor(stmt);
        (void) SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return;
    }

    (void) SQLCloseCursor(stmt);

    ret = SQLPrepare(stmt, (SQLCHAR*)
        "SELECT productcode, quantityordered, priceeach "
        "FROM orderdetails "
        "WHERE ordernumber = ? "
        "ORDER BY orderlinenumber",
        SQL_NTS);
    (void) SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &ordernumber, 0, NULL);
    (void) SQLExecute(stmt);

    (void) SQLBindCol(stmt, 1, SQL_C_CHAR, productcode, sizeof(productcode), &ind_code);
    (void) SQLBindCol(stmt, 2, SQL_C_SLONG, &quantity, sizeof(SQLINTEGER), &ind_qty);
    (void) SQLBindCol(stmt, 3, SQL_C_DOUBLE, &price, 0, &ind_price);

    while (SQL_SUCCEEDED(SQLFetch(stmt))) {
        printf("%s %d %.2f\n", productcode, quantity, price);
    }

    (void) SQLCloseCursor(stmt);
    (void) SQLFreeHandle(SQL_HANDLE_STMT, stmt);
}
