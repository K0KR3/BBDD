#ifndef PRODUCT_H
#define PRODUCT_H

#include <sql.h>
#include <sqlext.h>

void product_stock(SQLHDBC dbc);
void product_find(SQLHDBC dbc);

#endif