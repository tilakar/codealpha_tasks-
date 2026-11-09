#include <stdio.h>
#include <string.h>

#define FILENAME "accounts.dat"

typedef struct {
    int accNo;
    char name[50];
    int pin;
    double balance;
} Account;

void clearInput(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

/* Finds account; returns 1 and fills *acc if found. */
int findAccount(int accNo, Account *acc) {
    FILE *fp = fopen(FILENAME, "rb");
    Account a;
    if (!fp) return 0;
    while (fread(&a, sizeof(Account), 1, fp) == 1) {
        if (a.accNo == accNo) {
            if (acc) *acc = a;
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

/* Overwrites the stored record that matches acc->accNo. */
int saveAccount(const Account *acc) {
    FILE *fp = fopen(FILENAME, "rb+");
    Account a;
    if (!fp) return 0;
    while (fread(&a, sizeof(Account), 1, fp) == 1) {
        if (a.accNo == acc->accNo) {
            fseek(fp, -(long)sizeof(Account), SEEK_CUR);
            fwrite(acc, sizeof(Account), 1, fp);
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

void createAccount(void) {
    Account a;
    FILE *fp;
    printf("Enter new account number: ");
    if (scanf("%d", &a.accNo) != 1) { clearInput(); printf("Invalid input.\n"); return; }
    if (findAccount(a.accNo, NULL)) { printf("Account already exists.\n"); return; }
    clearInput();
    printf("Enter account holder name: ");
    fgets(a.name, sizeof(a.name), stdin);
    a.name[strcspn(a.name, "\n")] = '\0';
    printf("Set 4-digit PIN: ");
    scanf("%d", &a.pin);
    printf("Enter initial deposit: ");
    scanf("%lf", &a.balance);
    if (a.balance < 0) { printf("Initial deposit cannot be negative.\n"); return; }

    fp = fopen(FILENAME, "ab");
    if (!fp) { printf("Error opening file.\n"); return; }
    fwrite(&a, sizeof(Account), 1, fp);
    fclose(fp);
    printf("Account created successfully.\n");
}

/* Asks for account number and PIN; returns 1 on success. */
int login(Account *acc) {
    int accNo, pin;
    printf("Enter account number: ");
    if (scanf("%d", &accNo) != 1) { clearInput(); printf("Invalid input.\n"); return 0; }
    if (!findAccount(accNo, acc)) { printf("Account not found.\n"); return 0; }
    printf("Enter PIN: ");
    scanf("%d", &pin);
    if (pin != acc->pin) { printf("Incorrect PIN.\n"); return 0; }
    return 1;
}

void deposit(void) {
    Account acc;
    double amt;
    if (!login(&acc)) return;
    printf("Enter amount to deposit: ");
    scanf("%lf", &amt);
    if (amt <= 0) { printf("Invalid amount.\n"); return; }
    acc.balance += amt;
    saveAccount(&acc);
    printf("Deposited %.2f. New balance: %.2f\n", amt, acc.balance);
}

void withdraw(void) {
    Account acc;
    double amt;
    if (!login(&acc)) return;
    printf("Enter amount to withdraw: ");
    scanf("%lf", &amt);
    if (amt <= 0) { printf("Invalid amount.\n"); return; }
    if (amt > acc.balance) { printf("Insufficient balance.\n"); return; }
    acc.balance -= amt;
    saveAccount(&acc);
    printf("Withdrew %.2f. New balance: %.2f\n", amt, acc.balance);
}

void balanceEnquiry(void) {
    Account acc;
    if (!login(&acc)) return;
    printf("Account Holder: %s\nAccount No: %d\nBalance: %.2f\n",
           acc.name, acc.accNo, acc.balance);
}

int main(void) {
    int choice;
    do {
        printf("\n===== BANK ACCOUNT MANAGEMENT SYSTEM =====\n");
        printf("1. Create Account\n2. Deposit\n3. Withdraw\n");
        printf("4. Balance Enquiry\n5. Exit\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) { clearInput(); choice = 0; }
        switch (choice) {
        case 1: createAccount(); break;
        case 2: deposit(); break;
        case 3: withdraw(); break;
        case 4: balanceEnquiry(); break;
        case 5: printf("Thank you for banking with us!\n"); break;
        default: printf("Invalid choice.\n");
        }
    } while (choice != 5);
    return 0;
}
