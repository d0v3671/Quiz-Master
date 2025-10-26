#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define TOTAL_QUESTIONS 50
#define QUIZ_QUESTIONS 10
#define MAX_PLAYERS 100
#define MAX_NAME_LENGTH 49

struct Question {
    char topic[50];
    char question[200];
    char options[4][100];
    char correct;
};

struct Player {
    char name[50];
    int score;
};

void shuffleQuestions(struct Question pool[], int total);
void clearInputBuffer();
void getPlayerName(char playerName[]);
int runRandomQuiz(struct Question pool[], int total, int numQuestions, char playerName[]);
void rules();
void saveScore(char name[], int score);
void displayLeaderboard();

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void getPlayerName(char playerName[]) {
    printf("Enter your name (max %d chars): ", MAX_NAME_LENGTH);
    if (fgets(playerName, MAX_NAME_LENGTH + 1, stdin)) {
        size_t len = strlen(playerName);

        if (len > 0 && playerName[len - 1] == '\n') {
            playerName[len - 1] = '\0';
        } else {
            clearInputBuffer();
        }

        char *p = playerName;
        while (*p != '\0') {
            if (!isspace((unsigned char)*p)) {
                break;
            }
            p++;
        }

        if (*p == '\0') {
            strcpy(playerName, "Guest");
        } else if (p != playerName) {
            memmove(playerName, p, strlen(p) + 1);
        }

    } else {
        strcpy(playerName, "Guest");
        clearInputBuffer();
    }
}

void shuffleQuestions(struct Question pool[], int total) {
    srand((unsigned int)time(NULL));

    for (int i = total - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        struct Question temp = pool[i];
        pool[i] = pool[j];
        pool[j] = temp;
    }
}

int runRandomQuiz(struct Question pool[], int total, int numQuestions, char playerName[]) {
    int score = 0;
    char ans;

    printf("\n=========================================\n");
    printf("              QUIZ STARTS              \n");
    printf("=========================================\n");
    printf("Player: %s\n", playerName);

    shuffleQuestions(pool, total);

    for (int i = 0; i < numQuestions; i++) {
        printf("\n------------------------------------------------------------------\n");
        printf("Q%d. %s\n", i + 1, pool[i].question);
        printf("Topic: %s\n", pool[i].topic);
        printf("------------------------------------------------------------------\n");

        for (int j = 0; j < 4; j++) {
            printf("    %s\n", pool[i].options[j]);
        }

        while (1) {
            printf("Enter your answer (A/B/C/D): ");
            if (scanf(" %c", &ans) != 1) {
                clearInputBuffer();
                printf("Invalid input! Please enter A, B, C, or D.\n");
                continue;
            }
            clearInputBuffer();

            ans = toupper((unsigned char)ans);

            if (ans >= 'A' && ans <= 'D') break;
            else printf("Invalid input! Please enter A, B, C, or D.\n");
        }

        if (ans == pool[i].correct) {
            printf("Correct!\n");
            score++;
        } else {
            printf("Wrong! Correct answer: %c\n", pool[i].correct);
        }
    }

    printf("\n=========================================\n");
    printf("            QUIZ COMPLETED             \n");
    printf("=========================================\n");
    printf("Your Final Score: %d / %d\n", score, numQuestions);
    printf("=========================================\n");

    return score;
}

void rules() {
    printf("\n=========================================\n");
    printf("            QUIZ RULES & INFO            \n");
    printf("=========================================\n");
    printf("1. Each question carries 1 mark.\n");
    printf("2. Enter answers as A/B/C/D (case-insensitive).\n");
    printf("3. No negative marking.\n");
    printf("4. Each quiz has %d random questions.\n", QUIZ_QUESTIONS);
    printf("=========================================\n");
}

void saveScore(char name[], int score) {
    FILE *fp = fopen("gk_scores.txt", "a");
    if (fp == NULL) {
        printf("Error: Could not open score file for saving.\n");
        return;
    }
    fprintf(fp, "%s %d\n", name, score);
    fclose(fp);
}

void displayLeaderboard() {
    FILE *fp = fopen("gk_scores.txt", "r");
    struct Player players[MAX_PLAYERS];
    int count = 0;

    if (fp == NULL) {
        printf("\nNo leaderboard data found. Play a quiz to create one!\n");
        return;
    }

    while (fscanf(fp, "%49s %d", players[count].name, &players[count].score) != EOF) {
        count++;
        if (count >= MAX_PLAYERS) break;
    }
    fclose(fp);

    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (players[j].score > players[i].score) {
                struct Player temp = players[i];
                players[i] = players[j];
                players[j] = temp;
            }
        }
    }

    printf("\n=========================================\n");
    printf("              LEADERBOARD              \n");
    printf("=========================================\n");
    printf("RANK  | PLAYER NAME             | SCORE\n");
    printf("-----------------------------------------\n");

    for (int i = 0; i < count; i++) {
        printf("%-5d | %-23.23s | %d\n", i + 1, players[i].name, players[i].score);
    }
    printf("=========================================\n");
}

int main() {
    char playerName[50];
    int choice;
    int playing = 1;

    printf("=========================================\n");
    printf("          WELCOME TO QUIZMASTER          \n");
    printf("=========================================\n");
    getPlayerName(playerName);

    printf("\nHello, %s! Ready to test your knowledge?\n", playerName);

    struct Question pool[TOTAL_QUESTIONS] = {
        {"Science", "Chemical symbol for water?", {"A. O2","B. CO2","C. H2O","D. H2"}, 'C'},
        {"Science", "Which planet is known as the Red Planet?", {"A. Mars","B. Venus","C. Jupiter","D. Mercury"}, 'A'},
        {"Science", "What gas do plants absorb from the atmosphere?", {"A. Oxygen","B. Carbon Dioxide","C. Nitrogen","D. Hydrogen"}, 'B'},
        {"Science", "What is the approximate speed of light in a vacuum?", {"A. 3x10^8 m/s","B. 3x10^6 m/s","C. 3x10^5 m/s","D. 3x10^7 m/s"}, 'A'},
        {"Science", "Who developed the Theory of Relativity?", {"A. Isaac Newton","B. Galileo Galilei","C. Albert Einstein","D. Nikola Tesla"}, 'C'},
        {"Science", "What is the hardest natural substance on Earth?", {"A. Diamond","B. Gold","C. Iron","D. Quartz"}, 'A'},
        {"Science", "Which vitamin is primarily obtained from sunlight exposure?", {"A. Vitamin A","B. Vitamin B","C. Vitamin C","D. Vitamin D"}, 'D'},
        {"Science", "How many chambers does the human heart have?", {"A. 2","B. 3","C. 4","D. 5"}, 'C'},
        {"Science", "Which organ in the human body purifies the blood?", {"A. Heart","B. Kidney","C. Liver","D. Lungs"}, 'B'},
        {"Science", "In which medium does sound travel fastest?", {"A. Air","B. Water","C. Steel (Solids)","D. Vacuum"}, 'C'},

        {"History", "Who was the first President of the USA?", {"A. George Washington","B. Abraham Lincoln","C. Thomas Jefferson","D. John Adams"}, 'A'},
        {"History", "In which year did World War II end?", {"A. 1942","B. 1945","C. 1948","D. 1950"}, 'B'},
        {"History", "Who is credited with discovering America in 1492?", {"A. Christopher Columbus","B. Vasco da Gama","C. Ferdinand Magellan","D. Marco Polo"}, 'A'},
        {"History", "The Great Wall of China was primarily built to protect against which group?", {"A. Japanese invaders","B. Mongols","C. Russians","D. Indians"}, 'B'},
        {"History", "Who is famously known as the 'Iron Man of India'?", {"A. Mahatma Gandhi","B. Jawaharlal Nehru","C. Sardar Vallabhbhai Patel","D. Subhas Chandra Bose"}, 'C'},
        {"History", "Who was the first Prime Minister of India?", {"A. Jawaharlal Nehru","B. Indira Gandhi","C. Rajendra Prasad","D. Lal Bahadur Shastri"}, 'A'},
        {"History", "In which country did the Renaissance movement begin?", {"A. France","B. Italy","C. England","D. Germany"}, 'B'},
        {"History", "Who invented the printing press in the 15th century?", {"A. Johannes Gutenberg","B. Thomas Edison","C. Alexander Graham Bell","D. Nikola Tesla"}, 'A'},
        {"History", "What was the war between the North and South states of the USA called?", {"A. World War I","B. Civil War","C. Revolutionary War","D. Vietnam War"}, 'B'},
        {"History", "Who was the first person to walk on the moon?", {"A. Yuri Gagarin","B. Neil Armstrong","C. Buzz Aldrin","D. Michael Collins"}, 'B'},

        {"Sports", "How many players are on the field for one team in a Football (Soccer) match?", {"A. 10","B. 11","C. 12","D. 9"}, 'B'},
        {"Sports", "How often are the Summer Olympic Games typically held?", {"A. Every 2 years","B. Every 3 years","C. Every 4 years","D. Every 5 years"}, 'C'},
        {"Sports", "Who won the 2018 FIFA World Cup?", {"A. Brazil","B. Germany","C. France","D. Argentina"}, 'C'},
        {"Sports", "The term 'Love' is used in which sport?", {"A. Cricket","B. Tennis","C. Football","D. Hockey"}, 'B'},
        {"Sports", "Michael Jordan is most famous for playing which sport?", {"A. Football","B. Basketball","C. Cricket","D. Baseball"}, 'B'},
        {"Sports", "How many players from one team are on the court during a standard Basketball game?", {"A. 5","B. 6","C. 7","D. 11"}, 'A'},
        {"Sports", "The Wimbledon tennis championship is hosted in which country?", {"A. USA","B. UK","C. Australia","D. France"}, 'B'},
        {"Sports", "A shuttlecock is used in which racquet sport?", {"A. Cricket","B. Badminton","C. Tennis","D. Table Tennis"}, 'B'},
        {"Sports", "Which country won the very first FIFA World Cup in 1930?", {"A. Brazil","B. Uruguay","C. Italy","D. Germany"}, 'B'},
        {"Sports", "How many interlocking rings are on the Olympic flag?", {"A. 4","B. 5","C. 6","D. 7"}, 'B'},

        {"Geography", "What is the largest ocean in the world?", {"A. Atlantic","B. Indian","C. Pacific","D. Arctic"}, 'C'},
        {"Geography", "What is the highest mountain peak above sea level?", {"A. K2","B. Kangchenjunga","C. Mount Everest","D. Makalu"}, 'C'},
        {"Geography", "What is the longest river in the world?", {"A. Nile","B. Amazon","C. Mississippi","D. Yangtze"}, 'A'},
        {"Geography", "What is the capital city of Japan?", {"A. Tokyo","B. Osaka","C. Kyoto","D. Hiroshima"}, 'A'},
        {"Geography", "What is the largest hot desert in the world?", {"A. Sahara","B. Gobi","C. Kalahari","D. Arabian"}, 'A'},
        {"Geography", "Which country is known as the 'Land of the Rising Sun'?", {"A. China","B. Japan","C. India","D. Thailand"}, 'B'},
        {"Geography", "Mount Fuji is located in which country?", {"A. China","B. India","C. Japan","D. Nepal"}, 'C'},
        {"Geography", "The Amazon rainforest is primarily located on which continent?", {"A. Africa","B. Australia","C. South America","D. Asia"}, 'C'},
        {"Geography", "What is the longest river in India?", {"A. Ganga (Ganges)","B. Brahmaputra","C. Yamuna","D. Narmada"}, 'A'},
        {"Geography", "What is the largest country in the world by total area?", {"A. USA","B. China","C. Russia","D. Canada"}, 'C'},

        {"Current Affairs", "Who is the current Secretary-General of the United Nations?", {"A. António Guterres","B. Kofi Annan","C. Ban Ki-moon","D. Paul Kagame"}, 'A'},
        {"Current Affairs", "What is the official currency of Japan?", {"A. Yen","B. Rupee","C. Dollar","D. Euro"}, 'A'},
        {"Current Affairs", "The World Economic Forum annual meeting is famously held in which Swiss town?", {"A. Paris","B. Davos","C. London","D. New York"}, 'B'},
        {"Current Affairs", "Which city is scheduled to host the 2024 Summer Olympics?", {"A. Paris","B. Los Angeles","C. Tokyo","D. Berlin"}, 'A'},
        {"Current Affairs", "Who is the current President of the United States?", {"A. Joe Biden","B. Donald Trump","C. Barack Obama","D. Bill Clinton"}, 'A'},
        {"Current Affairs", "In which month is World Health Day observed annually?", {"A. March","B. April","C. May","D. June"}, 'B'},
        {"Current Affairs", "The 2023 Global Climate Summit was known by which COP number?", {"A. COP26","B. COP27","C. COP28","D. COP25"}, 'C'},
        {"Current Affairs", "The 'Digital India' initiative was officially launched in which year?", {"A. 2009","B. 2010","C. 2015","D. 2017"}, 'C'},
        {"Current Affairs", "The BRICS group initially consisted of how many countries?", {"A. 4","B. 5","C. 6","D. 7"}, 'B'},
        {"Current Affairs", "Who was the first female Prime Minister of India?", {"A. Indira Gandhi","B. Sonia Gandhi","C. Mamata Banerjee","D. Mayawati"}, 'A'}
    };

    while (playing) {
        printf("\n=========================================\n");
        printf("               MAIN MENU               \n");
        printf("=========================================\n");
        printf("1. Start Quiz\n");
        printf("2. View Leaderboard\n");
        printf("3. Change Player Name (Current: %s)\n", playerName);
        printf("4. View Rules\n");
        printf("5. Exit QuizMaster\n");
        printf("-----------------------------------------\n");
        printf("Enter your choice (1-5): ");

        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            choice = 0;
        } else {
            clearInputBuffer();
        }

        switch (choice) {
            case 1: {
                int totalScore = runRandomQuiz(pool, TOTAL_QUESTIONS, QUIZ_QUESTIONS, playerName);
                saveScore(playerName, totalScore);
                break;
            }
            case 2: {
                displayLeaderboard();
                break;
            }
            case 3: {
                printf("\n=========================================\n");
                printf("           CHANGE PLAYER NAME            \n");
                printf("=========================================\n");
                getPlayerName(playerName);
                printf("\nPlayer name updated to: %s\n", playerName);
                break;
            }
            case 4: {
                rules();
                break;
            }
            case 5: {
                playing = 0;
                break;
            }
            default: {
                printf("\nInvalid choice. Please enter a number between 1 and 5.\n");
                break;
            }
        }
    }

    printf("\nThank you for playing QuizMaster! Goodbye!\n");
    printf("=========================================\n");
    return 0;
}
