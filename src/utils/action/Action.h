#pragma once
#include "TrenchCoatRepository.h"

class Action {
protected:
    TrenchCoatRepository& repository_;
public:
    Action(TrenchCoatRepository& repository);
    virtual ~Action() = default;

    virtual void undo()=0;
    virtual void redo()=0;
};

class ActionAdd: public Action {
private:
    TrenchCoat coat;
public:
    ActionAdd(TrenchCoatRepository& repository, TrenchCoat coat);
    void undo() override;
    void redo() override;
};

class ActionRemove: public Action {
private:
    TrenchCoat coat;
public:
    ActionRemove(TrenchCoatRepository& repository, TrenchCoat coat);
    void undo() override;
    void redo() override;
};

class ActionUpdate: public Action {
private:
    TrenchCoat oldCoat, newCoat;
public:
    ActionUpdate(TrenchCoatRepository& repository, TrenchCoat oldCoat, TrenchCoat newCoat);
    void undo() override;
    void redo() override;
};
