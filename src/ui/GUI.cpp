#include "GUI.h"
#include "GUI.h"
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QApplication>
#include <QtCharts/QChartView>
#include "statistics/StatisticsGenerator.h"
#include <QShortcut>

GUI::GUI(AdminService &adminService, UserService &userService, bool adminMode, BasketModel& model, QWidget* parent):
UI{adminService, userService, adminMode}, model{model}, QWidget{parent}{
    this->initGUI();
    this->connectSignalsAndSlots();
    if (this->adminMode)
        this->updateRepoDisplay();
}

void GUI::handleException(const std::exception &exception) const {
    QMessageBox::critical(nullptr, "Error", exception.what());
}
void GUI::handleErrorMessage(const std::string &message) const {
    QMessageBox::critical(nullptr, "Error", message.c_str());
}
void GUI::handleSuccessfulOperation(const std::string &message) const {
    QMessageBox::information(nullptr, "Success", QString::fromStdString(message));
}

void GUI::initGUI() {
    this->adminMode ? createAdminWindow() : createUserWindow();
}
void GUI::createAdminWindow(){
    this->setWindowTitle("ADMIN MENU");
    auto* layout = new QHBoxLayout{this};

    //button layout
    auto* buttonsWidget = new QWidget{};
    auto* buttonsLayout = new QVBoxLayout{buttonsWidget};

    this->addCoatButton = new QPushButton{"Add coat"};
    this->deleteCoatButton = new QPushButton{"Delete coat"};
    this->updateCoatButton = new QPushButton{"Update coat"};
    this->showStatisticButton = new QPushButton{"Show statistics"};
    this->undoButton = new QPushButton{"Undo"};
    this->redoButton = new QPushButton{"Redo"};
    this->exitButton = new QPushButton{"Exit"};

    buttonsLayout->addWidget(this->addCoatButton);
    buttonsLayout->addWidget(this->deleteCoatButton);
    buttonsLayout->addWidget(this->updateCoatButton);
    buttonsLayout->addWidget(this->showStatisticButton);
    buttonsLayout->addWidget(this->undoButton);
    buttonsLayout->addWidget(this->redoButton);
    buttonsLayout->addWidget(this->exitButton); //FIXME make interface prettier

    //trench coat display
    auto* coatsWidget = new QWidget{};
    auto* coatsLayout = new QVBoxLayout{coatsWidget};

    auto* label = new QLabel{"Trench Coats"};
    label->setAlignment(Qt::AlignCenter);
    QFont font{ "Verdana", 20 };
    label->setFont(font);
    coatsLayout->addWidget(label);

    this->trenchCoatsList = new QListWidget{};
    coatsLayout->addWidget(this->trenchCoatsList);
    this->trenchCoatsList->setSpacing(1);
    this->trenchCoatsList->setAlternatingRowColors(true);

    //add all widgets
    layout->addWidget(buttonsWidget);
    layout->addWidget(coatsWidget);
}
void GUI::createUserWindow(){
    this->setWindowTitle("USER MENU");
    auto* layout = new QHBoxLayout{this};

    auto* buttonsWidget = new QWidget{};

    this->shopButton = new QPushButton("Shop for trench coats", nullptr);
    this->openBasketButton = new QPushButton("Open shopping basket in a file", nullptr);
    this->exitButton = new QPushButton{"Exit"};

    auto* vLayout = new QVBoxLayout{buttonsWidget};
    vLayout->addWidget(this->shopButton);
    vLayout->addWidget(this->openBasketButton);
    vLayout->addWidget(this->exitButton);

    //shopping basket
    auto *basketWidget = new QWidget{};
    auto *basketLayout = new QVBoxLayout{basketWidget};

    auto* label = new QLabel{"Shopping Basket"};
    label->setAlignment(Qt::AlignCenter);
    label->setFont(QFont{"Verdana", 20});
    basketLayout->addWidget(label);

    this->basketList = new QListWidget{};
    basketLayout->addWidget(this->basketList);
    this->basketList->setSpacing(2);
    this->basketList->setAlternatingRowColors(true);

    this->totalLabel = new QLabel{("Basket Total: " + std::to_string(this->userService.getBasketTotal())).c_str()};
    totalLabel->setAlignment(Qt::AlignCenter);
    totalLabel->setFont(QFont{"Verdana", 15});
    basketLayout->addWidget(this->totalLabel);

    this->BasketView = new QTableView{};
    this->BasketView->setModel(&this->model); //FIXME resize according to the size
    this->BasketView->show();

    layout->addWidget(buttonsWidget);
    layout->addWidget(basketWidget);
}

void GUI::connectSignalsAndSlots() {
    this->adminMode ? connectSignalsAndSlotsAdmin() : connectSignalsAndSlotsUser();
    connect(this->exitButton, SIGNAL(clicked()), QApplication::instance(), SLOT(quit()));
}
void GUI::connectSignalsAndSlotsAdmin() {
    //connect buttons
    connect(this->addCoatButton, &QPushButton::released, [this]() {this->addCoat(); this->updateRepoDisplay();});
    connect(this->deleteCoatButton, &QPushButton::released, [this]() {this->deleteCoat(); this->updateRepoDisplay();});
    connect(this->updateCoatButton, &QPushButton::released, [this]() {this->updateCoat(); this->updateRepoDisplay();});
    connect(this->undoButton, &QPushButton::released, [this](){this->undo(); this->updateRepoDisplay();});
    connect(this->redoButton, &QPushButton::released, [this](){this->redo(); this->updateRepoDisplay();});
    connect(this->showStatisticButton, &QPushButton::released, [this](){this->displayStatistics();});

    this->undoShortcut = new QShortcut(QKeySequence("Ctrl+Z"), this);
    connect(undoShortcut, &QShortcut::activated, [this](){this->undoButton->animateClick();});

    this->redoShortcut = new QShortcut(QKeySequence("Ctrl+X"), this);
    connect(redoShortcut, &QShortcut::activated, [this](){this->redoButton->animateClick();});

    this->addShortcut = new QShortcut(QKeySequence("Ctrl+A"), this);
    connect(addShortcut, &QShortcut::activated, [this](){this->addCoatButton->animateClick();});

    this->deleteShortcut = new QShortcut(QKeySequence("Ctrl+D"), this);
    connect(deleteShortcut, &QShortcut::activated, [this](){this->deleteCoatButton->animateClick();});

    this->updateShortcut = new QShortcut(QKeySequence("Ctrl+U"), this);
    connect(updateShortcut, &QShortcut::activated, [this](){this->updateCoatButton->animateClick();});
}
void GUI::connectSignalsAndSlotsUser() {
    //connect buttons
    connect(this->shopButton, &QPushButton::released, [this](){this->shop(); this->updateBasketDisplay();});
    connect(this->openBasketButton, &QPushButton::released, [this](){this->openBasket();});
}

void updateListDisplay(QListWidget* list, std::vector<TrenchCoat>& coats, std::string displayString = "%1\nSize: %2\nColor: %3\nPrice: %4\nStock: %5") {
    if (list->count() > 0)
        list->clear();

    for (const auto& coat: coats) {
        QString coatString;
        if (displayString[displayString.size()-1]=='6')
            coatString = QString::fromStdString(displayString).arg(coat.getName().c_str()).arg(coat.getSize()).arg(coat.getColor().c_str()).arg(coat.getPrice()).arg(coat.getQuantity()).arg(coat.getPhoto().c_str());
        else
            coatString = QString::fromStdString(displayString).arg(coat.getName().c_str()).arg(coat.getSize()).arg(coat.getColor().c_str()).arg(coat.getPrice()).arg(coat.getQuantity());

        auto* coatItem = new QListWidgetItem{coatString};
        coatItem->setFont(QFont{ "Verdana", 15 });
        list->addItem(coatItem);
    }

    list->setMinimumWidth(list->sizeHintForColumn(0)+10);
    list->setMinimumHeight(list->sizeHint().height()+280);
}
void updateDisplayLite(QListWidget* list, std::vector<TrenchCoat>& coats) {
    if (list->count() > 0)
        list->clear();

    for (const auto& coat: coats) {
        QString coatString = QString::fromStdString("%1 - %2 - %3").arg(coat.getName().c_str()).arg(coat.getSize()).arg(coat.getColor().c_str());
        auto* coatItem = new QListWidgetItem{coatString};
        coatItem->setFont(QFont{ "Verdana", 15 });
        list->addItem(coatItem);
    }

    list->setMinimumWidth(list->sizeHintForColumn(0)+10);
    list->setMinimumHeight(list->sizeHint().height()+30);

    if (!coats.empty())
        list->setCurrentRow(0);
}
void GUI::updateRepoDisplay() const {
    auto coats = this->adminService.getAllCoats();
    updateListDisplay(this->trenchCoatsList, coats, "%1\nSize: %2\nColor: %3\nPrice: %4\nStock: %5\nLink: %6");
    //updateDisplayLite(this->trenchCoatsList, coats);
}
void GUI::updateBasketDisplay() const {
    auto coats = this->userService.getBasket();
    updateListDisplay(this->basketList, coats, "%1\nSize: %2\nColor: %3\nPrice: %4\nIn basket: %5");
    this->totalLabel->setText(("Basket Total: " + std::to_string(this->userService.getBasketTotal())).c_str());
}

void GUI::displayBasket() const {
    this->handleErrorMessage("Not implemented yet");
}
void GUI::displayStatistics() const {
    if (!this->adminService.size()) {
        this->handleErrorMessage("No coats in the repository");
        return;
    }

    auto allCoats = this->adminService.getAllCoats();
    QDialog dialog = QDialog{};
    auto dialogLayout = QVBoxLayout{&dialog};
    auto layout = QHBoxLayout{};

    auto generator = StatisticGenerator{allCoats};
    auto barChartItem = generator.itemBarChart();
    auto barChartTotal = generator.totalBarChart();
    auto pieChart = generator.totalPieChart();

    auto chartItemView =  new QtCharts::QChartView(barChartItem);
    auto chartTotalView = new QtCharts::QChartView(barChartTotal);
    auto pieChartView = new QtCharts::QChartView(pieChart);
    layout.addWidget(chartItemView);
    layout.addWidget(chartTotalView);
    dialogLayout.addLayout(&layout);
    dialogLayout.addWidget(pieChartView);
    dialog.exec();
}

std::string GUI::readAttributeType() {
    QDialog dialog; dialog.setWindowTitle("enter attribute");
    QVBoxLayout vLayout{&dialog};

    QLabel title{"Select Attribute to Update"};
    title.setAlignment(Qt::AlignCenter);
    vLayout.addWidget(&title);

    QPushButton priceButton("price");
    QPushButton quantityButton("quantity");
    QPushButton linkButton("link");

    vLayout.addWidget(&priceButton);
    vLayout.addWidget(&quantityButton);
    vLayout.addWidget(&linkButton);

    std::string result;
    connect(&priceButton, &QPushButton::released , &dialog, [&result, &dialog](){result="price";dialog.close();});
    connect(&quantityButton, &QPushButton::released ,[&result, &dialog](){result="quantity";dialog.close();});
    connect(&linkButton, &QPushButton::released ,[&result, &dialog](){result="link";dialog.close();});

    dialog.exec();
    return result;
}
TrenchCoat GUI::readCoatIdentifiers() {
    QDialog dialog; dialog.setWindowTitle("enter coat identifiers");
    QFormLayout form(&dialog);

    QButtonGroup sizeGroup;
    QRadioButton sizeS = QRadioButton("S", &dialog);
    QRadioButton sizeM = QRadioButton("M", &dialog);
    QRadioButton sizeL = QRadioButton("L", &dialog);
    sizeGroup.addButton(&sizeS);
    sizeGroup.addButton(&sizeM);
    sizeGroup.addButton(&sizeL);

    QHBoxLayout sizeLayout;
    sizeLayout.addWidget(&sizeS);
    sizeLayout.addWidget(&sizeM);
    sizeLayout.addWidget(&sizeL);

    QLineEdit nameEdit, colorEdit;
    QLabel nameLabel("Name: "), sizeLabel("Size: "), colorLabel("Color: ");
    nameLabel.setBuddy(&nameEdit);
    colorLabel.setBuddy(&colorEdit);

    form.addRow(&nameLabel, &nameEdit);
    form.addRow(&sizeLabel, &sizeLayout);
    form.addRow(&colorLabel, &colorEdit);

    QDialogButtonBox dialogButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    form.addRow(&dialogButtons);

    connect(&dialogButtons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&dialogButtons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        char size = '#';
        if (sizeS.isChecked())
            size = 'S';
        else if (sizeM.isChecked())
            size = 'M';
        else if (sizeL.isChecked())
            size = 'L';

        TrenchCoat newCoat =  TrenchCoat(nameEdit.text().toStdString(),size,colorEdit.text().toStdString());
        TrenchCoatValidator::validateCoat(newCoat);
        return newCoat;
    }
    throw UIException("Operation cancelled.");
}
std::string GUI::readNewValue(const std::string& attribute) {
    QDialog dialog; dialog.setWindowTitle(("enter " + attribute).c_str());
    QFormLayout form{&dialog};

    QLabel label(attribute.c_str());
    QLineEdit attributeEdit;
    label.setBuddy(&attributeEdit);
    form.addRow(&label, &attributeEdit);

    QDialogButtonBox dialogButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(&dialogButtons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&dialogButtons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    form.addRow(&dialogButtons);

    if (dialog.exec() == QDialog::Accepted) {
        return attributeEdit.text().toStdString();
    }
    throw UIException("Operation cancelled.");
}
TrenchCoat GUI::readValidCoat() {
    QDialog dialog; dialog.setWindowTitle("enter coat");
    QFormLayout form(&dialog);

    QLineEdit nameEdit, colorEdit, linkEdit;
    QSpinBox priceEdit;
    priceEdit.setMinimum(0);
    priceEdit.setMaximum(5000);
    QSpinBox quantityEdit;
    quantityEdit.setMinimum(0);
    quantityEdit.setMaximum(5000);

    QButtonGroup sizeGroup;
    QRadioButton sizeS = QRadioButton("S", &dialog);
    QRadioButton sizeM = QRadioButton("M", &dialog);
    QRadioButton sizeL = QRadioButton("L", &dialog);
    sizeGroup.addButton(&sizeS);
    sizeGroup.addButton(&sizeM);
    sizeGroup.addButton(&sizeL);

    QHBoxLayout sizeLayout;
    sizeLayout.addWidget(&sizeS);
    sizeLayout.addWidget(&sizeM);
    sizeLayout.addWidget(&sizeL);

    QLabel nameLabel("Name: "), sizeLabel("Size: "), colorLabel("Color: "),
    priceLabel("Price: "), quantityLabel("Quantity: "), linkLabel("Link: ");
    nameLabel.setBuddy(&nameEdit);
    colorLabel.setBuddy(&colorEdit);
    priceLabel.setBuddy(&priceEdit);
    quantityLabel.setBuddy(&quantityEdit);
    linkLabel.setBuddy(&linkEdit);

    form.addRow(&nameLabel, &nameEdit);
    form.addRow(&sizeLabel, &sizeLayout);
    form.addRow(&colorLabel, &colorEdit);
    form.addRow(&priceLabel, &priceEdit);
    form.addRow(&quantityLabel, &quantityEdit);
    form.addRow(&linkLabel, &linkEdit);

    QDialogButtonBox dialogButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(&dialogButtons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&dialogButtons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    form.addRow(&dialogButtons);

    if (dialog.exec() == QDialog::Accepted) {
        char size = '#';
        if (sizeS.isChecked())
            size = 'S';
        else if (sizeM.isChecked())
            size = 'M';
        else if (sizeL.isChecked())
            size = 'L';

        TrenchCoat newCoat =  TrenchCoat(
        nameEdit.text().toStdString(),
        size,
        colorEdit.text().toStdString(),
        priceEdit.value(),
        quantityEdit.value(),
        linkEdit.text().toStdString());
        TrenchCoatValidator::validateCoat(newCoat);
        return newCoat;

    }
    throw UIException("Operation cancelled.");
}
char GUI::readSize() const {
    QDialog dialog; dialog.setWindowTitle("Select Size");
    QFormLayout form(&dialog);

    QButtonGroup sizeGroup;
    QRadioButton sizeS = QRadioButton("S", &dialog);
    QRadioButton sizeM = QRadioButton("M", &dialog);
    QRadioButton sizeL = QRadioButton("L", &dialog);
    QRadioButton allSizes = QRadioButton("All Sizes", &dialog);
    sizeGroup.addButton(&sizeS);
    sizeGroup.addButton(&sizeM);
    sizeGroup.addButton(&sizeL);
    sizeGroup.addButton(&allSizes);

    QHBoxLayout sizeLayout;
    sizeLayout.addWidget(&sizeS);
    sizeLayout.addWidget(&sizeM);
    sizeLayout.addWidget(&sizeL);
    sizeLayout.addWidget(&allSizes);

    QDialogButtonBox dialogButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    connect(&dialogButtons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&dialogButtons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    form.addRow("Size: ", &sizeLayout);
    form.addRow(&dialogButtons);

    if (dialog.exec() == QDialog::Accepted) {
        if (sizeS.isChecked())
            return 'S';
        else if (sizeM.isChecked())
            return 'M';
        else if (sizeL.isChecked())
            return 'L';
        else if (allSizes.isChecked())
            return 'X';
        TrenchCoatValidator::validateSize('#'); //just for the exception
    }
    throw UIException("Operation cancelled.");

}

void GUI::runShop(std::vector<TrenchCoat>& validCoats) const {
    int currentCoatIndex = 0;
    TrenchCoat currentCoat = validCoats.at(currentCoatIndex);

    QDialog dialog{};
    QVBoxLayout layout{&dialog};

    //display coat information
    QWidget coatInfo{};
    QVBoxLayout coatLayout{&coatInfo};
    QFormLayout coatInfoLayout{};
    QLabel coatNameLabel, coatSizeLabel, coatColorLabel, coatPriceLabel, coatQuantityLabel;
    QLabel sizeLabel{"Size: "}, colorLabel{"Color: "}, priceLabel{"Price: "}, quantityLabel{"In Stock: "};
    coatNameLabel.setAlignment(Qt::AlignCenter);
    coatNameLabel.setFont(QFont{"Verdana", 20});
    coatNameLabel.setTextFormat(Qt::RichText);
    coatNameLabel.setTextInteractionFlags(Qt::TextBrowserInteraction);
    coatNameLabel.setOpenExternalLinks(true);

    coatInfoLayout.addRow(&sizeLabel, &coatSizeLabel);
    coatInfoLayout.addRow(&colorLabel, &coatColorLabel);
    coatInfoLayout.addRow(&priceLabel, &coatPriceLabel);
    coatInfoLayout.addRow(&quantityLabel, &coatQuantityLabel);

    coatLayout.addWidget(&coatNameLabel);
    coatLayout.addLayout(&coatInfoLayout);

    auto updateCoatQuantity = [&coatQuantityLabel, this](const TrenchCoat& coat) {
        coatQuantityLabel.setText(QString( std::to_string(coat.getQuantity()).c_str()));
    };
    auto setNewCoat = [&coatNameLabel, &coatSizeLabel, &coatColorLabel, &coatPriceLabel, &coatQuantityLabel](const TrenchCoat& coat) {
        std::string clickableName = "<a href=\"" + coat.getPhoto() +"\">" + coat.getName() + "</a>";
        coatNameLabel.setText(QString(clickableName.c_str()));
        coatSizeLabel.setText(QString(coat.getSize()));
        coatColorLabel.setText(QString(coat.getColor().c_str()));
        coatPriceLabel.setText(QString(std::to_string(coat.getPrice()).c_str()));
        coatQuantityLabel.setText(QString(std::to_string(coat.getQuantity()).c_str()));
        //UI::openLink(coat.getPhoto());
    };

    layout.addWidget(&coatInfo);
    setNewCoat(currentCoat);

    //have a previous, next, add to basket and back to menu set of buttons
    QWidget buttonWidget{};
    QHBoxLayout buttonLayout{&buttonWidget};
    QPushButton previousButton{"<", &dialog};
    QPushButton nextButton{">", &dialog};
    QPushButton addButton{"buy", &dialog};
    QPushButton backButton{"back", &dialog};
    buttonLayout.addWidget(&previousButton);
    buttonLayout.addWidget(&nextButton);
    buttonLayout.addWidget(&addButton);
    buttonLayout.addWidget(&backButton);

    connect(&previousButton, &QPushButton::released, [&currentCoatIndex, &currentCoat, &validCoats, &setNewCoat]() {
        currentCoatIndex = (currentCoatIndex - 1 + static_cast<int>(validCoats.size())) % static_cast<int>(validCoats.size());
        currentCoat = validCoats.at(currentCoatIndex);
        setNewCoat(currentCoat);
    });
    connect(&nextButton, &QPushButton::released, [&currentCoatIndex, &currentCoat, &validCoats, &setNewCoat]() {
        currentCoatIndex = (currentCoatIndex + 1) % static_cast<int>(validCoats.size());
        currentCoat = validCoats.at(currentCoatIndex);
        setNewCoat(currentCoat);
    });
    connect(&addButton, &QPushButton::released, [&]() {
        try {
            this->userService.addToBasket(currentCoat);
            validCoats[currentCoatIndex].setQuantity(currentCoat.getQuantity()-1);
            currentCoat = validCoats.at(currentCoatIndex);
            this->handleSuccessfulOperation("Item added to basket successfully!\nTotal: " + std::to_string(this->userService.getBasketTotal()));
        }
        catch (ServiceException& exception) {
            this->handleException(exception);
            return;
        }
        updateCoatQuantity(currentCoat);
        this->model.refresh();
        this->BasketView->resizeColumnsToContents();
    });
    connect(&backButton, &QPushButton::released, &dialog, &QDialog::reject);

    layout.addWidget(&buttonWidget);

    dialog.exec();
}
void GUI::run() {
    this->show();
    QApplication::exec();
}