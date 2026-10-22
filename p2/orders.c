
/**
 * @brief Shows and processes the fairy menu.
 *
 * This function implements a loop that prints the fairy
 * menu, analyze the user selection and prints
 * the selected fairy tail
 *
 * @return selected option
 * @author rmarabini
 */

void ShowFairyMenu() {
    int nChoice = 0;

    do {
        nChoice = ShowFairySubMenu();
        switch (nChoice) {
            case 1: {
                PrintZub();
            }
                break;

            case 2: {
                PrintBigFatGit();
            }
                break;

            case 3: {
                PrintAliens();
            }
                break;

            case 4: {
                printf("Bye Bye\n\n");
            }
                break;
        }
    } while (nChoice != 4);

}

/**
 * @brief prints the fairy menu and allows to select an option.
 *
 * @return selected option
 * @author rmarabini
 */
 int ShowFairySubMenu() {
    int nSelected = 0;
    char buf[16];

    do {

        printf(" (1) The zub zubs\n"
               " (2) The BFG\n"
               " (3) PrintAliens\n"
               " (4) Quit\n\n");

        printf("Enter a number that corresponds to your choice > ");
        if (!fgets(buf, 16, stdin))
            /* reading input failed, give up: */
            nSelected =0;
        else
            /* have some input, convert it to integer: */
            nSelected = atoi(buf);
        printf("\n");

        if ((nSelected < 1) || (nSelected > 4)) {
            printf("You have entered an invalid choice. Please try again\n\n\n");
        }
    } while ((nSelected < 1) || (nSelected > 4));

    return nSelected;
}
