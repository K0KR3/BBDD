#include <stdio.h>
#include <stdlib.h>
#include <sql.h>
#include <sqlext.h>

#include "products.h"
#include "odbc.h"

static int ShowProductsSubMenu();
static void ProductStock();
static void ProductFind();

/**
 * @brief Shows and processes the rhyme menu.
 *
 * This function implements a loop that prints the rhyme
 * menu, analyze the user selection and prints
 * the selected rhyme
 * @return void
 * @author rmarabini
 */
void ShowProductsMenu() {
    int nChoice = 0;
    do {
        nChoice = ShowProductsSubMenu();
        switch (nChoice) {

            case 1: {
                ProductStock();
            }
                break;

            case 2: {
                ProductFind();
            }
                break;

            case 3:  

                break;
            default:
            break;
        }
    } while (nChoice != 3);

}


/**
 * @brief prints rhyme menu and allows to select an option.
 *
 * @return selected option
 * @author rmarabini
 */

 int ShowRhymeSubMenu() {
    int nSelected = 0;
    char buf[16];

    do {

        printf(" 1. Stock\n"
               " 2. Find\n"
               " 3. Back\n\n");

        printf("Enter a number that corresponds to your choice > ");
        if (!fgets(buf, 16, stdin))
            /* reading input failed, give up: */
            nSelected =0;
        else
            /* have some input, convert it to integer: */
            nSelected = atoi(buf);
        printf("\n");


        if ((nSelected < 1) || (nSelected > 3)) {
            printf("You have entered an invalid choice. Please try again\n\n\n");
        }
    } while ((nSelected < 1) || (nSelected > 3));

    return nSelected;
}

static void ProductStock() {
    printf("Stock.\n");
}

static void ProductFind() {
    printf("Find.\n");
}

