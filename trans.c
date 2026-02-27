#include <stdio.h>
#include <stdlib.h>

struct clientData {
    unsigned int acctNum;
    char lastName[15];
    char firstName[10];
    double balance;
};

void addAccount(FILE *fPtr);
void displayAll(FILE *fPtr);
void searchAccount(FILE *fPtr);
void updateAccount(FILE *fPtr);
void generateTextFile(FILE *fPtr);

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
        printf("\n----- BANK MANAGEMENT SYSTEM -----\n");
        printf("1. Add Account\n");
        printf("2. Display All Accounts\n");
        printf("3. Search Account\n");
        printf("4. Update Account\n");
        printf("5. Generate accounts.txt\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addAccount(cfPtr); break;
            case 2: displayAll(cfPtr); break;
            case 3: searchAccount(cfPtr); break;
            case 4: updateAccount(cfPtr); break;
            case 5: generateTextFile(cfPtr); break;
            case 6: printf("Exiting...\n"); break;
            default: printf("Invalid choice.\n");
        }

    } while (choice != 6);

    fclose(cfPtr);
    return 0;
}

void addAccount(FILE *fPtr) {
    struct clientData client;

    printf("Enter Account Number (1-100): ");
    scanf("%u", &client.acctNum);

    printf("Enter Last Name: ");
    scanf("%s", client.lastName);

    printf("Enter First Name: ");
    scanf("%s", client.firstName);

    printf("Enter Balance: ");
    scanf("%lf", &client.balance);

    fseek(fPtr, (client.acctNum - 1) * sizeof(struct clientData), SEEK_SET);
    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    printf("Account added successfully.\n");
}

void displayAll(FILE *fPtr) {
    struct clientData client;

    rewind(fPtr);

    printf("\n%-6s %-15s %-15s %-10s\n", "Acct", "Last Name", "First Name", "Balance");

    while (fread(&client, sizeof(struct clientData), 1, fPtr)) {
        if (client.acctNum != 0) {
            printf("%-6u %-15s %-15s %-10.2f\n",
                   client.acctNum,
                   client.lastName,
                   client.firstName,
                   client.balance);
        }
    }
}

void searchAccount(FILE *fPtr) {
    struct clientData client;
    unsigned int acc;
    int found = 0;

    printf("Enter Account Number to search: ");
    scanf("%u", &acc);

    rewind(fPtr);

    while (fread(&client, sizeof(struct clientData), 1, fPtr)) {
        if (client.acctNum == acc) {
            printf("Account Found:\n");
            printf("%u %s %s %.2f\n",
                   client.acctNum,
                   client.lastName,
                   client.firstName,
                   client.balance);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Account not found.\n");
}

void updateAccount(FILE *fPtr) {
    struct clientData client;
    unsigned int acc;
    double amount;

    printf("Enter Account Number to update: ");
    scanf("%u", &acc);

    fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0) {
        printf("Account does not exist.\n");
        return;
    }

    printf("Current Balance: %.2f\n", client.balance);
    printf("Enter amount to deposit (+) or withdraw (-): ");
    scanf("%lf", &amount);

    client.balance += amount;

    fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);
    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    printf("Balance updated successfully.\n");
}

void generateTextFile(FILE *fPtr) {
    FILE *writePtr;
    struct clientData client;

    writePtr = fopen("accounts.txt", "w");
    if (writePtr == NULL) {
        printf("Cannot create text file.\n");
        return;
    }

    rewind(fPtr);

    fprintf(writePtr, "%-6s %-15s %-15s %-10s\n",
            "Acct", "Last Name", "First Name", "Balance");

    while (fread(&client, sizeof(struct clientData), 1, fPtr)) {
        if (client.acctNum != 0) {
            fprintf(writePtr, "%-6u %-15s %-15s %-10.2f\n",
                    client.acctNum,
                    client.lastName,
                    client.firstName,
                    client.balance);
        }
    }

    fclose(writePtr);
    printf("accounts.txt generated successfully.\n");
}