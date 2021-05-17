#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

#include "Actor.h"
#include <vector>
// class IceMan;
// class Ice;

class StudentWorld : public GameWorld {
public:

    StudentWorld(std::string assetDir);

    virtual int init(); //gets called 1) game starts 2) during the start of a new level 3) loss of life
                        //when it gets called all previous objects should be deleted 

    virtual int move();

    virtual void cleanUp();

    void makeIceMan();

    void makeIceCubes();

    void makeGoodies();

    void makeStatString();
    bool isIcePresent(int x, int y);
    void destroyIce(int x, int y);
private:
    IceMan* iceMan;

    //(x, y) coordinate: column, row
    Ice* iceCube[VIEW_WIDTH][VIEW_HEIGHT - 4];//an array of ice* so that we can access the objects. 
    std::vector <Actor*> goodies;
};

#endif // STUDENTWORLD_H_
