#pragma once
#include <qabstractitemmodel.h>
#include <FileBasket.h>

class BasketModel: public QAbstractTableModel {
private:
    FileBasket& basket_;
public:
    explicit BasketModel(FileBasket& basket, QObject* parent = nullptr);
    [[nodiscard]] int rowCount(const QModelIndex &parent) const override;
    [[nodiscard]] int columnCount(const QModelIndex &parent) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
    [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    void refresh();
signals:
    void itemAdded();
};
