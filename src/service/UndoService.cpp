#include "UndoService.h"

UndoException::UndoException(std::string message): message{message} {}
const char *UndoException::what() const noexcept {
    return this->message.c_str();
}

void UndoService::undo() {
    if (this->undoStack.empty())
        throw UndoException("No actions to undo\n");
    this->undoStack.back()->undo();
    this->redoStack.push_back(std::move(this->undoStack.back()));
    this->undoStack.pop_back();
}
void UndoService::redo() {
    if (this->redoStack.empty())
        throw UndoException("No actions to redo\n");
    this->redoStack.back()->redo();
    this->undoStack.push_back(std::move(this->redoStack.back()));
    this->redoStack.pop_back();
}

void UndoService::addAction(std::unique_ptr<Action> action) {
    this->redoStack.clear();
    this->undoStack.push_back(std::move(action));
}



