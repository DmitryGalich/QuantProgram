#pragma once

#include <QAbstractTableModel>
#include <QMouseEvent>

#include "storage/storage.h"

class TableModel : public QAbstractTableModel {
  Q_OBJECT

public:
  TableModel(Storage &storage, QObject *parent = 0);

  int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
  int columnCount(const QModelIndex &parent = QModelIndex()) const
      Q_DECL_OVERRIDE;

  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
  bool setData(const QModelIndex &index, const QVariant &value,
               int role = Qt::EditRole) Q_DECL_OVERRIDE;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
  Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;

private:
  std::vector<QString> object_keys_;
  std::vector<QString> value_keys_;

  Storage &storage_;
};
