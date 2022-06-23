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

  return getValueByIndex(index);
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value,
                         int role) {
  if (index.isValid() && role == Qt::EditRole) {

    std::cout << value.toString().toStdString() << std::endl;

    switch (storage_.getValueType(index.column(), index.row())) {
    case (QMetaType::Bool): {
      //        storage_.setValue();
      break;
    }
    case (QMetaType::Double): {
      break;
    }
    case (QMetaType::QString): {
      break;
    }
    default: {
      return false;
    }
    }
    //    const auto keys = getKeysByIndex(index);
    //    storage_.setValue(keys.at(0), keys.at(1), value);

    //    const int row = index.row();
    //    auto contact = contacts.value(row);

    //    switch (index.column()) {
    //    case 0:
    //      contact.name = value.toString();
    //      break;
    //    case 1:
    //      contact.address = value.toString();
    //      break;
    //    default:
    //      return false;
    //    }
    //    contacts.replace(row, contact);
    emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});
    return true;
  }

  return false;
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation,
                                int role) const {
  if (role == Qt::DisplayRole) {
    if (orientation == Qt::Horizontal) {
      auto front = object_keys_.begin();
      std::advance(front, section);
      return *front;

    } else if (orientation == Qt::Vertical) {
      auto front = value_keys_.begin();
      std::advance(front, section);
      return *front;
    }
  }

  return QVariant();
}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const {
  if (!index.isValid())
    return Qt::ItemIsEnabled;

  //  if (getValueByIndex(index).isEmpty())
  //    return Qt::ItemIsEnabled;

  return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

QVector<QString> TableModel::getKeysByIndex(const QModelIndex &index) const {
  auto objects_iter = object_keys_.begin();
  std::advance(objects_iter, index.column());

  auto values_iter = value_keys_.begin();
  std::advance(values_iter, index.row());

  return {*objects_iter, *values_iter};
}

QVariant TableModel::getValueByIndex(const QModelIndex &index) const {
  const auto keys = getKeysByIndex(index);

  {
    bool value = false;
    auto status = storage_.getBinaryValue(keys.at(0), keys.at(1), value);
    if (status.is_object_exists_ && status.is_value_exists_) {
      if (status.is_value_set_)
        return value;
      else
        return {"Null_bool"};
    }
  }

  {
    double value = 0.0;
    auto status = storage_.getNumericValue(keys.at(0), keys.at(1), value);
    if (status.is_object_exists_ && status.is_value_exists_) {
      if (status.is_value_set_)
        return value;
      else
        return {"Null_double"};
    }
  }

  {
    QString value;
    auto status = storage_.getStringValue(keys.at(0), keys.at(1), value);
    if (status.is_object_exists_ && status.is_value_exists_) {
      if (status.is_value_set_)
        return value;
      else
        return {"Null_QString"};
    }
  }

  return {};
}
