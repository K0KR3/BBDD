#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include "customers.h"
#include "odbc.h"

#define STR_LEN 128

void customer_find(SQLHDBC dbc) {
    
    SQLHSTMT stmt = NULL;
    SQLRETURN ret;
    char nombre[STR_LEN] = "\0";
    char pattern[STR_LEN * 2] = "\0";
    int customernumber = 0;
    char customername[STR_LEN] = "\0";
    char contactfirstname[STR_LEN] = "\0";
    char contactlastname[STR_LEN] = "\0";

    printf("Enter customer name > ");
    (void) fflush(stdout);

    if (scanf("%127s", nombre) == EOF) {
        return;
    }
    while(getchar() != '\n');

    sprintf(pattern, "%%%s%%", nombre);

    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    if (!SQL_SUCCEEDED(ret)) {
        return;
    }

    ret = SQLPrepare(stmt, (SQLCHAR *)
        "select customernumber, customername, contactfirstname, contactlastname "
        "from customers "
        "where contactfirstname like ? or contactlastname like ? "
        "order by customernumber",
        SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("SQLPrepare", stmt, SQL_HANDLE_STMT);
        (void) SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return;
    }

    (void) SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 0, 0, pattern, 0, NULL);
    (void) SQLBindParameter(stmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 0, 0, pattern, 0, NULL);

    ret = SQLExecute(stmt);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("SQLExecute", stmt, SQL_HANDLE_STMT);
        (void) SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return;
    }

    (void) SQLBindCol(stmt, 1, SQL_C_SLONG, &customernumber, sizeof(customernumber), NULL);
    (void) SQLBindCol(stmt, 2, SQL_C_CHAR, (SQLCHAR *) customername, sizeof(customername), NULL);
    (void) SQLBindCol(stmt, 3, SQL_C_CHAR, (SQLCHAR *) contactfirstname, sizeof(contactfirstname), NULL);
    (void) SQLBindCol(stmt, 4, SQL_C_CHAR, (SQLCHAR *) contactlastname, sizeof(contactlastname), NULL);

    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        printf("%d %s %s %s\n", customernumber, customername, contactfirstname, contactlastname);
    }

    (void) SQLCloseCursor(stmt);
    (void) SQLFreeHandle(SQL_HANDLE_STMT, stmt);

}

void customer_list_products(SQLHDBC dbc) {

    SQLHSTMT stmt = NULL;
    SQLRETURN ret;
    int numero = 0;
    int total = 0;
    char productname[STR_LEN] = "\0";

    printf("Enter customer number > ");
    (void) fflush(stdout);

    if (scanf("%d", &numero) == EOF) {
        return;
    }
    while (getchar() != '\n');

    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    if (!SQL_SUCCEEDED(ret)) {
        return;
    }

    ret = SQLPrepare(stmt, (SQLCHAR *)
        "select p.productname, sum(od.quantityordered) "
        "from orders o "
        "join orderdetails od on o.ordernumber = od.ordernumber "
        "join products p on od.productcode = p.productcode "
        "where o.customernumber = ? "
        "group by p.productcode, p.productname "
        "order by p.productcode",
        SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("SQLPrepare", stmt, SQL_HANDLE_STMT);
        (void) SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return;
    }

    (void) SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &numero, 0, NULL);

    ret = SQLExecute(stmt);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("SQLExecute", stmt, SQL_HANDLE_STMT);
        (void) SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return;
    }

    (void) SQLBindCol(stmt, 1, SQL_C_CHAR, (SQLCHAR *) productname, sizeof(productname), NULL);
    (void) SQLBindCol(stmt, 2, SQL_C_SLONG, &total, sizeof(total), NULL);

    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        printf("%s %d\n", productname, total);
    }

    (void) SQLCloseCursor(stmt);
    (void) SQLFreeHandle(SQL_HANDLE_STMT, stmt);

}

void customer_balance(SQLHDBC dbc) {
    SQLHSTMT stmt = NULL;
    SQLRETURN ret;
    int numero = 0;
    double pagado = 0.0;
    double comprado = 0.0;
    double balance = 0.0;

    printf("Enter customer number > ");
    (void) fflush(stdout);

    if (scanf("%d", &numero) == EOF) {
        return;
    }
    while (getchar() != '\n');

    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    if (!SQL_SUCCEEDED(ret)) {
        return;
    }

    ret = SQLPrepare(stmt, (SQLCHAR *)
        "select sum(amount) from payments where customernumber = ?",
        SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        (void) SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return;
    }

    (void) SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &numero, 0, NULL);
    (void) SQLExecute(stmt);
    (void) SQLBindCol(stmt, 1, SQL_C_DOUBLE, &pagado, sizeof(pagado), NULL);
    (void) SQLFetch(stmt);
    (void) SQLCloseCursor(stmt);

    ret = SQLPrepare(stmt, (SQLCHAR *)
        "select sum(od.quantityordered * od.priceeach) "
        "from orders o join orderdetails od on o.ordernumber = od.ordernumber "
        "where o.customernumber = ?",
        SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        (void) SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return;
    }

    (void) SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &numero, 0, NULL);
    (void) SQLExecute(stmt);
    (void) SQLBindCol(stmt, 1, SQL_C_DOUBLE, &comprado, sizeof(comprado), NULL);
    (void) SQLFetch(stmt);
    (void) SQLCloseCursor(stmt);

    (void) SQLFreeHandle(SQL_HANDLE_STMT, stmt);

    balance = pagado - comprado;
    printf("Balance = %.2f\n", balance);
}

