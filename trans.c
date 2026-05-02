#include <stdio.h>
#include <stdlib.h>

struct clientData {
    unsigned int acctNum;
    char lastName[15];
    char firstName[10];
    double balance;
};

void addAccount(FILE *fPtr);

int main() {

    FILE *cfPtr;

    if ((cfPtr = fopen("credit.dat", "rb+")) == NULL) {
        cfPtr = fopen("credit.dat", "wb+");
    }

    int choice;

    do {
        printf("\n--- BANK SYSTEM ---\n");
        printf("1. Add Account\n");
        printf("2. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if (choice == 1)
            addAccount(cfPtr);

    } while (choice != 2);

    fclose(cfPtr);
    return 0;
}

void addAccount(FILE *fPtr) {

    struct clientData client;

    printf("Enter Account Number: ");
    scanf("%u", &client.acctNum);

    printf("Enter Last Name: ");
    scanf("%s", client.lastName);

    printf("Enter First Name: ");
    scanf("%s", client.firstName);

    printf("Enter Balance: ");
    scanf("%lf", &client.balance);

    fseek(fPtr, (client.acctNum - 1) * sizeof(struct clientData), SEEK_SET);
    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    printf("Account added.\n");
}