#include "backend.h"
#include <QRandomGenerator>
#include <QVector>

CellItem::CellItem(const QString &type, QString status, int mineCount, QObject *parent) : QObject(parent),
    m_type(type), m_status(status), m_mineCount(mineCount)
{

}

CellModel::CellModel(QObject *parent) : QAbstractTableModel(parent)
{


}

int CellModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_cellItems.count();
}

int CellModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if (!m_cellItems.isEmpty())
        return m_cellItems.first().count();
    return 0;
}


QVariant CellModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_cellItems.count() || index.column() >= m_cellItems.first().count())
        return QVariant();

    CellItem *cellItem = m_cellItems[index.row()][index.column()];
    if (role == TypeRole) {
        if (cellItem)
            return cellItem->type();
        else
            return QString();
    }
    else if (role == StatusRole) {
        if (cellItem)
            return cellItem->status();
        else
            return QString();
    }
    else if (role == MineCountRole) {
        if (cellItem)
            return cellItem->mineCount();
        else
            return QString();
    }

    return QVariant();
}

bool CellModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || index.row() >= m_cellItems.count() || index.column() >= m_cellItems.first().count())
        return false;

    if (role == TypeRole)
        m_cellItems[index.row()][index.column()]->setType(value.toString());
    else if (role == StatusRole)
        m_cellItems[index.row()][index.column()]->setStatus(value.toString());
    else if (role == MineCountRole)
        m_cellItems[index.row()][index.column()]->setMineCount(value.toInt());

    emit dataChanged(index, index, {role});
    return true;
}

QHash<int, QByteArray> CellModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[TypeRole] = "type";
    roles[StatusRole] = "status";
    roles[MineCountRole] = "mineCount";
    return roles;
}

QString CellItem::type() const
{
    return m_type;
}

QString CellItem::status() const
{
    return m_status;
}

int CellItem::mineCount() const
{
    return m_mineCount;
}

void CellItem::setType(const QString &newType)
{
    m_type = newType;
}
void CellItem::setStatus(const QString &newStatus)
{
    m_status = newStatus;
}
void CellItem::setMineCount (int newMineCount)
{
    m_mineCount = newMineCount;
}
void CellModel::addItem(int width, int height)
{
    beginResetModel();
    m_cellItems.clear();

    for (int i = 0; i < width; i++)
    {
        QList<CellItem*> rowItems;
        for (int j = 0; j < height; j++)
        {
            CellItem *cell = new CellItem("clean", "unchecked", 0, this);
            rowItems.append(cell);
        }
        m_cellItems.append(rowItems);
    }

    // Set 'difficulty' number of mines randomly
    int totalCells = width * height;
    for (int j = 0; j < totalMine; j++)
    {
        int randMine = QRandomGenerator::global()->bounded(0, totalCells);
        int row = randMine / height;
        int column = randMine % height;
        m_cellItems[row][column]->setType("mine");
    }

    endResetModel();
}

QVector<QPair<int, int>> CellModel:: getAdjacentCells(int row, int col, int rowCount, int colCount)

{
    QVector<QPair<int, int>> neighbors;

        // Define the offsets for the neighboring cells
        int offsets[8][2] = {
            {-1, -1}, {-1, 0}, {-1, 1},
            {0, -1},           {0, 1},
            {1, -1}, {1, 0}, {1, 1}
        };

        // Iterate through the offsets and calculate the coordinates of the neighbors
        for (int i = 0; i < 8; i++) {
            int neighborRow = row + offsets[i][0];
            int neighborCol = col + offsets[i][1];

            // Check if the neighbor coordinates are within the bounds of the array
            if (neighborRow >= 0 && neighborRow < rowCount &&
                neighborCol >= 0 && neighborCol < colCount)
            {
                neighbors.append(QPair<int, int>(neighborRow, neighborCol));
            }
        }

        return neighbors;

}


bool CellModel::unCheckingCells(int row, int col, int rowCount, int colCount)
{
    // Get the current cell
    CellItem* cell = m_cellItems[row][col];

    // If the cell is a mine, return false (game over)
    if (cell->type() == "mine")
    {
        return false;
    }
    else
    {
        // Set the cell as checked
        beginResetModel();
        cell->setStatus("checked");
        endResetModel();
        // Get the adjacent cells
        QVector<QPair<int, int>> neighbors = getAdjacentCells(row, col, rowCount, colCount);

        // Iterate through the neighbors
        for (int i = 0; i < neighbors.count(); i++)
        {
            int nRow = neighbors.at(i).first;
            int nCol = neighbors.at(i).second;

            // Get the neighbor cell
            CellItem* neighborItem = m_cellItems[nRow][nCol];


            // Recursive call to unCheckingCells for unchecked and non-mine neighbors
            if (neighborItem && neighborItem->type() != "mine" && neighborItem->status() != "checked")
            {
                unCheckingCells(nRow, nCol, rowCount, colCount);
            }

            else {
                return false;
            }


        }

        QModelIndex topLeft = index(0, 0);
        QModelIndex bottomRight = index(rowCount - 1, colCount - 1);
        emit dataChanged(topLeft, bottomRight, {StatusRole});

        return true;
    }
}

void CellModel::adjacentMine( int rowCount, int colCount)
{

   for (int i = 0; i< rowCount; i++){
       for (int j = 0; j< colCount; j++){
           CellItem* cell = m_cellItems[i][j];

             QVector<QPair<int, int>> neighbors = getAdjacentCells(i, j, rowCount, colCount);

             mCount = 0;

             if (cell->type() != "mine") {
                 for (int n = 0; n < neighbors.count(); n++){

                     int nRow = neighbors.at(n).first;
                     int nCol = neighbors.at(n).second;
                     CellItem* neighborItem = m_cellItems[nRow][nCol];



                     if ( neighborItem->type() == "mine"){
                         mCount++;
                     }

                     beginResetModel();
                     cell->setMineCount(mCount);
                     endResetModel();

                 }

             }


       }
   }
}

bool CellModel::checkWin( int rowCount, int colCount)
{

    int count = 0;
    int mineCount = 0;
    int flagCount = 0;

    for (int i=0; i< rowCount; i++)
    {
        for (int j=0; j< colCount; j++)
        {
            CellItem* cell = m_cellItems[i][j];

            if (cell->type() == "mine"){
                mineCount++;
            }

            if ( cell->status() == "flag"){
                flagCount++;
            }

            if (cell->type() == "mine" && cell->status() == "flag"){
                count++;
            }
        }
    }

    qDebug()<< count << mineCount;

    if (count == mineCount && flagCount == mineCount)
        return true;
    else
        return false;

}



//bool CellModel::unCheckingCells(int row, int col, int rowCount, int colCount)
//{
//    CellItem* cell = m_cellItems[row][col];

//    if (cell->type() == "mine") {
//        // Game over, mine is clicked
//        return false;
//    } else if (cell->status() == "checked") {
//        // Cell already checked, no need to continue
//        return true;
//    } else {
//        // Uncheck the cell and update its status
//        beginResetModel();

//        cell->setStatus("checked");
//        bug()<< "cke";
//        endResetModel();
//        // Get the adjacent cells
//        QVector<QPair<int, int>> neighbors = getAdjacentCells(row, col, rowCount, colCount);

//        // Recursively uncheck neighboring cells
//        for (int i = 0; i < neighbors.count(); i++) {
//            int nRow = neighbors.at(i).first;
//            int nCol = neighbors.at(i).second;
//            if (unCheckingCells(nRow, nCol, rowCount, colCount) == false) {
//                // Game over, propagate the false return value
//                return false;
//            }
//        }

//        // All neighboring cells are successfully uncovered
//        return true;
//    }
//}


