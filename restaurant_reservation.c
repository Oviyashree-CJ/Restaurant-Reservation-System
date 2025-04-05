#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STACK_MAX 10
#define QUEUE_MAX 20
#define TOTAL_TABLES 30
#define COLUMNS 5
#define ROWS 5

typedef struct {
    int reservationID;
    char customerName[50];
    int numPeople;
} Reservation;

// Stack Implementation for Last-Minute Cancellations
int stack[STACK_MAX];
int top = -1;

void push(int table) {
    if (top < STACK_MAX - 1) {
        stack[++top] = table;
        printf("Table %d is cancelled at last minute\n", table);
    } else {
        printf("No more tables available for cancellation\n");
    }
}

int pop() {
    if (top >= 0) {
        return stack[top--];
    } else {
        printf("No last-minute cancellations available.\n");
        return -1;
    }
}

// Queue Implementation for Reservation Management
Reservation queue[QUEUE_MAX];
int front = -1, rear = -1;

void enqueue(Reservation res) {
    if (rear < QUEUE_MAX - 1) {
        if (front == -1) front = 0;
        queue[++rear] = res;
    } else {
        printf("Reservations are full. No more booking\n");
    }
}

Reservation dequeue() {
    Reservation empty = {0, "", 0};
    if (front <= rear && front != -1) {
        Reservation res = queue[front++];
        if (front > rear) front = rear = -1;
        return res;
    } else {
        printf("No active reservations.\n");
        return empty;
    }
}

// Binary Tree Implementation for Seating Arrangement
typedef struct TreeNode {
    int tableNumber;
    struct TreeNode *left;
    struct TreeNode *right;
    int available;
} TreeNode;

TreeNode* createNode(int tableNumber) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    newNode->tableNumber = tableNumber;
    newNode->left = newNode->right = NULL;
    newNode->available = 1;  // All tables start as available
    return newNode;
}

TreeNode* insertTable(TreeNode* root, int tableNumber) {
    if (!root) return createNode(tableNumber);
    if (tableNumber < root->tableNumber)
        root->left = insertTable(root->left, tableNumber);
    else
        root->right = insertTable(root->right, tableNumber);
    return root;
}

TreeNode* searchTable(TreeNode* root, int tableNumber) {
    if (!root || root->tableNumber == tableNumber)
        return root;
    if (tableNumber < root->tableNumber)
        return searchTable(root->left, tableNumber);
    return searchTable(root->right, tableNumber);
}

// Display available tables categorized as Indoor and Outdoor
void displayAvailableTables(TreeNode* root) {
    int indoorTables[20] = {0};
    int outdoorTables[10] = {0};
    int indoorIndex = 0, outdoorIndex = 0;

    for (int i = 1; i <= TOTAL_TABLES; i++) {
        TreeNode* tableNode = searchTable(root, i);
        if (tableNode && tableNode->available) {
            if (i <= 20) {
                indoorTables[indoorIndex++] = i;
            } else {
                outdoorTables[outdoorIndex++] = i;
            }
        }
    }

    printf("\n--- Indoor Tables ---\n");
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            int pos = i * COLUMNS + j;
            if (pos < 20 && indoorTables[pos] != 0) {
                printf("%4d ", indoorTables[pos]);
            } else {
                printf("     ");
            }
        }
        printf("\n");
    }

    printf("\n--- Outdoor Tables ---\n");
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            int pos = i * COLUMNS + j;
            if (pos < 10 && outdoorTables[pos] != 0) {
                printf("%4d ", outdoorTables[pos]);
            } else {
                printf("     ");
            }
        }
        printf("\n");
    }
}

// Display Reservations Queue and Last-Minute Cancellation Stack
void displayReservations() {
    printf("\n--- Reservations ---\n");
    if (front == -1) {
        printf("No active reservations\n");
    } else {
        printf("| Reservation ID | Customer Name        | Number of People | \n");
        printf("|----------------|----------------------|------------------|\n");
        for (int i = front; i <= rear; i++) {
            printf("|      %4d      | %-20s |       %4d       |\n",
                   queue[i].reservationID, queue[i].customerName, queue[i].numPeople);
        }
        printf("|----------------|----------------------|------------------|\n");
    }

    printf("\n--- Last-Minute Cancellations ---\n");
    if (top == -1) {
        printf("No last-minute cancellations available.\n");
    } else {
        printf("| Table Number |\n");
        printf("|--------------|\n");
        for (int i = top; i >= 0; i--) {
            printf("|     %4d     |\n", stack[i]);
        }
        printf("|--------------|\n");
    }
}
//Add reservation
void addReservation(TreeNode* seatingTree, int isLastMinute) {
    Reservation res;
    int tableNumber;

    if (isLastMinute) {
        // Check if there are any last-minute cancellations
        if (top == -1) {
            printf("No tables available for last-minute booking.\n");
            return;
        }

        // Assign a last-minute cancelled table
        tableNumber = pop();
        printf("Table %d is allocated for your last-minute booking.\n", tableNumber);
        printf("Enter reservation ID: ");
        scanf("%d", &res.reservationID);
        printf("Enter customer name: ");
        scanf(" %[^\n]s", res.customerName);
        printf("Enter number of people: ");
        scanf("%d", &res.numPeople);

        if (res.numPeople > 5) {
            TreeNode* nextTableNode = searchTable(seatingTree, tableNumber + 1);
            TreeNode* prevTableNode = tableNumber > 1 ? searchTable(seatingTree, tableNumber - 1) : NULL;

            if (nextTableNode && nextTableNode->available) {
                nextTableNode->available = 0;
                printf("Reservation %d added successfully for %s. Assigned Tables %d and %d.\n",
                       res.reservationID, res.customerName, tableNumber, tableNumber + 1);
            } else if (prevTableNode && prevTableNode->available) {
                prevTableNode->available = 0;
                printf("Reservation %d added successfully for %s. Assigned Tables %d and %d.\n",
                       res.reservationID, res.customerName, tableNumber - 1, tableNumber);
            } else {
                printf("No adjacent table available. Only Table %d is allocated.\n", tableNumber);
                printf("Please accommodate guests within the space of one table.\n");
            }
        } else {
            printf("Reservation %d added successfully for %s. Assigned Table %d.\n",
                   res.reservationID, res.customerName, tableNumber);
        }

        enqueue(res);

    } else {
        printf("Enter reservation ID: ");
        scanf("%d", &res.reservationID);
        printf("Enter customer name: ");
        scanf(" %[^\n]s", res.customerName);
        printf("Enter number of people: ");
        scanf("%d", &res.numPeople);
        int preference;
        printf("Reservation preference:\n1. Indoor\n2. Outdoor\nChoose preference: ");
        scanf("%d", &preference);

        while (1) {
            printf("\nAvailable tables based on preference:\n");
            displayAvailableTables(seatingTree);
            printf("Enter table number to reserve: ");
            scanf("%d", &tableNumber);

            TreeNode* tableNode = searchTable(seatingTree, tableNumber);
            TreeNode* nextTableNode = searchTable(seatingTree, tableNumber + 1);
            TreeNode* prevTableNode = tableNumber > 1 ? searchTable(seatingTree, tableNumber - 1) : NULL;

            if (tableNode && tableNode->available &&
                ((preference == 1 && tableNumber <= 20) || (preference == 2 && tableNumber > 20))) {

                if (res.numPeople > 5) {
                    if (nextTableNode && nextTableNode->available &&
                        ((preference == 1 && tableNumber + 1 <= 20) || (preference == 2 && tableNumber + 1 > 20))) {
                        tableNode->available = 0;
                        nextTableNode->available = 0;
                        enqueue(res);
                        printf("Reservation %d added successfully for %s. Assigned Tables %d and %d.\n",
                               res.reservationID, res.customerName, tableNumber, tableNumber + 1);
                        break;
                    } else if (prevTableNode && prevTableNode->available &&
                               ((preference == 1 && tableNumber - 1 <= 20) || (preference == 2 && tableNumber - 1 > 20))) {
                        tableNode->available = 0;
                        prevTableNode->available = 0;
                        enqueue(res);
                        printf("Reservation %d added successfully for %s. Assigned Tables %d and %d.\n",
                               res.reservationID, res.customerName, tableNumber - 1, tableNumber);
                        break;
                    } else {
                        printf("No adjacent table is available. Please choose another table.\n");
                    }
                } else {
                    tableNode->available = 0;
                    enqueue(res);
                    printf("Reservation %d added successfully for %s. Assigned Table %d.\n",
                           res.reservationID, res.customerName, tableNumber);
                    break;
                }
            } else {
                printf("Invalid or unavailable table. Please try again.\n");
            }
        }
    }
}


// Modify Reservation
void modifyReservation() {
    int reservationID, found = 0;
    printf("Enter reservation ID to modify: ");
    scanf("%d", &reservationID);

    for (int i = front; i <= rear; i++) {
        if (queue[i].reservationID == reservationID) {
            found = 1;
            printf("Enter customer name: ");
            scanf(" %[^\n]s", queue[i].customerName);
            printf("Enter new number of people: ");
            scanf("%d", &queue[i].numPeople);
            printf("Reservation %d updated successfully.\n", reservationID);
            break;
        }
    }

    if (!found) {
        printf("Reservation ID not found.\n");
    }
}

// Clear Screen Function
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Main Menu
int main() {
    TreeNode* seatingTree = NULL;
    int choice;

    for (int i = 1; i <= TOTAL_TABLES; i++) {
        seatingTree = insertTable(seatingTree, i);
    }

    while (1) {
        clearScreen();
        printf("\n--- Welcome to Restaurant Reservation System ---\n");
        printf("1. Add Reservation\n");
        printf("2. Add Last-Minute Booking\n");
        printf("3. Modify Reservation\n");
        printf("4. Add Last-Minute Cancellation\n");
        printf("5. Display Reservations and Cancellations\n");
        printf("6. View Available Tables\n");
        printf("7. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addReservation(seatingTree, 0);  // Normal reservation
                break;
            case 2:
                addReservation(seatingTree, 1);  // Last-minute booking
                break;
            case 3:
                modifyReservation();
                break;
            case 4:
                {
                    int tableNumber;
                    printf("Enter table number to cancel last-minute: ");
                    scanf("%d", &tableNumber);
                    push(tableNumber);
                }
                break;
            case 5:
                displayReservations();
                break;
            case 6:
                displayAvailableTables(seatingTree);
                break;
            case 7:
                printf("Exiting system.Thank you for your visit..\n");
                exit(0);
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
        printf("\nPress any key to continue...");
        getchar();  // Wait for user input before returning to menu
        getchar();  // Wait for user input
    }

    return 0;
}
