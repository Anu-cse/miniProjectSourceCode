#include <stdio.h>
#include <stdlib.h>

struct clientData {
    unsigned int acctNum;
    char lastName[15];
    char firstName[10];
    double balance;
};

int main() {

    FILE *cfPtr;

    if ((cfPtr = fopen("credit.dat", "rb+")) == NULL) {
        cfPtr = fopen("credit.dat", "wb+");
        if (cfPtr == NULL) {
            printf("File could not be opened.\n");
            return 1;
        }
    }

    int choice;

    do {
        printf("\n--- BANK SYSTEM ---\n");
        printf("1. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

    } while (choice != 1);

    fclose(cfPtr);
    return 0;
}