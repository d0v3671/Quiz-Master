#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define QUIZ_QUESTIONS 5
#define MAX_NAME_LENGTH 49
#define MAX_TOPIC_LENGTH 50
#define MAX_QUESTION_LENGTH 200
#define MAX_OPTION_LENGTH 100

//-----------------------------------------------
// STRUCTURE DEFINITIONS
//-----------------------------------------------

struct Question {
    char topic[MAX_TOPIC_LENGTH];
    char question[MAX_QUESTION_LENGTH];
    char options[4][MAX_OPTION_LENGTH];
    char correct;
};

struct QuestionNode {
    struct Question data;
    struct QuestionNode *next;
};

struct Player {
    char name[MAX_NAME_LENGTH];
    int score;
};

struct PlayerNode {
    struct Player data;
    struct PlayerNode *left;
    struct PlayerNode *right;
};

struct Attempt {
    char playerName[MAX_NAME_LENGTH];
    int score;
    char timeString[50];
};

struct Stack {
    struct Attempt attempts[10];
    int top;
};

struct PlayerQueue {
    char names[10][MAX_NAME_LENGTH];
    int front, rear;
};

//-----------------------------------------------
// FUNCTION DECLARATIONS
//-----------------------------------------------

void clearInputBuffer();
void getPlayerName(char name[]);
void rules();

// Linked List (Question Bank)
struct QuestionNode* loadQuestions();
void displayQuestions(struct QuestionNode *head);
struct QuestionNode* shuffleQuestions(struct QuestionNode *head);
int runQuiz(struct QuestionNode *head, char playerName[], struct Stack *s);

// BST (Leaderboard)
struct PlayerNode* insertPlayer(struct PlayerNode *root, struct Player p);
void inorderLeaderboard(struct PlayerNode *root, int *rank);
void saveLeaderboardToFile(struct PlayerNode *root, FILE *fp);
struct PlayerNode* loadLeaderboard();
void displayLeaderboard(struct PlayerNode *root);

// Stack (Quiz History)
void pushAttempt(struct Stack *s, struct Attempt a);
void displayAttempts(struct Stack *s);

// Queue (Optional Multiplayer)
void enqueue(struct PlayerQueue *q, char name[]);
char* dequeue(struct PlayerQueue *q);

//-----------------------------------------------
// UTILITY FUNCTIONS
//-----------------------------------------------

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void getPlayerName(char name[]) {
    printf("Enter your name: ");
    if (fgets(name, MAX_NAME_LENGTH, stdin)) {
        size_t len = strlen(name);
        if (len > 0 && name[len - 1] == '\n')
            name[len - 1] = '\0';
    }
    if (strlen(name) == 0)
        strcpy(name, "Guest");
}

void rules() {
    printf("\n=========================================\n");
    printf("             QUIZ RULES & INFO           \n");
    printf("=========================================\n");
    printf("1. Each question carries 1 mark.\n");
    printf("2. Enter answers as A/B/C/D (case-insensitive).\n");
    printf("3. No negative marking.\n");
    printf("4. Each quiz has %d random questions.\n", QUIZ_QUESTIONS);
    printf("=========================================\n\n");
}

//-----------------------------------------------
// LINKED LIST: QUESTION BANK
//-----------------------------------------------

struct QuestionNode* createQuestionNode(struct Question q) {
    struct QuestionNode *newNode = malloc(sizeof(struct QuestionNode));
    newNode->data = q;
    newNode->next = NULL;
    return newNode;
}

struct QuestionNode* loadQuestions() {
    struct QuestionNode *head = NULL, *tail = NULL;

    struct Question q[] = {
        {"Science", "Chemical symbol for water?", {"A. O2", "B. CO2", "C. H2O", "D. H2"}, 'C'},
        {"Science", "Which planet is known as the Red Planet?", {"A. Mars", "B. Venus", "C. Jupiter", "D. Mercury"}, 'A'},
        {"History", "Who was the first President of the USA?", {"A. George Washington", "B. Abraham Lincoln", "C. Jefferson", "D. Adams"}, 'A'},
        {"Sports", "The term 'Love' is used in which sport?", {"A. Cricket", "B. Tennis", "C. Football", "D. Hockey"}, 'B'},
        {"Geography", "What is the largest ocean?", {"A. Atlantic", "B. Indian", "C. Pacific", "D. Arctic"}, 'C'},
        {"Current Affairs", "Current UN Secretary-General?", {"A. António Guterres", "B. Kofi Annan", "C. Ban Ki-moon", "D. Paul Kagame"}, 'A'},
        {"Science", "Who developed the Theory of Relativity?", {"A. Newton", "B. Einstein", "C. Tesla", "D. Galileo"}, 'B'},
        {"Geography", "Capital of Japan?", {"A. Osaka", "B. Kyoto", "C. Tokyo", "D. Hiroshima"}, 'C'}
    };

    int total = sizeof(q) / sizeof(q[0]);

    for (int i = 0; i < total; i++) {
        struct QuestionNode *node = createQuestionNode(q[i]);
        if (head == NULL) head = tail = node;
        else {
            tail->next = node;
            tail = node;
        }
    }

    return head;
}

// Shuffle linked list questions
struct QuestionNode* shuffleQuestions(struct QuestionNode *head) {
    struct QuestionNode *array[100];
    int n = 0;

    struct QuestionNode *temp = head;
    while (temp) {
        array[n++] = temp;
        temp = temp->next;
    }

    srand((unsigned int)time(NULL));
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        struct Question tempData = array[i]->data;
        array[i]->data = array[j]->data;
        array[j]->data = tempData;
    }

    return head;
}

void displayQuestions(struct QuestionNode *head) {
    int i = 1;
    while (head) {
        printf("%d. %s\n", i++, head->data.question);
        head = head->next;
    }
}

//-----------------------------------------------
// STACK: QUIZ ATTEMPTS
//-----------------------------------------------

void pushAttempt(struct Stack *s, struct Attempt a) {
    if (s->top == 9) {
        for (int i = 1; i < 10; i++) s->attempts[i - 1] = s->attempts[i];
        s->top = 8;
    }
    s->attempts[++s->top] = a;
}

void displayAttempts(struct Stack *s) {
    if (s->top == -1) {
        printf("No recent attempts.\n");
        return;
    }
    printf("\nRecent Quiz Attempts:\n");
    printf("-----------------------------------------\n");
    for (int i = s->top; i >= 0; i--) {
        printf("%s - Score: %d - Time: %s\n", s->attempts[i].playerName, s->attempts[i].score, s->attempts[i].timeString);
    }
    printf("-----------------------------------------\n");
}

//-----------------------------------------------
// BINARY SEARCH TREE: LEADERBOARD
//-----------------------------------------------

struct PlayerNode* insertPlayer(struct PlayerNode *root, struct Player p) {
    if (root == NULL) {
        struct PlayerNode *node = malloc(sizeof(struct PlayerNode));
        node->data = p;
        node->left = node->right = NULL;
        return node;
    }

    if (p.score < root->data.score)
        root->left = insertPlayer(root->left, p);
    else
        root->right = insertPlayer(root->right, p);

    return root;
}

void inorderLeaderboard(struct PlayerNode *root, int *rank) {
    if (!root) return;
    inorderLeaderboard(root->right, rank);
    printf("%-5d | %-20s | %d\n", (*rank)++, root->data.name, root->data.score);
    inorderLeaderboard(root->left, rank);
}

void saveLeaderboardToFile(struct PlayerNode *root, FILE *fp) {
    if (!root) return;
    saveLeaderboardToFile(root->left, fp);
    fprintf(fp, "%s %d\n", root->data.name, root->data.score);
    saveLeaderboardToFile(root->right, fp);
}

struct PlayerNode* loadLeaderboard() {
    FILE *fp = fopen("leaderboard.txt", "r");
    if (!fp) return NULL;

    struct PlayerNode *root = NULL;
    struct Player p;
    while (fscanf(fp, "%49s %d", p.name, &p.score) == 2)
        root = insertPlayer(root, p);
    fclose(fp);
    return root;
}

void displayLeaderboard(struct PlayerNode *root) {
    if (!root) {
        printf("\nNo leaderboard data found.\n");
        return;
    }
    int rank = 1;
    printf("\n=========================================\n");
    printf("              LEADERBOARD              \n");
    printf("=========================================\n");
    printf("RANK  | PLAYER NAME          | SCORE\n");
    printf("-----------------------------------------\n");
    inorderLeaderboard(root, &rank);
    printf("=========================================\n");
}

//-----------------------------------------------
// QUIZ FUNCTION
//-----------------------------------------------

int runQuiz(struct QuestionNode *head, char playerName[], struct Stack *s) {
    head = shuffleQuestions(head);
    int score = 0;
    struct QuestionNode *temp = head;
    char ans;

    printf("\n=========================================\n");
    printf("              QUIZ STARTS              \n");
    printf("=========================================\n");

    for (int i = 0; i < QUIZ_QUESTIONS && temp; i++, temp = temp->next) {
        printf("\nQ%d. %s\n", i + 1, temp->data.question);
        for (int j = 0; j < 4; j++)
            printf("%s\n", temp->data.options[j]);

        while (1) {
            printf("Enter your answer (A/B/C/D): ");
            if (scanf(" %c", &ans) != 1) {
                clearInputBuffer();
                continue;
            }
            clearInputBuffer();
            ans = toupper(ans);
            if (ans >= 'A' && ans <= 'D') break;
            else printf("Invalid input. Try again.\n");
        }

        if (ans == temp->data.correct) {
            printf("Correct!\n");
            score++;
        } else {
            printf("Wrong! Correct answer: %c\n", temp->data.correct);
        }
    }

    printf("\nQuiz Completed! Score: %d/%d\n", score, QUIZ_QUESTIONS);

    struct Attempt a;
    strcpy(a.playerName, playerName);
    a.score = score;
    time_t now = time(NULL);
    strcpy(a.timeString, ctime(&now));
    a.timeString[strlen(a.timeString) - 1] = '\0';
    pushAttempt(s, a);

    return score;
}

//-----------------------------------------------
// MAIN FUNCTION
//-----------------------------------------------

int main() {
    char playerName[MAX_NAME_LENGTH];
    int choice;
    int running = 1;

    struct QuestionNode *questions = loadQuestions();
    struct PlayerNode *leaderboard = loadLeaderboard();
    struct Stack quizHistory = {.top = -1};

    printf("=========================================\n");
    printf("          WELCOME TO QUIZMASTER DS       \n");
    printf("=========================================\n");
    getPlayerName(playerName);

    while (running) {
        printf("\n=========================================\n");
        printf("                 MAIN MENU               \n");
        printf("=========================================\n");
        printf("1. Start Quiz\n");
        printf("2. View Leaderboard\n");
        printf("3. View Rules\n");
        printf("4. View Quiz History\n");
        printf("5. Change Player Name\n");
        printf("6. Exit\n");
        printf("=========================================\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            case 1: {
                int score = runQuiz(questions, playerName, &quizHistory);
                struct Player p = {.score = score};
                strcpy(p.name, playerName);
                leaderboard = insertPlayer(leaderboard, p);

                FILE *fp = fopen("leaderboard.txt", "w");
                if (fp) {
                    saveLeaderboardToFile(leaderboard, fp);
                    fclose(fp);
                }
                break;
            }
            case 2:
                displayLeaderboard(leaderboard);
                break;
            case 3:
                rules();
                break;
            case 4:
                displayAttempts(&quizHistory);
                break;
            case 5:
                getPlayerName(playerName);
                break;
            case 6:
                running = 0;
                break;
            default:
                printf("Invalid choice.\n");
        }
    }

    printf("\nThank you for playing QuizMaster DS!\n");
    return 0;
}
