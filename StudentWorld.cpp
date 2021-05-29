#include "StudentWorld.h"
#include <string>
using namespace std;



GameWorld* createStudentWorld(string assetDir) {
    return new StudentWorld(assetDir);
}

StudentWorld::StudentWorld(std::string assetDir)
    : GameWorld(assetDir), iceMan(nullptr) {
    _ticksSinceLastProtester = 0;
}

int StudentWorld::init() {
    makeIceMan();
    makeIceCubes();
    makeGoodies();
    _numProtesters = 0;


    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move() {
    // This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    static int tick = 0;
    makeStatString(); //set the status string 

    if (iceMan->getState()) {//if iceMan is alive then call do something 
        iceMan->doSomething();
    }
    else if (!(iceMan->getState())) {  // Actor is dead.
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }

    if (iceMan->getBarrels() == 0)
        return GWSTATUS_FINISHED_LEVEL;


    // As we begin to add if statements here for dynamically allocating new goodies I was thinking we could push them into the
    // goodie vector from there perhaps and then use this to call their doSomething()'s.
    for (auto it = goodies.begin(); it != goodies.end(); ++it) {
        (*it)->doSomething();
    }

    makeProtesters();

    //make sonar or water pool 
    tick++;
    int G = (getLevel() * 25) + 300;
    if (tick == G) {
        srand(time(0));
        int chance = (rand() % 4) + 1;
        if (chance == 1)
            makeSonar();
        else
            makeWaterPool();
        tick = 0;
    }
    //delete all dead goodies 
    vector<Actor*>::iterator temp;

    for (auto p = goodies.begin(); p != end(goodies); p++) {
        if (!goodies.empty()) {
            if (!(*p)->getState()) {
                delete* p;
                temp = goodies.erase(p);
                if (temp != goodies.begin())
                    --temp;
                p = temp;
            }
        }

    }


    return GWSTATUS_CONTINUE_GAME;
}


void StudentWorld::cleanUp() {

    delete iceMan;
    iceMan = nullptr;
    //delete all ice objects
    for (int row = 0; row < (VIEW_HEIGHT - 4); row++) {
        for (int col = 0; col < VIEW_WIDTH; col++) {
            if (iceCube[col][row] != nullptr)
                destroyIce(col, row);
        }
    }

    //delete all goodies
   /* vector<Actor*>::iterator temp;
    int i = 0;
    for (auto p = goodies.begin(); p != end(goodies); p++) {
        if (!goodies.empty()) {
            if (p[i] != nullptr) {
                delete p[i];
                temp = goodies.erase(p);
                if (temp != goodies.begin())
                    --temp;
                p = temp;
            }
        }
        i++;
    }*/
    for (auto it = goodies.begin(); it != goodies.end(); ++it) {
        delete (*it);
    }
    goodies.clear();

}

void StudentWorld::makeIceMan() { // Creates Ice Man.
    iceMan = new IceMan(this);
}



void StudentWorld::makeIceCubes() { // Creates Ice Field.

    for (int column = 0; column < VIEW_WIDTH; ++column) { //x
        if (column == 30) {
            for (int x = 0; x < 4; x++) {
                //add 4*4 block at the bottom of column 30
                for (int row = 0; row < 4; ++row) {
                    iceCube[column][row] = new Ice(column, row, this);
                }

                //set pointers to nullptr
                for (int row = 4; row < (VIEW_HEIGHT - 4); ++row) {
                    iceCube[column][row] = nullptr;
                }
                column++; // Skip middle four column as initial hole.

            }
        }
        for (int row = 0; row < (VIEW_HEIGHT - 4); ++row) {//y
            iceCube[column][row] = new Ice(column, row, this);
        }
    }
}

void StudentWorld::makeProtesters() {
    if (timeToCreateNewProtester()) {
        // Still need to calculate probability for desired protester to be created.
        Actor* protester = new RegularProtester(this);
        goodies.push_back(protester);
    }
}

void StudentWorld::makeGoodies() {
    int num = (getLevel() / 2) + 2;
    int B = std::min(num, 9);
    num = 5 - (getLevel() / 2);
    int G = std::max(num, 2);
    num = 2 + getLevel();
    int L = std::min(num, 21);

    makeBoulders(B);
    makeGold(G);
    makeOilBarrels(L);
    getIceMan()->setBarrels(L);
}
void StudentWorld::makeBoulders(int num) {

    srand(time(0));
    int x, y;
    for (num; num > 0; num--) {
        do {
            x = rand() % 60;
            while (x == 27 || x == 28 || x == 29 || x == 30 || x == 31 || x == 32 || x == 33) {
                x = rand() % 60;
            }
            y = (rand() % 36) + 20;
        } while (!checkRadiusForConstruction(x, y));
        Actor* ptr = new Boulder(x, y, this);
        goodies.push_back(ptr);

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (iceCube[x + i][y + j] != nullptr)
                    destroyIce(x + i, y + j);
            }
        }
    }

}

void StudentWorld::makeGold(int num) {
    int x, y;
    srand(time(0));
    for (num; num > 0; num--) {
        do {
            x = rand() % 60;
            while (x == 27 || x == 28 || x == 29 || x == 30 || x == 31 || x == 32 || x == 33) {
                x = rand() % 60;
            }
            y = rand() % 56;
        } while (!checkRadiusForConstruction(x, y));
        Actor* ptr = new GoldNugget(x, y, this, false);
        goodies.push_back(ptr);
    }
}

void StudentWorld::makeOilBarrels(int num) {
    int x, y;
    srand(time(0));
    for (num; num > 0; num--) {
        do {
            x = rand() % 60;
            while (x == 27 || x == 28 || x == 29 || x == 30 || x == 31 || x == 32 || x == 33) {
                x = rand() % 60;
            }
            y = rand() % 56;
        } while (!checkRadiusForConstruction(x, y));
        Actor* ptr = new OilBarrel(x, y, this);
        goodies.push_back(ptr);
    }
}
void StudentWorld::makeSonar() {
    Actor* ptr = new Sonar(0, 60, this);
    goodies.push_back(ptr);
}





void StudentWorld::makeWaterPool() {
    //not working properly
    srand(time(0));
    int x = rand() % 60;
    int y = rand() % 56;
    int stop = 0;
    while (isIceAt(x, y, x + 3, y + 4)) {
        x = rand() % 60;
        y = rand() % 56;
        stop++;
        if (stop == 1000)
            return;
    }
    if (!isIceAt(x, y, x + 3, y + 4)) {
        Actor* ptr = new WaterPool(x, y, this);
        goodies.push_back(ptr);
    }
}

void StudentWorld::makeSquirt(int x, int y) {
    Actor* ptr = new Squirt(x, y, getIceMan()->getDirection(), this);
    goodies.push_back(ptr);

}


void StudentWorld::makeDroppedGoldNugget() {
    Actor* ptr = new GoldNugget(getIceMan()->getX(), getIceMan()->getY(), this, true);
    goodies.push_back(ptr);
}

bool StudentWorld::checkRadiusForConstruction(int x, int y) {
    for (const auto p : goodies) {
        double distance = sqrt((p->getX() - x) * (p->getX() - x) + (p->getY() - y) * (p->getY() - y));
        if (distance <= 6) {
            return false;
        }

    }
    return true;
}

void StudentWorld::makeStatString() {
    //format: Lvl: _ Lives: _ Hlth: _ Wtr: _ Gld: _ Oil Left: _ Sonar: _ Scr: _ 
    /*Does not print hlth, wtr, gld, oil left, sonar*/
    int level = getLevel();
    int lives = getLives();
    int score = getScore();
    int oilLeft = iceMan->getBarrels();
    int health = iceMan->getHitPoints() * 10;
    int water = iceMan->getWater();
    int gold = iceMan->getGoldNuggets();
    int sonar = iceMan->getSonar();
    //Needs work but should be okay for testing

    string gameStat = "Lvl: ";

    string add = to_string(level);
    gameStat += add;

    gameStat += " Lives: ";
    add = to_string(lives);
    gameStat += add;

    //Do not have anything that keeps track of health yet
    gameStat += " Hlth: ";
    add = to_string(health);
    gameStat += add;
    gameStat += "% Wtr: ";
    add = to_string(water);
    gameStat += add;
    gameStat += " Gld: ";
    add = to_string(gold);
    gameStat += add;
    gameStat += " Oil Left: ";
    add = to_string(oilLeft);
    gameStat += add;
    gameStat += " Sonar: ";
    add = to_string(sonar);
    gameStat += add;
    //get, convert and add score to string 
    gameStat += " Scr: ";
    add = to_string(score);
    gameStat += add;

    setGameStatText(gameStat);
}

bool StudentWorld::isIcePresent(int x, int y) {
    if (y < (VIEW_HEIGHT - 4)) { //if out of bounds
        if (iceCube[x][y] != nullptr) { //if ice is not present 
            if ((*iceCube[x][y]).getState())
                return true;
            else
                return false;
        }
    }
    return false;
}

void StudentWorld::destroyIce(int x, int y) {
    delete iceCube[x][y];
    iceCube[x][y] = nullptr;
}

bool StudentWorld::isIceAt(const int actorX, const int actorY, const int endX, const int endY) { // name better

    if (actorX + 4 == endX && actorY + 3 == endY) { // trying to check rightward
        for (int i = actorY; i <= endY; i++) {
            if (isIcePresent(endX, i)) {
                return true;
            }
        }
    }
    else if (actorX - 1 == endX && actorY + 3 == endY) { // trying to check leftward
        for (int i = actorY; i <= endY; i++) {
            if (isIcePresent(endX, i)) {
                return true;
            }
        }
    }
    else if (actorY + 4 == endY && actorX + 3 == endX) { // trying to check upward
        for (int i = actorX; i <= endX; i++) {
            if (isIcePresent(i, endY)) {
                return true;
            }
        }
    }
    else if (actorY - 1 == endY && actorX + 3 == endX) { // trying to check downward
        for (int i = actorX; i <= endX; i++) {
            if (isIcePresent(i, endY)) {
                return true;
            }
        }
    }
    return false;

    /*
    Coordinate Guide:
    To check rightward send: getWorld()->isIceAt(getX(), getY(), getX() + 4, getY() + 3
    To check leftward send: getWorld()->isIceAt(getX(), getY(), getX() - 1, getY() + 3
    To check up send: getWorld()->isIceAt(getX(), getY(), getX() + 3, getY() + 4
    To check down send: getWorld()->isIceAt(getX(), getY(), getX() + 3, getY() - 1
    */
}

bool StudentWorld::isBlockableActorNearby(Actor* actorChecking, int radius) {
    for (auto it = goodies.begin(); it != goodies.end(); ++it) {
        if ((*it)->canBlock()) {
            if (actorChecking->checkRadius(actorChecking->getX(), actorChecking->getY(),
                (*it)->getX(), (*it)->getY(), radius)) { // if blocking actor within radius
                return true;
            }
        }
    }
    return false;
}

IceMan* StudentWorld::getIceMan() {
    return iceMan;
}


bool StudentWorld::timeToCreateNewProtester() {
    int temp1 = (200 - getLevel());
    double temp2 = 2 + (1.5 * getLevel());
    double maxProtesters = std::min(15.0, temp2);

    if (_ticksSinceLastProtester <= 0 && _numProtesters < maxProtesters) {
        _ticksSinceLastProtester = std::max(25, temp1);
        _numProtesters++;
        return true;
    }
    else {
        _ticksSinceLastProtester--;
        return false;
    }

}

bool StudentWorld::boulderPresent(int x, int y) {
    //find boulder
    //check if it present at x,y
    for (const auto p : goodies) {
        if (p->canBlock()) {
            for (int i = -2; i < 3; i++) {
                if ((p->getX() + i) == x && p->getY() == (y - 3))
                    return true;

            }
        }
    }
    return false;
}

////////////// ANNOY PERSON        //////////////////
//annoy's protestors if they lie withing the provided radius. If all protesters
//are to be annoyed annoyAll -> true
//there is a mistake with the return values: I think it's fixed but still need to test
bool StudentWorld::annoyPerson(int x, int y, int annoyValue, int radius, bool annoyAll) {
    for (const auto p : goodies) {
        if (p->isAnnoyable()) {
            double distance = sqrt((p->getX() - x) * (p->getX() - x) + (p->getY() - y) * (p->getY() - y));
            if (distance <= radius) {
                p->annoy(annoyValue);
                if (!annoyAll)
                    return true;
            }
        }
    }
    return false;
}


//////                        PICKGOLD          /////////////////////

// enables a protester within a provided radius to pick gold up
bool StudentWorld::pickGold(int x, int y, int radius) {
    for (const auto p : goodies) {
        if (p->isAnnoyable()) {
            double distance = sqrt((p->getX() - x) * (p->getX() - x) + (p->getY() - y) * (p->getY() - y));
            if (distance <= radius) {
                //add method to bribe protester!
                return true;
            }
        }
    }
    return false;
}

//////////               DESTRUCTOR       //////////////////////////////////////
StudentWorld::~StudentWorld() {
    for (int row = 0; row < (VIEW_HEIGHT - 4); row++) {
        for (int col = 0; col < VIEW_WIDTH; col++) {
            if (iceCube[col][row] != nullptr)
                destroyIce(col, row);
        }
    }

    if (iceMan != nullptr) {
        delete iceMan;
        iceMan = nullptr;
    }
}
//#include "StudentWorld.h"
//#include <string>
//using namespace std;

//// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

//GameWorld* createStudentWorld(string assetDir) {
//    return new StudentWorld(assetDir);
//}

//StudentWorld::StudentWorld(std::string assetDir)
//    : GameWorld(assetDir), iceMan(nullptr) {
//    _ticksSinceLastProtester = 0;
//    _numProtesters = 0;
//}

//int StudentWorld::init() {
//        makeIceMan();
//        makeGoodies();
//        makeIceCubes();

//    return GWSTATUS_CONTINUE_GAME;
//}

//int StudentWorld::move() {
//    // This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
//    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.

//    makeStatString(); //set the status string 

//    if (iceMan->getState()) {//if iceMan is alive then call do something 
//        iceMan->doSomething();
//    }

//        if (!(iceMan->getState())) {  // Actor is dead.
//            decLives();
//            return GWSTATUS_PLAYER_DIED;
//        }

//        if (iceMan->getBarrels() == 0) // There may be a cleaner indicator for the end of a level, but this is a placeholder.
//            return GWSTATUS_FINISHED_LEVEL;

//    makeProtesters();

//    // As we begin to add if statements here for dynamically allocating new goodies I was thinking we could push them into the
//    // goodie vector from there perhaps and then use this to call their doSomething()'s.
//    for (auto it = goodies.begin(); it != goodies.end(); ++it) {
//        (*it)->doSomething();
//    }

//    //delete all dead goodies 


//     vector<Actor*>::iterator temp;

//    for (auto p = goodies.begin(); p != end(goodies); p++) {
//        if (!goodies.empty()) {
//            if (!(*p)->getState()) {
//                delete *p;
//                temp = goodies.erase(p);
//                if (temp != goodies.begin())
//                    --temp;
//                p = temp;
//            }
//        }

//    }


//    return GWSTATUS_CONTINUE_GAME;
//}


//void StudentWorld::cleanUp() {

//    delete iceMan;
//    iceMan = nullptr;
//    //delete all ice objects
//    for (int row = 0; row < (VIEW_HEIGHT - 4); row++) {
//        for (int col = 0; col < VIEW_WIDTH; col++) {
//            if (iceCube[col][row] != nullptr)
//                destroyIce(col, row);
//        }
//    }


//    //delete all goodies
//    for (auto it = goodies.begin(); it != goodies.end(); ++it) {
//        delete (*it);
//    }
//    goodies.clear();


//}

//void StudentWorld::makeIceMan() { // Creates Ice Man.
//    iceMan = new IceMan(this);
//}



//void StudentWorld::makeIceCubes() { // Creates Ice Field.

//    for (int column = 0; column < VIEW_WIDTH; ++column) { //x
//        if (column == 30) {
//            for (int x = 0; x < 4; x++) {
//                //add 4*4 block at the bottom of column 30
//                for (int row = 0; row < 4; ++row) {
//                    iceCube[column][row] = new Ice(column, row, this);
//                }

//                //set pointers to nullptr
//                for (int row = 4; row < (VIEW_HEIGHT - 4); ++row) {
//                    iceCube[column][row] = nullptr;
//                }
//                column++; // Skip middle four column as initial hole.

//            }
//        }
//        for (int row = 0; row < (VIEW_HEIGHT - 4); ++row) {//y
//            iceCube[column][row] = new Ice(column, row, this);
//        }
//    }
//}

//void StudentWorld::makeProtesters() {
//    if (timeToCreateNewProtester()) {
//        // Still need to calculate probability for desired protester to be created.
//        Actor* protester = new RegularProtester(this);
//        goodies.push_back(protester);
//    }
//}

////void StudentWorld::makeGoodies() {
////    int num = (getLevel() / 2) + 2;
////    int B = std::min(num, 9);
////    num = 5 - (getLevel() / 2);
////    int G = std::max(num , 2);
////    num = 2 + getLevel();
////    int L = std::min(num, 21);

////    makeBoulders(B);
////    makeGold(G);
////    makeOilBarrels(L);
////    getIceMan()->setBarrels(L);
////}

////void StudentWorld::makeGold(int num) {
////    int x, y;
////    for (num; num > 0; num--) {
////        do {
////            x = rand() % 60;
////            while (x == 30 || x == 31 || x == 32 || x == 33) {
////                x = rand() % 60;
////            }
////            y = rand() % 56;
////        } while (!checkRadiusForConstruction(x, y));
////        Actor* ptr = new GoldNugget(x, y, this, false);
////        goodies.push_back(ptr);
////    }
////}

////void StudentWorld::makeOilBarrels(int num) {
////    int x, y;
////    for (num; num > 0; num--) {
////        do {
////            x = rand() % 60;
////            while (x == 30 || x == 31 || x == 32 || x == 33) {
////                x = rand() % 60;
////            }
////            y = rand() % 56;
////        } while (!checkRadiusForConstruction(x, y));
////        Actor* ptr = new OilBarrel(x, y, this);
////        goodies.push_back(ptr);
////    }
////}

////void StudentWorld::makeBoulders(int num) {

////    int x, y;
////    for (num; num > 0; num--) {
////        do {
////             x = rand() % 60;
////             while (x == 30 || x == 31 || x == 32 || x == 33) {
////                 x = rand() % 60;
////             }
////             y = (rand() % 36) + 20;
////        } while (!checkRadiusForConstruction(x, y));
////        Actor* ptr = new Boulder(x, y, this);
////        goodies.push_back(ptr);


////        for (int i = 0; i < 4; i++) {
////            for (int j = 0; j < 4; j++) {
////                if (iceCube[x + i][y + j] != nullptr) {
////                     destroyIce(x + i, y + j);
////                }
////            }
////        }




////    }

////}

////bool StudentWorld::checkRadiusForConstruction(int x, int y) {
////    for (const auto p : goodies) {
////            double distance = sqrt((p->getX() - x) * (p->getX() - x) + (p->getY() - y) * (p->getY() - y));
////            if (distance <= 6) {
////                return false;
////            }

////    }
////    return true;
////}
//void StudentWorld::makeGoodies() {
//    int num = (getLevel() / 2) + 2;
//    int B = std::min(num, 9);
//    num = 5 - (getLevel() / 2);
//    int G = std::max(num , 2);
//    num = 2 + getLevel();
//    int L = std::min(num, 21);

//    makeBoulders(B);
//    makeGold(G);
//    makeOilBarrels(L);
//    getIceMan()->setBarrels(L);
//}
//void StudentWorld::makeBoulders(int num) {

//    int x, y;
//    for (num; num > 0; num--) {
//        do {
//             x = rand() % 60;
//             while (x == 30 || x== 31|| x== 32 || x== 33) {
//                 x = rand() % 60;
//             }
//             y = (rand() % 36) + 20;
//        } while (!checkRadiusForConstruction(x, y));
//        Actor* ptr = new Boulder(x, y, this);
//        goodies.push_back(ptr);

//        for (int i = 0; i < 4; i++) {
//            for (int j = 0; j < 4; j++) {
//                if (iceCube[x + i][y+j] != nullptr)
//                    destroyIce(x +i, y+j);
//            }
//        }
//    }

//}

//void StudentWorld::makeGold(int num) {
//    int x, y;
//    for (num; num > 0; num--) {
//        do {
//            x = rand() % 60;
//            while (x == 30 || x == 31 || x == 32 || x == 33) {
//                x = rand() % 60;
//            }
//            y = rand() % 56;
//        } while (!checkRadiusForConstruction(x, y));
//        Actor* ptr = new GoldNugget(x, y, this, false);
//        goodies.push_back(ptr);
//    }
//}

//void StudentWorld::makeOilBarrels(int num) {
//    int x, y;
//    for (num; num > 0; num--) {
//        do {
//            x = rand() % 60;
//            while (x == 30 || x == 31 || x == 32 || x == 33) {
//                x = rand() % 60;
//            }
//            y = rand() % 56;
//        } while (!checkRadiusForConstruction(x, y));
//        Actor* ptr = new OilBarrel(x, y, this);
//        goodies.push_back(ptr);
//    }
//}

//bool StudentWorld::checkRadiusForConstruction(int x, int y) {
//    for (const auto p : goodies) {
//            double distance = sqrt((p->getX() - x) * (p->getX() - x) + (p->getY() - y) * (p->getY() - y));
//            if (distance <= 6) {
//                return false;
//            }

//    }
//    return true;
//}

//void StudentWorld::makeStatString() {
//    //format: Lvl: _ Lives: _ Hlth: _ Wtr: _ Gld: _ Oil Left: _ Sonar: _ Scr: _ 
//    /*Does not print hlth, wtr, gld, oil left, sonar*/
//    int level = getLevel();
//    int lives = getLives();
//    int score = getScore();
//    int oilLeft = iceMan->getBarrels();
//    int health = iceMan->getHitPoints();
//    int water = iceMan->getWater();
//    int gold = iceMan->getGoldNuggets();
//    int Sonar = iceMan->getSonar();
//    //int oilLeft! maybe make a variable in student world that holds this value

//    string gameStat = "Lvl: ";

//    string levelStr = to_string(level);
//    gameStat += levelStr;

//    gameStat += " Lives: ";
//    string livesStr = to_string(lives);
//    gameStat += livesStr;

//    //Do not have anything that keeps track of health yet
//    gameStat += " Hlth: ";

//    //get, convert and add score to string 
//    gameStat += " Scr: ";
//    string scoreStr = to_string(score);
//    gameStat += scoreStr;

//    setGameStatText(gameStat);
//}

//bool StudentWorld::isIcePresent(int x, int y) {
//    if (y < (VIEW_HEIGHT - 4)) { //if out of bounds
//        if (iceCube[x][y] != nullptr) { //if ice is not present 
//            if ((*iceCube[x][y]).getState())
//                return true;
//            else
//                return false;
//        }
//    }
//    return false;
//}

//void StudentWorld::destroyIce(int x, int y) {
//    delete iceCube[x][y];
//    iceCube[x][y] = nullptr;
//}

//bool StudentWorld::isIceAt(const int actorX, const int actorY, const int endX, const int endY) {

//    if (actorX + 4 == endX && actorY + 3 == endY) { // trying to check rightward
//        for (int i = actorY; i <= endY; i++) {
//            if (isIcePresent(endX, i)) {
//                return true;
//            }
//        }

//    }
//    else if (actorX - 1 == endX && actorY + 3 == endY) { // trying to check leftward
//        for (int i = actorY; i <= endY; i++) {
//            if (isIcePresent(endX, i)) {
//                return true;
//            }
//        }

//    }
//    else if (actorY + 4 == endY && actorX + 3 == endX) { // trying to check upward
//        for (int i = actorX; i <= endX; i++) {
//            if (isIcePresent(i, endY)) {
//                return true;
//            }
//        }
//    }
//    else if (actorY - 1 == endY && actorX + 3 == endX) { // trying to check downward
//        for (int i = actorX; i <= endX; i++) {
//            if (isIcePresent(i, endY)) {
//                return true;
//            }
//        }
//    }
//    return false;

//    /*
//    Coordinate Guide:
//    To check rightward send: getWorld()->isIceAt(getX(), getY(), getX() + 4, getY() + 3
//    To check leftward send: getWorld()->isIceAt(getX(), getY(), getX() - 1, getY() + 3
//    To check up send: getWorld()->isIceAt(getX(), getY(), getX() + 3, getY() + 4
//    To check down send: getWorld()->isIceAt(getX(), getY(), getX() + 3, getY() - 1
//    */
//}

//IceMan* StudentWorld::getIceMan() {
//    return iceMan;
//}


//bool StudentWorld::timeToCreateNewProtester() {
//    int temp1 = (200 - getLevel());
//    double temp2 = 2 + (1.5 * getLevel());
//    double maxProtesters = std::min(15.0, temp2);

//    if (_ticksSinceLastProtester <= 0 && _numProtesters < maxProtesters) {
//        _ticksSinceLastProtester = std::max(25, temp1);
//        _numProtesters++;
//        return true;
//    }
//    else {
//        _ticksSinceLastProtester--;
//        return false;
//    }

//}

//StudentWorld::~StudentWorld() {
//    for (int row = 0; row < (VIEW_HEIGHT - 4); row++) {
//        for (int col = 0; col < VIEW_WIDTH; col++) {
//            if (iceCube[col][row] != nullptr)
//                destroyIce(col, row);
//        }
//    }

//    if (iceMan != nullptr) {
//        delete iceMan;
//        iceMan = nullptr;
//    }
//}


//bool StudentWorld::boulderPresent(int x, int y) {
//    //find boulder
//    //check if it present at x,y
//    for (const auto p : goodies) {
//        if (p->canBlock()) {
//            for (int i = -2; i < 3; i++) {
//                if ((p->getX() + i) == x && p->getY() == (y - 3))
//                    return true;

//            }
//        }
//    }
//    return false;
//}

////annoy's protestors if they lie withing the provided radius
////there is a mistake with the return values
//bool StudentWorld::annoyPerson(int x, int y, int annoyValue, int radius) {
//    for (const auto p : goodies) {
//        if (p->isAnnoyable()) {
//                double distance = sqrt((p->getX() - x) * (p->getX() - x) + (p->getY() - y) * (p->getY() - y));
//                if (distance <= radius) {
//                    p->annoy(annoyValue);
//                    return true;
//                }
//        }
//    }
//    return false;
//}

//bool StudentWorld::pickGold(int x, int y, int radius) {
//    for (const auto p : goodies) {
//        if (p->isAnnoyable()) {
//                double distance = sqrt((p->getX() - x) * (p->getX() - x) + (p->getY() - y) * (p->getY() - y));
//                if (distance <= radius) {
//                    //add method to bribe protester!
//                    return true;
//                }
//        }
//    }
//    return false;
//}