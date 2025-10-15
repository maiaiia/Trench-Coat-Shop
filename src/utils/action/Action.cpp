//
// Created by Maia Ungureanu on 31.05.2025.
//

#include "Action.h"

Action::Action(TrenchCoatRepository &repository): repository_{repository} {}

ActionAdd::ActionAdd(TrenchCoatRepository &repository, TrenchCoat coat):
Action(repository), coat{coat} {}

void ActionAdd::undo() {
    this->repository_.removeCoat(coat);
}

void ActionAdd::redo() {
    this->repository_.addCoat(coat);
}

ActionRemove::ActionRemove(TrenchCoatRepository &repository, TrenchCoat coat):
Action(repository), coat{coat} {}

void ActionRemove::undo() {
    this->repository_.addCoat(coat);
}

void ActionRemove::redo() {
    this->repository_.removeCoat(coat);
}

ActionUpdate::ActionUpdate(TrenchCoatRepository &repository, TrenchCoat oldCoat, TrenchCoat newCoat):
Action(repository), oldCoat{oldCoat}, newCoat{newCoat} {}

void ActionUpdate::undo() {
    this->repository_.removeCoat(this->newCoat);
    this->repository_.addCoat(this->oldCoat);
}

void ActionUpdate::redo() {
    this->repository_.removeCoat(this->oldCoat);
    this->repository_.addCoat(this->newCoat);
}

