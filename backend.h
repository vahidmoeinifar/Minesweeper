#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QAbstractItemModel>
#include <qqml.h>

class CellItem : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    QString m_type;
    QString m_status;
    int m_mineCount;


public:
    explicit CellItem(const QString &type, QString status, int mineCount, QObject *parent = nullptr);
    QString type() const;
    QString status() const;
    int mineCount() const;
    void setType(const QString &type);
    void setStatus(const QString &status);
    void setMineCount(const int minCount);


signals:
    void typeChanged();
    void statusChanged();
    void mineCountChanged();
};

class CellModel : public QAbstractTableModel
{
    Q_OBJECT
    QML_ELEMENT

    QList<QList<CellItem*>> m_cellItems;
    int mCount = 0;
    int totalMine = 33;

public:
    explicit CellModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_INVOKABLE QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Q_INVOKABLE bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    Q_INVOKABLE void addItem(int width, int height);
    Q_INVOKABLE QVector<QPair<int, int>> getAdjacentCells(int row, int col, int rowCount, int colCount);
    Q_INVOKABLE bool unCheckingCells(int row, int col, int rowCount, int colCount);
    Q_INVOKABLE void adjacentMine(int rowCount, int colCount);
    Q_INVOKABLE bool checkWin(int rowCount, int colCount);


    enum ItemRoles {
        TypeRole,
        StatusRole,
        MineCountRole
    };




protected:
    QHash<int, QByteArray> roleNames() const override;
};

#endif // BACKEND_H
