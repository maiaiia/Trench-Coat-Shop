#include "BasketModel.h"
#include <QFont>

BasketModel::BasketModel(FileBasket& basket, QObject *parent):
    QAbstractTableModel(parent), basket_{basket} {}

QVariant BasketModel::headerData(int section, Qt::Orientation orientation, int role) const {
    //name, size, color, price, quantity
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            switch (section) {
                case 0:
                    return QString{"Name"};
                case 1:
                    return QString{"Size"};
                case 2:
                    return QString{"Color"};
                case 3:
                    return QString{"Price"};
                case 4:
                    return QString{"Quantity"};
                default:
                    break;
            }
        }
    }
    return QVariant{};
}

int BasketModel::columnCount(const QModelIndex &parent) const {
    return 5;
}

int BasketModel::rowCount(const QModelIndex &parent) const {
    return this->basket_.size();
}

QVariant BasketModel::data(const QModelIndex &index, int role) const {
    int row = index.row(), column = index.column();
    auto coat = this->basket_.getCoat(row);
    if (role == Qt::DisplayRole) {
        switch (column) {
            case 0:
                return QString::fromStdString(coat.getName());
            case 1:
                return QString{coat.getSize()};
            case 2:
                return QString::fromStdString(coat.getColor());
            case 3:
                return QString::fromStdString(std::to_string(coat.getPrice()));
            case 4:
                return QString::fromStdString(std::to_string(coat.getQuantity()));
            default:
                break;
        }
    }
    return QVariant{};
}

void BasketModel::refresh() {
    beginResetModel();
    endResetModel();
}
