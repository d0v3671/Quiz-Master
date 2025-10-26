#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define QUIZ_BANK_SIZE 10
#define QUIZ_PER_ROUND 5
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
    char correct; // 'A'/'B'/'C'/'D'
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

//-----------------------------------------------
// FUNCTION DECLARATIONS
//-----------------------------------------------

void clearInputBuffer();
void getPlayerName(char name[]);
void rules();

struct QuestionNode* createQuestionNode(struct Question q);
struct QuestionNode* insertQuestion(struct QuestionNode *head,
                                    const char *topic,
                                    const char *question,
                                    const char *optA,
                                    const char *optB,
                                    const char *optC,
                                    const char *optD,
                                    char correct);
struct QuestionNode* loadQuestions();
int countQuestions(struct QuestionNode *head);
struct QuestionNode* shuffleQuestions(struct QuestionNode *head);
int runQuiz(struct QuestionNode *head, char playerName[], struct Stack *quizHistory);

// BST (Leaderboard)
struct PlayerNode* insertPlayer(struct PlayerNode *root, struct Player p);
void inorderLeaderboard(struct PlayerNode *root, int *rank);
void saveLeaderboardToFile(struct PlayerNode *root, FILE *fp);
struct PlayerNode* loadLeaderboard();
void displayLeaderboard(struct PlayerNode *root);

// Stack (Quiz History)
void pushAttempt(struct Stack *s, struct Attempt a);
void displayAttempts(struct Stack *s);

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
    printf("4. Each quiz will ask %d random questions from a bank of %d.\n", QUIZ_PER_ROUND, QUIZ_BANK_SIZE);
    printf("5. Your final score will be displayed at the end.\n");
    printf("=========================================\n\n");
}

//-----------------------------------------------
// LINKED LIST: QUESTION BANK
//-----------------------------------------------

struct QuestionNode* createQuestionNode(struct Question q) {
    struct QuestionNode *newNode = malloc(sizeof(struct QuestionNode));
    if (!newNode) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    newNode->data = q;
    newNode->next = NULL;
    return newNode;
}

struct QuestionNode* insertQuestion(struct QuestionNode *head,
                                    const char *topic,
                                    const char *question,
                                    const char *optA,
                                    const char *optB,
                                    const char *optC,
                                    const char *optD,
                                    char correct) {
    struct Question q;
    strncpy(q.topic, topic, MAX_TOPIC_LENGTH - 1); q.topic[MAX_TOPIC_LENGTH - 1] = '\0';
    strncpy(q.question, question, MAX_QUESTION_LENGTH - 1); q.question[MAX_QUESTION_LENGTH - 1] = '\0';
    strncpy(q.options[0], optA, MAX_OPTION_LENGTH - 1); q.options[0][MAX_OPTION_LENGTH - 1] = '\0';
    strncpy(q.options[1], optB, MAX_OPTION_LENGTH - 1); q.options[1][MAX_OPTION_LENGTH - 1] = '\0';
    strncpy(q.options[2], optC, MAX_OPTION_LENGTH - 1); q.options[2][MAX_OPTION_LENGTH - 1] = '\0';
    strncpy(q.options[3], optD, MAX_OPTION_LENGTH - 1); q.options[3][MAX_OPTION_LENGTH - 1] = '\0';
    q.correct = toupper((unsigned char)correct);

    struct QuestionNode *node = createQuestionNode(q);
    if (!head) return node;

    struct QuestionNode *temp = head;
    while (temp->next) temp = temp->next;
    temp->next = node;
    return head;
}

struct QuestionNode* loadQuestions() {
    struct QuestionNode *head = NULL;

    head = insertQuestion(head, "Geography", "What is the capital of France?",
                          "A. Paris", "B. London", "C. Rome", "D. Berlin", 'A');

    head = insertQuestion(head, "Data Structures", "Which data structure uses LIFO?",
                          "A. Queue", "B. Stack", "C. Tree", "D. Array", 'B');

    head = insertQuestion(head, "Algorithms", "Which of these is a binary tree traversal?",
                          "A. Inorder", "B. Circular", "C. Random", "D. Linear", 'A');

    head = insertQuestion(head, "C Language", "Which symbol is used to start a single-line comment in C?",
                          "A. //", "B. /* */", "C. #", "D. --", 'A');

    head = insertQuestion(head, "C Library", "Which header file declares printf()?",
                          "A. stdio.h", "B. conio.h", "C. stdlib.h", "D. string.h", 'A');

    head = insertQuestion(head, "Algorithms", "Which sorting algorithm is generally fastest on average?",
                          "A. Bubble", "B. Quick", "C. Selection", "D. Insertion", 'B');

    head = insertQuestion(head, "Graphs", "Which data structure is typically used in BFS?",
                          "A. Stack", "B. Queue", "C. Tree", "D. Graph", 'B');

    head = insertQuestion(head, "Complexity", "What is the time complexity of binary search (sorted array)?",
                          "A. O(n)", "B. O(n^2)", "C. O(log n)", "D. O(1)", 'C');

    head = insertQuestion(head, "C Memory", "Which function allocates memory dynamically in C?",
                          "A. malloc", "B. alloc", "C. memalloc", "D. new", 'A');

    head = insertQuestion(head, "Data Structures", "Which data structure uses FIFO?",
                          "A. Queue", "B. Stack", "C. BST", "D. Array", 'A');

    return head;
}

int countQuestions(struct QuestionNode *head) {
    int count = 0;
    while (head) {
        count++;
        head = head->next;
    }
    return count;
}

/* optional shuffle - not required since we pick random unique indices;
   kept for compatibility if you want to shuffle node data instead */
struct QuestionNode* shuffleQuestions(struct QuestionNode *head) {
    int n = countQuestions(head);
    if (n <= 1) return head;

    struct QuestionNode *arr[QUIZ_BANK_SIZE];
    struct QuestionNode *tmp = head;
    int i = 0;
    while (tmp && i < QUIZ_BANK_SIZE) { arr[i++] = tmp; tmp = tmp->next; }

    srand((unsigned int)time(NULL));
    for (int a = n - 1; a > 0; a--) {
        int b = rand() % (a + 1);
        struct Question tmpq = arr[a]->data;
        arr[a]->data = arr[b]->data;
        arr[b]->data = tmpq;
    }
    return head;
}

//-----------------------------------------------
// STACK: QUIZ ATTEMPTS
//-----------------------------------------------

void pushAttempt(struct Stack *s, struct Attempt a) {
    if (s->top == 9) {
        /* shift left to make room (drop oldest) */
        for (int i = 1; i < 10; i++) s->attempts[i - 1] = s->attempts[i];
        s->top = 8;
    }
    s->attempts[++s->top] = a;
}

void displayAttempts(struct Stack *s) {
    if (s->top == -1) {
        printf("\nNo recent attempts found.\n");
        return;
    }
    printf("\n-----------------------------------------\n");
    printf("         RECENT QUIZ ATTEMPTS\n");
    printf("-----------------------------------------\n");
    for (int i = s->top; i >= 0; i--) {
        printf("%s - Score: %d - Time: %s\n",
               s->attempts[i].playerName,
               s->attempts[i].score,
               s->attempts[i].timeString);
    }
    printf("-----------------------------------------\n");
}

//-----------------------------------------------
// BINARY SEARCH TREE: LEADERBOARD
//-----------------------------------------------

struct PlayerNode* insertPlayer(struct PlayerNode *root, struct Player p) {
    if (root == NULL) {
        struct PlayerNode *node = malloc(sizeof(struct PlayerNode));
        if (!node) { perror("malloc"); exit(EXIT_FAILURE); }
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
    inorderLeaderboard(root->right, rank); // descending
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
    printf("               LEADERBOARD               \n");
    printf("=========================================\n");
    printf("RANK  | PLAYER NAME          | SCORE\n");
    printf("-----------------------------------------\n");
    inorderLeaderboard(root, &rank);
    printf("=========================================\n");
}

//-----------------------------------------------
// QUIZ FUNCTION (selects QUIZ_PER_ROUND unique random questions)
//-----------------------------------------------

int runQuiz(struct QuestionNode *head, char playerName[], struct Stack *quizHistory) {
    if (!head) {
        printf("No questions available.\n");
        return 0;
    }

    int totalQuestions = countQuestions(head);
    int numQuestions = QUIZ_PER_ROUND;
    if (totalQuestions < numQuestions) numQuestions = totalQuestions;

    int *selected = calloc(totalQuestions, sizeof(int));
    if (!selected) {
        perror("calloc");
        return 0;
    }

    int score = 0;
    srand((unsigned int)time(NULL));

    printf("\n=========================================\n");
    printf("               QUIZ STARTS               \n");
    printf("=========================================\n");

    for (int i = 0; i < numQuestions; i++) {
        int qIndex;
        do {
            qIndex = rand() % totalQuestions;
        } while (selected[qIndex]);
        selected[qIndex] = 1;

        struct QuestionNode *temp = head;
        for (int j = 0; j < qIndex; j++) temp = temp->next;

        printf("\nQ%d. %s\n", i + 1, temp->data.question);
        for (int k = 0; k < 4; k++) {
            printf("%s\n", temp->data.options[k]);
        }

        char ans = '\0';
        while (1) {
            printf("Enter your answer (A/B/C/D): ");
            if (scanf(" %c", &ans) != 1) {
                clearInputBuffer();
                continue;
            }
            clearInputBuffer();
            ans = toupper((unsigned char)ans);
            if (ans >= 'A' && ans <= 'D') break;
            printf("Invalid input. Try again.\n");
        }

        if (ans == temp->data.correct) {
            printf("Correct!\n");
            score++;
        } else {
            printf("Wrong! Correct answer: %c\n", temp->data.correct);
        }
    }

    printf("\n=========================================\n");
    printf("            QUIZ COMPLETED               \n");
    printf("=========================================\n");
    printf("Your Score: %d / %d\n", score, numQuestions);
    printf("=========================================\n");

    /* push attempt to history stack */
    struct Attempt a;
    strncpy(a.playerName, playerName, MAX_NAME_LENGTH - 1); a.playerName[MAX_NAME_LENGTH - 1] = '\0';
    a.score = score;
    time_t now = time(NULL);
    strncpy(a.timeString, ctime(&now), sizeof(a.timeString) - 1);
    a.timeString[sizeof(a.timeString) - 1] = '\0';
    /* remove trailing newline from ctime */
    size_t tlen = strlen(a.timeString);
    if (tlen > 0 && a.timeString[tlen - 1] == '\n') a.timeString[tlen - 1] = '\0';

    pushAttempt(quizHistory, a);

    free(selected);
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
    printf("\nWelcome, %s!\n", playerName);
    rules(); // Display once after name input

    while (running) {
        printf("\n=========================================\n");
        printf("                 MAIN MENU               \n");
        printf("=========================================\n");
        printf("1. Start Quiz\n");
        printf("2. View Leaderboard\n");
        printf("3. View Quiz History\n");
        printf("4. Change Player Name\n");
        printf("5. Exit\n");
        printf("=========================================\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            printf("Invalid input. Please enter a number 1-5.\n");
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            case 1: {
                int score = runQuiz(questions, playerName, &quizHistory);
                struct Player p;
                strncpy(p.name, playerName, MAX_NAME_LENGTH - 1); p.name[MAX_NAME_LENGTH - 1] = '\0';
                p.score = score;
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
                displayAttempts(&quizHistory);
                break;
            case 4:
                getPlayerName(playerName);
                printf("\nWelcome, %s!\n", playerName);
                rules(); // show rules again after name change
                break;
            case 5:
                running = 0;
                break;
            default:
                printf("Invalid choice.\n");
        }
    }

    printf("\nThank you for playing QuizMaster DS!\n");
    return 0;
}
