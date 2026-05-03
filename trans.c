#include <stdio.h>
#include <stdlib.h>

struct clientData
{
    unsigned int acctNum;
    char lastName[15];
    char firstName[10];
    double balance;
};

// function prototypes
unsigned int enterChoice(void);
void textFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void newRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);
void searchAccount(FILE *fPtr);
void sortAccounts(FILE *fPtr);

int main()
{
    FILE *cfPtr;
    unsigned int choice;

    printf("\n==== BANK MANAGEMENT SYSTEM ====\n");

    cfPtr = fopen("credit.dat", "rb+");
    if (cfPtr == NULL)
    {
        cfPtr = fopen("credit.dat", "wb+");
        if (cfPtr == NULL)
        {
            printf("File could not be opened.\n");
            return 1;
        }
    }

    while ((choice = enterChoice()) != 7)
    {
        switch (choice)
        {
        case 1: textFile(cfPtr); break;
        case 2: updateRecord(cfPtr); break;
        case 3: newRecord(cfPtr); break;
        case 4: deleteRecord(cfPtr); break;
        case 5: searchAccount(cfPtr); break;
        case 6: sortAccounts(cfPtr); break;
        default: printf("Invalid choice.\n");
        }
    }

    fclose(cfPtr);
    printf("Thank you for using the system!\n");
    return 0;
}

// ---------------- TEXT FILE ----------------
void textFile(FILE *readPtr)
{
    FILE *writePtr;
    struct clientData client;

    writePtr = fopen("accounts.txt", "w");

    if (writePtr == NULL)
    {
        printf("File could not be opened.\n");
        return;
    }

    rewind(readPtr);

    fprintf(writePtr, "%-6s %-15s %-15s %-10s\n",
            "Acct", "Last Name", "First Name", "Balance");

    while (fread(&client, sizeof(struct clientData), 1, readPtr) == 1)
    {
        if (client.acctNum != 0)
        {
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

// ---------------- UPDATE ----------------
void updateRecord(FILE *fPtr)
{
    unsigned int account;
    double transaction;
    struct clientData client;

    long recSize = (long)sizeof(struct clientData);

    printf("Enter account number to update (1-100): ");
    scanf("%u", &account);

    if (account < 1 || account > 100)
    {
        printf("Invalid account number.\n");
        return;
    }

    fseek(fPtr, (account - 1) * recSize, SEEK_SET);
    fread(&client, recSize, 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
        return;
    }

    printf("Current Balance: %.2f\n", client.balance);
    printf("Enter amount (+deposit / -withdraw): ");
    scanf("%lf", &transaction);

    if (client.balance + transaction < 0)
    {
        printf("Insufficient balance.\n");
        return;
    }

    client.balance += transaction;

    fseek(fPtr, -recSize, SEEK_CUR);   // FIXED
    fwrite(&client, recSize, 1, fPtr);

    printf("Balance updated successfully.\n");
}

// ---------------- NEW RECORD ----------------
void newRecord(FILE *fPtr)
{
    struct clientData client = {0};
    unsigned int account;

    printf("Enter new account number (1-100): ");
    scanf("%u", &account);

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum != 0)
    {
        printf("Account already exists.\n");
        return;
    }

    printf("Enter LastName FirstName Balance: ");
    scanf("%14s %9s %lf",
          client.lastName,
          client.firstName,
          &client.balance);

    client.acctNum = account;

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    printf("Account created successfully.\n");
}

// ---------------- DELETE ----------------
void deleteRecord(FILE *fPtr)
{
    struct clientData client;
    struct clientData blank = {0};
    unsigned int account;
    char confirm;

    printf("Enter account number to delete: ");
    scanf("%u", &account);

    printf("Are you sure? (y/n): ");
    scanf(" %c", &confirm);

    if (confirm != 'y')
        return;

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account does not exist.\n");
        return;
    }

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    fwrite(&blank, sizeof(struct clientData), 1, fPtr);

    printf("Account deleted successfully.\n");
}

// ---------------- SEARCH ----------------
void searchAccount(FILE *fPtr)
{
    struct clientData client;
    unsigned int acc;
    int found = 0;

    printf("Enter account number to search: ");
    scanf("%u", &acc);

    rewind(fPtr);

    while (fread(&client, sizeof(struct clientData), 1, fPtr) == 1)
    {
        if (client.acctNum == acc)
        {
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

// ---------------- SORT ----------------
void sortAccounts(FILE *fPtr)
{
    struct clientData clients[100];
    int count = 0, i, j;

    rewind(fPtr);

    while (fread(&clients[count], sizeof(struct clientData), 1, fPtr) == 1)
    {
        if (clients[count].acctNum != 0)
            count++;
    }

    for (i = 0; i < count - 1; i++)
    {
        for (j = 0; j < count - i - 1; j++)
        {
            if (clients[j].balance > clients[j + 1].balance)
            {
                struct clientData temp = clients[j];
                clients[j] = clients[j + 1];
                clients[j + 1] = temp;
            }
        }
    }

    printf("\nAccounts Sorted by Balance:\n");

    for (i = 0; i < count; i++)
    {
        printf("%u %s %s %.2f\n",
               clients[i].acctNum,
               clients[i].lastName,
               clients[i].firstName,
               clients[i].balance);
    }
}

// ---------------- MENU ----------------
unsigned int enterChoice(void)
{
    unsigned int choice;

    printf("\n------------------------------\n");
    printf("1 - Generate accounts.txt\n");
    printf("2 - Update account\n");
    printf("3 - Add new account\n");
    printf("4 - Delete account\n");
    printf("5 - Search account\n");
    printf("6 - Sort accounts by balance\n");
    printf("7 - Exit\n");
    printf("Enter choice: ");

    scanf("%u", &choice);
    return choice;
}