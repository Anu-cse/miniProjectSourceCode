// SAME HEADERS + STRUCT

void searchAccount(FILE *fPtr);

// inside main add option 3

void searchAccount(FILE *fPtr) {
    struct clientData client;
    unsigned int acc;
    int found = 0;

    printf("Enter Account Number: ");
    scanf("%u", &acc);

    rewind(fPtr);

    while (fread(&client, sizeof(client), 1, fPtr)) {
        if (client.acctNum == acc) {
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
        printf("Not found\n");
}