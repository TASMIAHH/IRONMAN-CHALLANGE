# Iron Man Challenge: The Ultimate Training

## Game Description

**Iron Man Challenge: The Ultimate Training** is a 2D endurance training game created using the **iGraphics** library in C/C++. It is inspired by the Iron Man triathlon. The player goes through four training levels: **Running, Climbing, Cycling and Swimming**. Each level has its own controls, obstacles and power-ups. The player must manage Health and Stamina in every level (and Oxygen in the swimming level) while avoiding obstacles and finishing before the time limit.

## Features
- Four levels: Running, Climbing, Cycling and Swimming, each with 4 different biomes.
- Player movement: sprinting, jumping, wall climbing, rope climbing, cycling with gear shifting, and swimming with oxygen.
- Health and Stamina bars in all levels, and an Oxygen bar in the swimming level.
- Different obstacles in each biome, spawned during gameplay with increasing difficulty.
- Power-ups: Heal, Shield, Energy, Jet Boots, Nitro, Oxygen, Speed Fins and Coins.
- Score based on distance, coins and remaining time, with Gold, Silver and Bronze medals for each level.
- Player profiles saved to a file, and a Hall of Fame leaderboard showing the top players.
- Background music for each level, collision sounds and a mute option.
- Animated intro screen, main menu, level select, controls screen, about screen and a pause option.



## Project Details
IDE: Visual studio 2010/2013

Language: C,C++.

Graphics Library: iGraphics (built on OpenGL/GLUT), stb_image.h for loading images

Audio: Windows MCI (mciSendString)

Platform : Windows PC, 1024 × 576 resolution

Genre : 2D endurance / action


## How to Run the Project

Make sure you have the following installed:
- **Visual Studio 2013**
- **MinGW Compiler** (if needed)
- **iGraphics Library** (included in this repository)


Open the project in Visual Studio 2013
- Open Visual Studio 2013.
- Go to File → Open → Project/Solution.
- Locate and select the .sln file from the cloned repository.
- Click Build → Build Solution
- Run the program by clicking Debug → Start Without Debugging


## How to Play

### **Controls**

**General / Menu Controls**

| Action | Key |
|--------|-----|
| Start game from intro screen | `Enter` / `Space` |
| Navigate menu | `Arrow Keys` or `W` / `S` |
| Select | `Enter` (mouse click also works) |
| Select discipline (intro screen) | `1` - `4` |
| Toggle sound | `M` |
| Pause | `P` |
| Back to opening screen | `Esc` |



### **Game Rules**

- Each level has a time limit. Finish the level before time runs out.
- Manage your Health and Stamina in every level, and Oxygen in the swimming level.
- Obstacles reduce your health, so avoid them or use a Shield power-up.
- Collect power-ups (Heal, Shield, Energy, Jet Boots, Nitro, Oxygen, Speed Fins) and Coins to help you and raise your score.
- Score is based on distance, coins collected and remaining time.
- Gold, Silver and Bronze medals are awarded for each level based on performance.
- Your best scores are saved to your profile and shown in the Hall of Fame.


## Project Contributors

1. Md. Mustaeen Bin Saif (ID: 00725105101001) – Level 01 (Running), intro and menu screens
2. Md. Samiul Islam (ID: 00725105101017) – Level 02 (Climbing) and Level 03 (Cycling)
3. Shanjida Imam Tasmia (ID: 00725105101024) – Level 04 (Swimming), audio, profiles and UI


## Screenshots

### **Intro Screen**
<img src="screenshots/INTRO.png" width="400">

### **Main Menu**
<img src="screenshots/MAIN_MENU.png" width="400">

### **Level 1 – Running**
<img src="screenshots/RUNNING.png" width="400">

### **Level 2 – Climbing**
<img src="screenshots/CLIMBING.png" width="400">

### **Level 3 – Cycling**
<img src="screenshots/CYCLING.png" width="400">

### **Level 4 – Swimming**
<img src="screenshots/SWIMMING.png" width="400">

## Youtube Link
[CSE 1200 Project: Iron Man Challenge: The Ultimate Training](https://www.youtube.com/watch?v=ZYm-enAq7Mc&t=6s)

## Project Report
[Project Report: Iron Man Challenge: The Ultimate Training](https://drive.google.com/drive/folders/17fkcilACdcuQGpTwJsNqSy_6ezoFtkc7)
