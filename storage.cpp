#include "storage.h"

#include <algorithm>

Storage::Storage(std::map<QString, Object> &data) : data_(data) {}

void Storage::setValue(const QString &object_key, const QString &value_key,
                       const bool value) {
  if (!data_.contains(object_key))
    return;

  if (!data_.at(object_key).binary_values_.contains(value_key))
    return;

  data_.at(object_key).binary_values_.at(value_key).reset(new bool(value));
}

void Storage::setValue(const QString &object_key, const QString &value_key,
                       const double value) {
  if (!data_.contains(object_key))
    return;

  if (!data_.at(object_key).numeric_values_.contains(value_key))
    return;

  data_.at(object_key).numeric_values_.at(value_key).reset(new double(value));
}

void Storage::setValue(const QString &object_key, const QString &value_key,
                       const QString &value) {
  if (!data_.contains(object_key))
    return;

  if (!data_.at(object_key).string_values_.contains(value_key))
    return;

  data_.at(object_key).string_values_.at(value_key).reset(new QString(value));
}

Storage::Status Storage::getBinaryValue(const QString &object_key,
                                        const QString &value_key,
                                        bool &value) const {
  if (!data_.contains(object_key))
    return {false, false, false};

  if (!data_.at(object_key).binary_values_.contains(value_key))
    return {true, false, false};

  if (!data_.at(object_key).binary_values_.at(value_key))
    return {true, true, false};

  value = *data_.at(object_key).binary_values_.at(value_key);
  return {true, true, true};
}

Storage::Status Storage::getNumericValue(const QString &object_key,
                                         const QString &value_key,
                                         double &value) const {
  if (!data_.contains(object_key))
    return {false, false, false};

  if (!data_.at(object_key).numeric_values_.contains(value_key))
    return {true, false, false};

  if (!data_.at(object_key).numeric_values_.at(value_key))
    return {true, true, false};

  value = *data_.at(object_key).numeric_values_.at(value_key);
  return {true, true, true};
}

Storage::Status Storage::getStringValue(const QString &object_key,
                                        const QString &value_key,
                                        QString &value) const {
  if (!data_.contains(object_key))
    return {false, false, false};

  if (!data_.at(object_key).string_values_.contains(value_key))
    return {true, false, false};

  if (!data_.at(object_key).string_values_.at(value_key))
    return {true, true, false};

  value = *data_.at(object_key).string_values_.at(value_key);
  return {true, true, true};
}

std::set<QString> Storage::getAllObjectKeys() const {
  std::set<QString> keys;

  for (const auto &key : data_)
    keys.insert(key.first);

  return keys;
}

std::set<QString> Storage::getAllValueKeys() const {
  std::set<QString> keys;

  for (const auto &object_key : data_) {
    for (const auto &value_key : data_.at(object_key.first).binary_values_)
      keys.insert(value_key.first);

    for (const auto &value_key : data_.at(object_key.first).numeric_values_)
      keys.insert(value_key.first);

    for (const auto &value_key : data_.at(object_key.first).string_values_)
      keys.insert(value_key.first);
  }

  return keys;
}

QMetaType::Type Storage::getValueType(const QString &object_key,
                                      const QString &value_key) const {

  {
    bool value(false);
    auto status = getBinaryValue(object_key, value_key, value);
    if (status.is_object_exists_ && status.is_value_exists_)
      return QMetaType::Type::Bool;
  }

  {
    double value(0.0);
    auto status = getNumericValue(object_key, value_key, value);
    if (status.is_object_exists_ && status.is_value_exists_)
      return QMetaType::Type::Double;
  }

  {
    QString value;
    auto status = getStringValue(object_key, value_key, value);
    if (status.is_object_exists_ && status.is_value_exists_)
      return QMetaType::Type::QString;
  }

  return QMetaType::Type::UnknownType;
}

QMetaType::Type Storage::getValueType(const size_t &object_index,
                                      const size_t &value_index) const {

  if (object_index >= data_.size())
    return QMetaType::Type::UnknownType;

  size_t object_i = 0;
  for (const auto &object : data_) {
    if (object_i < object_index) {
      object_i++;
      continue;
    }

    if (value_index < object.second.binary_values_.size())
      return QMetaType::Type::Bool;
    else if (value_index >= object.second.binary_values_.size() &&
             value_index < (object.second.binary_values_.size() +
                            object.second.numeric_values_.size()))
      return QMetaType::Type::Double;
    else if (value_index >= (object.second.binary_values_.size() +
                             object.second.numeric_values_.size()) &&
             value_index < (object.second.binary_values_.size() +
                            object.second.numeric_values_.size() +
                            object.second.string_values_.size()))
      return QMetaType::Type::QString;

    break;
  }

  return QMetaType::Type::UnknownType;
}
