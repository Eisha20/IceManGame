#include "StudentWorld.h"
#include <string>
using namespace std;

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

GameWorld* createStudentWorld(string assetDir) {
    return new StudentWorld(assetDir);
}

StudentWorld::StudentWorld(std::string assetDir)
    : GameWorld(assetDir), iceMan(nullptr) {
}

int StudentWorld::init() {
    makeIceMan();
    makeIceCubes();

    // According to the manual (P. 16), keep remainder of game objects (Protestors, Gold Nuggets, Oil,
    // Etc.) within a single STL collection such as a list or vector.
    //
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move() {
    // This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
   
    makeStatString(); //set the status string 
     
    if (iceMan->getState()) {//if iceMan is alive then call do something 
        iceMan->doSomething();
    }


    decLives();
    return GWSTATUS_PLAYER_DIED;
}


void StudentWorld::cleanUp() {
    //delete iceMan
    delete iceMan;
    iceMan = nullptr;
     //delete all ice objects
    for (int row = 0; row < (VIEW_HEIGHT - 4); row++) {
        for (int col = 0; col < VIEW_WIDTH; col++) {
            delete iceCube[row][col];
            iceCube[row][col] = nullptr;
        }
    }

    //delete all goodies
    vector<Actor*>::iterator temp;
    for (auto p = goodies.begin(); p != end(goodies); p++) {
        delete (*p);
        temp = goodies.erase(p);
        --temp;
        p = temp;
    }

}

void StudentWorld::makeIceMan() { // Creates Ice Man.
    iceMan = new IceMan(this);
}

void StudentWorld::makeIceCubes() { // Creates Ice Field.

    //for (int column = 0; column < (VIEW_WIDTH - 4); ++column) {
    //    if (column == 30)
    //        column += 4; // Skip middle four column as initial hole.
    //    for (int row = 4; row < (VIEW_HEIGHT - 4); ++row) {
    //        iceCube[row][column] = new Ice(column, row, this);
    //    }
    //}
    for (int column = 0; column < VIEW_WIDTH; ++column) {
        if (column == 30) {
            for (int x = 0; x < 4; x++) {
                for (int row = 0; row < 4; ++row) {
                    iceCube[row][column] = new Ice(column, row, this);
                }

                column ++; // Skip middle four column as initial hole.
            }
        }
        for (int row = 0; row < (VIEW_HEIGHT - 4); ++row) {
            iceCube[row][column] = new Ice(column, row, this);
        }
    }
    /*
    I totally forgot to tell you but I made some changes to this function too.
        - I removed - 4 from the for loop (line 71) because I believe the instructions 
          said that the ice should fill the entire width 
        - I added 4*4 ice to the end of the empty tunnel. I didn't
          come across anyting in the instructions that said that, however, the game does have it 
          in there. I'll leave it up to you to decide if we should keep it or not.        
    */
}

void StudentWorld::makeGoodies() {
    
}

void StudentWorld::makeStatString() {
    //format: Lvl: _ Lives: _ Hlth: _ Wtr: _ Gld: _ Oil Left: _ Sonar: _ Scr: _ 
    /*Does not print hlth, wtr, gld, oil left, sonar*/
    int level = getLevel(); 
    int lives = getLives();
    int score = getScore();
    int health = iceMan->getHealth();
    int water = iceMan->getWater();
    int gold = iceMan->getGoldNuggets();
    int Sonar = iceMan->getSonar();
    //int oilLeft! maybe make a variable in student world that holds this value

    string gameStat = "Lvl: ";
    
    string levelStr = to_string(level);
    gameStat += levelStr;
     
    gameStat += " Lives: ";
    string livesStr = to_string(lives);
    gameStat += livesStr;

    //Do not have anything that keeps track of health yet
    gameStat += " Hlth: ";

    //get, convert and add score to string 
    gameStat += " Scr: ";
    string scoreStr = to_string(score);
    gameStat += score;
   
    setGameStatText(gameStat); 
}


bool StudentWorld::isIcePresent(int x, int y) {
    if ((*iceCube[x][y]).getState())
        return true;
    else
        return false;
}

void StudentWorld::destroyIce(int x, int y) {
    delete iceCube[x][y];
    iceCube[x][y] = nullptr;
}