# Tetris Game in C++ with OpenGL

🎮 A clean and functional clone of the classic Tetris game, built in **C++** using **OpenGL** and **GLUT**.

---

## 🚀 How to Run

### ✅ Requirements

* macOS (or Linux) with OpenGL & GLUT support
* C++ compiler (g++ with C++11 support)

### ✅ Compile & Run

#### On macOS:

```bash
g++ -std=c++11 ~/Desktop/tetris.cpp -framework OpenGL -framework GLUT -o tetris
./tetris
```

#### On Linux:

```bash
g++ -std=c++11 tetris.cpp -lGL -lglut -o tetris
./tetris
```

---

## 🎮 Controls

| Key               | Action                   |
| ----------------- | ------------------------ |
| `←` (Left Arrow)  | Move piece left          |
| `→` (Right Arrow) | Move piece right         |
| `↓` (Down Arrow)  | Drop piece faster        |
| `↑` (Up Arrow)    | Rotate piece (clockwise) |

---

## 🧠 Features

* ✅ Full grid-based rendering using OpenGL
* ✅ 7 classic tetromino shapes with full rotation logic
* ✅ Real-time piece movement & collision detection
* ✅ Line clearing and row shifting
* ✅ Score tracking (100 points per cleared line)
* ✅ Fair piece randomization using the 7-bag system
* ✅ Game Over detection and automatic restart

---

## 🧩 Code Highlights

* **OpenGL** rendering using `GL_POLYGON`
* **Tetriminos** stored as shape/rotation point vectors
* **Grid** as a 2D `Color` struct array (with RGB + filled flag)
* **Game loop** handled using `glutTimerFunc()`
* **Game Over** triggers reset of board and score

---

## 🛠️ Project Structure

```
tetris/
├── tetris.cpp       // Main source code
├── README.md        // This file
```

---

## 🧑‍💻 Skills Demonstrated

* C++11: structs, vectors, deques, RNG
* OpenGL: real-time 2D graphics
* Game logic: rotation, falling, collision
* Input handling via GLUT
* Clean modular design & logic

---

## 📸 Screenshot


<img width="447" height="550" alt="Screenshot 2025-07-23 at 12 03 39 AM" src="https://github.com/user-attachments/assets/aaa2aa61-d2fd-424e-9dae-c7ef3d1fc718" />


---

## 🔮 Possible Improvements

* Game Over screen message
* Pause/Resume toggle
* Preview next piece
* Increasing speed with score
* High score persistence
* Sound effects

