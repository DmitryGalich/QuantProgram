#include "table_model.h"

#include <iostream>

TableModel::TableModel(Storage &storage, QObject *parent)
    : QAbstractTableModel(parent), storage_(storage) {

  object_keys_ = storage_.getAllObjectKeys();
  value_keys_ = storage_.getAllValueKeys();
}

int TableModel::rowCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);
  return value_keys_.size();
}

int TableModel::columnCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);
  return object_keys_.size();
}

QVariant TableModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid() || role != Qt::DisplayRole)
    return QVariant();

  //  switch (storage_.getValueType(index.column(), index.row())) {
  //  case QMetaType::Type::Bool: {
  //    QVariant value(false);

  //    auto value_status = storage_.getValue(index.column(), index.row(),
  //    value); if (value_status.is_object_exists_ &&
  //    value_status.is_value_exists_ &&
  //        value_status.is_value_set_)
  //      return value;

  //    break;
  //  }
  //  case QMetaType::Type::Double: {
  //    break;
  //  }
  //  case QMetaType::Type::QString: {
  //    break;
  //  }
  //  default:
  //    break;
  //  }

  QVariant value;

  auto value_status = storage_.getValue(index.column(), index.row(), value);
  if (value_status.is_object_exists_ && value_status.is_value_exists_ &&
      value_status.is_value_set_)
    return value;

  return {};
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value,
                         int role) {
  if (index.isValid() && role == Qt::EditRole) {

    if (!storage_.setValue(index.column(), index.row(), value))
      return false;

    emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});
    return true;
  }

  return false;
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation,
                                int role) const {
  if (role == Qt::DisplayRole) {
    if (orientation == Qt::Horizontal) {
      if ((section >= static_cast<int>(object_keys_.size())) || (section < 0))
        return QVariant();

      return object_keys_.at(section);

    } else if (orientation == Qt::Vertical) {
      if ((section >= static_cast<int>(value_keys_.size())) || (section < 0))
        return QVariant();

      return value_keys_.at(section);
    }
  }

  return QVariant();
}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const {
  if (!index.isValid())
    return Qt::ItemIsEnabled;

  QVariant value;
  auto value_status = storage_.getValue(index.column(), index.row(), value);
  if (!value_status.is_object_exists_ || !value_status.is_value_exists_)
    return Qt::ItemIsEnabled;

  return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}
