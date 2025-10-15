#pragma once
#include <QObject>

#include <qaction.h>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QMenu>
#include <QtWidgets/QListWidget>
#include <QTableView>

#include "UI.h"
#include <QShortcut>
#include "models/BasketModel.h"

class GUI: public QWidget, public UI {
    Q_OBJECT
private:
    QPushButton* addCoatButton, *deleteCoatButton, *updateCoatButton, *showStatisticButton, *undoButton, *redoButton;
    QListWidget* trenchCoatsList, *basketList;
    QPushButton* shopButton, *openBasketButton;
    QPushButton* exitButton;
    QLabel* totalLabel;
    BasketModel& model;
    QTableView* BasketView;

    QShortcut* undoShortcut, *redoShortcut, *addShortcut, *deleteShortcut, *updateShortcut;
public:
    explicit GUI(AdminService& adminService, UserService& userService, bool adminMode, BasketModel& model, QWidget* parent=nullptr);
    void run() override;
protected:
    void handleException(const std::exception& exception) const override; //!
    void handleErrorMessage(const std::string& message) const override;
    void handleSuccessfulOperation(const std::string& message) const override; //!

    TrenchCoat readValidCoat() override; //!
    TrenchCoat readCoatIdentifiers() override; //!
    [[nodiscard]] char readSize() const override;
    void runShop(std::vector<TrenchCoat> &) const override;

    std::string readAttributeType() override; //!
    std::string readNewValue(const std::string& attribute) override; //!
    void displayBasket() const override;
private:
    void initGUI();
    void createAdminWindow();
    void createUserWindow();

    void connectSignalsAndSlots();
    void connectSignalsAndSlotsAdmin();
    void connectSignalsAndSlotsUser();

    void updateRepoDisplay() const;
    void updateBasketDisplay() const;
    void displayStatistics() const;

};
