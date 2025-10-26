#  🎯 Quiz-Master
## Data Structure-Based Quiz Management System
**QuizMaster** DS is a mini project developed in C to demonstrate the implementation and practical use of core **Data Structures**— Linked Lists, Binary Search Trees (BSTs), Stacks, and Queues.
The project applies these structures to manage quiz questions, leaderboard rankings, and quiz history efficiently.
It also integrates file handling, modular programming, and an interactive console interface to provide a complete user experience.
### 🧩 Features
- 🎲 Randomly selects questions each quiz session  
- ✅ Only accepts valid answers (A/B/C/D)  
- 💾 Stores scores locally in a file  
- 🏆 Leaderboard system for top scores  
- ⚙️ Built with core C concepts like arrays, structs, and file handling  
----
### Implemented Data Structures

| Data Structure               | Purpose                               | Key Operations                      |
| ---------------------------- | ------------------------------------- | ----------------------------------- |
| **Linked List**              | Stores quiz questions dynamically     | Insertion, traversal, randomization |
| **Binary Search Tree (BST)** | Maintains leaderboard in sorted order | Insertion, in-order traversal       |
| **Stack**                    | Records recent quiz attempts          | Push, pop, display                  |
| **Queue**                    | (Optional) Manages player turn order  | Enqueue, dequeue                    |
----
### Core Modules

1. **Question Bank Module**-
   Uses Linked List.
   Handles question loading, traversal, and randomization.
2. **Quiz Engine**-
   Randomly selects and displays questions.
   Evaluates user responses and calculates score.
3. **Leaderboard Module**-
   Uses BST for score insertion and ranking.
   Saves and loads data from file.
4. **History Module**-
   Uses Stack to record the most recent quiz attempts.
5. **User Interface**-
   Provides an interactive menu for navigation.

   <img width="800" height="250" alt="image" src="https://github.com/user-attachments/assets/ff49d63c-a307-47cc-8b1a-467c7be7a9be" />
   <img width="398" height="250" alt="image" src="https://github.com/user-attachments/assets/9094f136-dd1d-45fc-9c3b-21ab5f5b3fdf" />
   <img width="398" height="250" alt="image" src="https://github.com/user-attachments/assets/307cf82b-5c63-4f1f-95b7-8d96b8ff072f" />
   <img width="800" height="200" alt="image" src="https://github.com/user-attachments/assets/56fa8aa2-6a8d-4085-9a5d-36ea04506c88" />
----
## Screenshots

### Main Menu
<img width="370" height="400" alt="image" src="https://github.com/user-attachments/assets/63355515-ad0e-422c-a90d-54e79be77417" />

### Sample Quiz Question
<img width="370" height="400" alt="image" src="https://github.com/user-attachments/assets/fcdde783-73f8-4048-9045-9fd8c9b43b8a" />

### Leaderboard
<img width="370" height="125" alt="image" src="https://github.com/user-attachments/assets/0388370f-16c9-4271-b734-cbcdfb88d437" />
