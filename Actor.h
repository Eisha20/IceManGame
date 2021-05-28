#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"


class StudentWorld;

class Actor : public GraphObject { // "Base" Class
public:
    Actor(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* swActor);

    virtual void doSomething() = 0; //pure virtual function
    virtual void annoy(int) = 0;
    void setState(bool state);
    bool getState();
    virtual StudentWorld* getWorld();
    virtual ~Actor();

private:
    bool _state;//Actor is alive or dead
    int _level;
    StudentWorld* _swActor;
};


/*************************************************************************************************/
class Person : public Actor {
public:
    Person(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth,
        StudentWorld* swActor, int hitPoints);

    int getHitPoints();
    void setHitPoints(int hitPoints);
    ~Person();
private:
    int _hitPoints;
};
///////////// ICE MAN CLASS  //////////////////////////////////////////////////////////

class IceMan : public Person {
public:
    IceMan(StudentWorld* swIceMan);
    void doSomething();
    virtual void annoy(int annoyValue);
    int getSonar();
    void setSonar(int sonar);
    int getWater();
    void setWater(int water);
    int getGoldNuggets();
    void setGoldNuggets(int goldNuggets);
    int getBarrels();
    void setBarrels(int barrelsLeft);
    void reduceBarrels();
    virtual ~IceMan();

private:
    int _water;
    int _sonar;
    int _goldNuggets;
    int _barrelsLeft;
};

/**************************************************************************************************/
class Ice : public Actor {
public:
    Ice(int startX, int startY, StudentWorld* swIce);
    void doSomething();
    void annoy(int);

    virtual ~Ice();

private:

};

/******************************************PROTESTER HIERARCHY*****************************/

class Protester : public Person {
public:
    Protester(int image, int hitPoints, StudentWorld* sw_Protester, bool state);

    bool getIsLeavingField();
    void setIsLeavingField(bool isLeavingField);
    bool isWithinShoutingDistance();
    bool isAbleToMove();
    bool isAbleToYell();
    bool isIceManInClearSight();
    bool isBlocked();
    void moveOneSquare();
    void randomDirection();
    virtual void doSomething();

    virtual ~Protester();

private:
    int _ticksToWaitBetweenMoves;
    int _ticksToWaitBetweenYells;
    int _image;
    int _level;
    int _numSquaresToMoveInCurrentDirection;
    bool _isLeavingField;
};


class RegularProtester : public Protester {
public:
    RegularProtester(StudentWorld* swRegProtester);
    virtual void annoy(int);



    virtual ~RegularProtester();
private:


};


/*
class RegularProtester : public Protester {
public:
    RegularProtester(StudentWorld* swR_Protester);

  virtual ~RegularProtester();
private:

}; */

//class HardcoreProtester : public Protester {
//public:
//    HardcoreProtester();
//
//    virtual ~HardcoreProtester();
//private:
//
//};
///*******************************************************************************************/
//
///******************************************GOODIES HIERARCHY****************************/
//
class Goodie : public Actor {
public:
    Goodie(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth,
        StudentWorld* swActor);
    void annoy(int);
    virtual bool checkRadius3(int x, int y, int checkX, int checkY);
    virtual bool checkRadius4(int x, int y, int checkX, int checkY);
    virtual ~Goodie();
private:
};

class OilBarrel : public Goodie {
public:
    OilBarrel(int x, int y, StudentWorld* swOilBarrel);
    virtual void doSomething();
    virtual ~OilBarrel();
private:
};

class Boulder : public Goodie {
public:
    Boulder(int x, int y, StudentWorld* swBoulder);
    virtual void doSomething();
    virtual ~Boulder();

private:
    void fall();
    bool isIcePresentBelow(int x, int y);
    bool _stable;
};



class Squirt : public Goodie {
public:
    Squirt(int x, int y, Direction dir, StudentWorld* swWorld);
    virtual void doSomething();
    virtual ~Squirt();
private:
    int _travelDis;
};
//class Sonar : public Goodies {
//public:
//    Sonar();
//
//  virtual ~Sonar();
//private:
//
//};
//
//class GoldNugget : public Goodies {
//public: 
//    GoldNugget();
//    virtual ~GoldNugget();
//private:
//
//};

#endif // ACTOR_H_
