#  🎯 Quiz-Master
## Data Structure-Based Quiz Management System
**QuizMaster** DS is a mini project developed in C to demonstrate the implementation and practical use of core **Data Structures**— Linked Lists, Binary Search Trees (BSTs), Stacks, and Queues.
The project applies these structures to manage quiz questions, leaderboard rankings, and quiz history efficiently.
It also integrates file handling, modular programming, and an interactive console interface to provide a complete user experience.
### 🧩 Features
- 🎲 Randomly selects 5 questions each quiz session  
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
<img width="1090" height="500" alt="Screenshot 2025-10-26 165547" src="https://github.com/user-attachments/assets/b7f92c22-57a4-4978-b802-4129f2ee8918" />

### Sample Quiz Question
<img width="1090" height="500" alt="Screenshot 2025-10-26 165624" src="https://github.com/user-attachments/assets/ece07346-80b6-4e64-b41c-da7ed7185d07" />

### Leaderboard
<img width="1090" height="165" alt="image" src="https://github.com/user-attachments/assets/790e7e00-97c5-4f39-9c05-56f8e95107ee" />
