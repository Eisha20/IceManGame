#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <queue>
#include <stack>
#include <memory>
class StudentWorld;
class Coordinate;

class Actor : public GraphObject { // "Base" Class
public:
    Actor(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* swActor);

    virtual void doSomething() = 0; //pure virtual function
    virtual void annoy(int) = 0;
    virtual bool isAnnoyable() = 0;
    virtual bool canBlock() = 0;
    virtual void bribe() = 0;
    virtual bool checkRadius(int x, int y, int checkX, int checkY, int radius);
    void setState(bool state);
    bool getState();
    virtual StudentWorld* getWorld();
    virtual ~Actor();

private:
    bool _state;//Actor is alive or dead
 //   int _level;
    StudentWorld* _swActor;
};


/*************************************************************************************************/
class Person : public Actor {
public:
    Person(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth,
        StudentWorld* swActor, int hitPoints);

    virtual bool isAnnoyable();
    virtual bool canBlock();
    virtual void bribe();
    int getHitPoints();
    void decHitPoints(int hitPoints);
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
    void decSonar();
    void incSonar();
    int getWater();
    void decWater();
    void incWater(int num);
    int getGoldNuggets();
    void incGoldNuggets();
    void decGoldNuggets();
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
    bool isAnnoyable();
    bool canBlock();
    void bribe();
    virtual ~Ice();

private:

};

/******************************************PROTESTER HIERARCHY*****************************/

class Protester : public Person {
public:
    Protester(int image, int hitPoints, StudentWorld* sw_Protester, bool state);

    int getIceManX();
    int getIceManY();
    bool getIsLeavingField();
    void setIsLeavingField(bool isLeavingField);
    bool isAbleToMove();
    bool isAbleToYell();
    bool isIceManInClearSight();
    bool isIceManBlocking();
    bool canPerpendicularlyMove();
    bool isBlocked();
    bool isPathBlocked();
    void moveOneSquare();
    void randomDirection();
    virtual void bribe();
    virtual void doSomething();
    void goBack();
    bool ifVisited(int, int, std::stack<std::shared_ptr<Coordinate>>);
    virtual void setTicksToWaitBetweenMoves(int ticksToWaitBetweenMoves);
    virtual ~Protester();

    void leaveOilField();
private:
    int _ticksToWaitBetweenMoves;
    int _ticksToWaitBetweenYells;
    int _ticksBetweenPerpendicularTurns;
    int _image;
    int _level; // take out
    int _numSquaresToMoveInCurrentDirection;
    bool _isLeavingField;
    std::stack<std::shared_ptr<Coordinate>> store;
    std::shared_ptr<Coordinate> _vertex;
    /* std::stack<Coordinate*> store;
     Coordinate* _vertex;*/
};


class RegularProtester : public Protester {
public:
    RegularProtester(StudentWorld* swRegProtester);
    virtual void annoy(int);
    virtual void bribe();


    virtual ~RegularProtester();
private:


};

class HardcoreProtester : public Protester {
public:
    HardcoreProtester(StudentWorld* swHardcoreProtester);
    virtual void annoy(int);
    virtual void bribe();

    virtual ~HardcoreProtester();
private:

};
///*******************************************************************************************/
//
///******************************************GOODIES HIERARCHY****************************/
//
class Goodie : public Actor {
public:
    Goodie(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth,
        StudentWorld* swActor);
    void annoy(int);
    virtual bool isAnnoyable();
    virtual bool canBlock();
    void bribe();
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
    bool canBlock();
    virtual ~Boulder();

private:
    void fall();
    bool isIcePresentBelow(int x, int y);
    bool _stable;
    bool _playSound;
};



class Squirt : public Goodie {
public:
    Squirt(int x, int y, Direction dir, StudentWorld* swWorld);
    virtual void doSomething();
    virtual ~Squirt();
private:
    int _travelDis;
    void decTravelDis();

};
class Sonar : public Goodie {
public:
    Sonar(int x, int y, StudentWorld* swSonar);
    int getTicks();
    void decTicks();
    void doSomething();
    virtual ~Sonar();
private:
    int _ticksLeft;
};


class WaterPool : public Goodie {
public:
    WaterPool(int x, int y, StudentWorld* swSonar);
    int getTicks();
    void decTicks();
    void doSomething();
    virtual ~WaterPool();
private:
    int _ticksLeft;
};

class GoldNugget : public Goodie {
public:
    GoldNugget(int x, int y, StudentWorld* swNugget, bool isPickableProtester);

    virtual void doSomething();
    virtual ~GoldNugget();
private:
    bool isPermanent();
    bool isPickableProtester();
    bool _pickableProtester;
    bool _permanent;
};


class Coordinate {
public:
    Coordinate(std::pair<int, int> index, int step) : _step(step), _index(index) {}

    int getSteps() { return _step; }
    int getFirst() { return _index.first; }
    int getSecond() { return _index.second; }

private:
    std::pair<int, int> _index;
    int _step;
};

#endif // ACTOR_H_

//ACTORH#deieACTRH#include"GraphObject.h"#ncud<queue>
//#include<stack>
//#clude<memor>
//classclass Coordinate;classActo public GraphObject "Bas"Class
//public:
//    Actor(inmageID,instartX, int startY, Direction dir,doublesize,unsignednt depth, StudentWorld*swAcorvrualvoddoSomething() 0; //pur virual functin
//    vitua voi annoyint = 0;
//    virul bool isAnoyable = 0;
//    virual bool canBlock = 0vrualvodbribe() 0;
//    virual bo checkRaiusint x, int y, int checkX, int checkY, int radius;
//    void sStatbool state;
//    bool StatevrualStudentWorld*Worldvrual~Acorprivate:
//bool _state;Aralvorded
////   nt_levl;StudentWord*_swActor;
//};*************************************************************************************************
//cssPeron:ublAcor{
//pblic:Pronint imageID,intstrtXint startYDrctio dirdouble szunsigned inteh,Studn* swActor, int hiPoit    virtual bool isAnnoyable();
//    virtual bool canBlock();
//    virtual void bribe();
//    int getHitPoints();
//    void decHitPoints(int hitPoints);
//    ~Person();
//private:
//    int _hitPoints;
//};
/////////////// ICE MAN CLASS  //////////////////////////////////////////////////////////
//class IceMan : public Person {
//public:
//    IceMan(StudentWorld* swIceMan);
//    void doSomething();
//    virtual void annoy(int annoyValue);
//    int getSonar();
//    void decSonar();
//    void incSonar();
//    int getWater();
//    void decWater();
//    void incWater(int num);
//    int getGoldNuggets();
//    void incGoldNuggets();
//    void decGoldNuggets();
//    int getBarrels();
//    void setBarrels(int barrelsLeft);
//    void reduceBarrels();
//    virtual ~IceMan();private:
//    int _water;
//    int _sonar;
//    int _goldNuggets;
//    int _barrelsLeft;
//};
///**************************************************************************************************/
//css Ic  public Actor {
//public
//    Ice(nt sttX, int statY, StudntWord* swIce);
//    void doSomething();
//    void annoyint;
//    boolisAnnoyable();ool canBock
//    voidbribe();    virtual ~Ice();pva};******************************************ROTETERHIERARCHY*****************************claspublicPrsonpliProerintimainhitPointsSudn*sw_ProtesterboolintgetIceManX();int getIceManY();
//    bool getIsLeavingField(); // might be able to delete.
//    void setIsLeavingField(bool isLeavingField); // might be able to delete.
//    bool isAbleToMove(); // change name tickLimitMet?
//    bool isAbleToYell();
//    bool isIceManInClearSight();
//    bool isIceManBlocking();
//    bool canPerpendicularlyMove();
//    bool isBlocked();
//    bool isPathBlocked();
//    void moveOneSquare();
//    void randomDirection();
//    virtual void bribe();
//    virtual void doSomething();
//    void goBack();
//    bool ifVisited(int, int, std::stack<std::shared_ptr<Coordinate>>);
//    virtual void setTicksToWaitBetweenMoves(int ticksToWaitBetweenMoves);
//    virtual ~Protester();voileaveOilField();
//private:
//    in_ticksToWaitBetwenMovs;
//    int_icksToWitBetweenYels;
//    int_tckBeweenPerpendiculrTurs;int _image;
//    int _level; takutnt_numquresToMoveInCurrenDirction;bool_sLavgField;//iht babltodelete.st::stack<std::shard_ptr<Coodint>> torestd::shad_pr<Coodiate> _vertex/*st:stack<Crdina*>storeCoordinate*_vtx*/}lsReguarProtser publicProtester{
//public:
//    ReularProtserStudentWorld* swRegProtestervitul od annoyintvirtual void bribe();virtual ~RgularProetrprivate:;class HardcoreProtester : public Protester {
//public:
//    HardcoreProtester(StudentWorld* swHardcoreProtester);
//    virtual void annoy(int);
//    virtual void bribe();
//    virtua~Harorprivate:};
/////*******************************************************************************************/
////
/////******************************************GOODIESHIERARCHY****************************///
//clasGoode:publicActor{pblic:GoodenmIDisaXintaY,Drectionrdoublsizunsinintdph,Studn* swActrvi anyintvirual bo isAyabvirtuaboolnBck;vodbrbvrua~Goodirivte:
//}casOlBarrel:publicGoodiepublic:
//OilBrrlint x, int y, StudentWord* wOilBarrlvirtalvoiddoSomething()virtual ~OilBarrel();
//private:;class Bur  public Goodie {
//public
//    Buer(int x, int y, StudentWorld* swBoulder);
//    virtal void doSomhing();
//    bool canBlock;
//    virtual~Boulder();private:    vid fll();    bol isIcePresetBelw;bool_sabbool_playSoud;class quit  public Goodie {
//public
//    Squirt(int x, int y, Direction dir, StudentWorld* swWorld);
//    virtual void doSomething();
//    virtual quit;
//private:
//    int _travelDis;
//    voiddecTravelDis();;
//class Sonar : public Goodie {
//public:
//    Sonar(int x, int y, StudentWorld* swSonar);
//    int getTicks();
//    void decTicks();
//    void doSomething();
//    virtual ~Sonar();
//private:
//    int _ticksLeft;
//};class  public Goodie {
//public
//    WaterPool(int x, int y, StudentWorld* swSonar);
//    int ;
//    void decTicks();
//    void doSomething();
//    virtual~WaterPool();
//private:in;classGdNugget public Goodie public:
//GoldNugget(int x, int y, StudentWorld* swNugge, bool isPableProtetr)    virtual void doSomething();
//    virtual ~GoldNugget();
//private:
//    bool isPermanent();
//    bool isPickableProtester();
//    bool _pickableProtester;
//    bool _permanent;
//};
//classCratepublc:Coordnatestd::pir<nininxint:_stpstep,_index(index    int getSteps() { return _step; }
//    int getFirst() { return _index.first; }
//    int getSecond() { return _index.second; }
//priv
//    std:pir<in,int> _index;
//    int _step;
//};

//#endif // ACTOR_H_