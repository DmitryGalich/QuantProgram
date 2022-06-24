#include "storage.h"

#include <algorithm>

Storage::Storage(std::map<QString, Object> &data) : data_(data) {}

std::vector<QString> Storage::getAllObjectKeys() const {
  std::vector<QString> keys;

  for (const auto &key : data_) {
    if (std::find(keys.begin(), keys.end(), key.first) == keys.end())
      keys.emplace_back(key.first);
  }

  return keys;
}

std::vector<QString> Storage::getAllValueKeys() const {
  std::vector<QString> keys;

  for (const auto &object_key : data_) {
    for (const auto &value_key : data_.at(object_key.first).binary_values_) {
      if (std::find(keys.begin(), keys.end(), value_key.first) == keys.end())
        keys.emplace_back(value_key.first);
    }

    for (const auto &value_key : data_.at(object_key.first).numeric_values_) {
      if (std::find(keys.begin(), keys.end(), value_key.first) == keys.end())
        keys.emplace_back(value_key.first);
    }

    for (const auto &value_key : data_.at(object_key.first).string_values_) {
      if (std::find(keys.begin(), keys.end(), value_key.first) == keys.end())
        keys.emplace_back(value_key.first);
    }
  }

  return keys;
}

bool Storage::setValue(const size_t &object_index, const size_t &value_index,
                       const QVariant &value) {
  const auto object_keys = getAllObjectKeys();
  if (object_index >= object_keys.size())
    return false;

  const auto value_keys = getAllValueKeys();
  if (value_index >= value_keys.size())
    return false;

  switch (getValueType(object_index, value_index)) {
  case (QMetaType::Bool): {
    data_.at(object_keys.at(object_index))
        .binary_values_.at(value_keys.at(value_index))
        .reset(new bool(value.toBool()));
    break;
  }
  case (QMetaType::Double): {
    data_.at(object_keys.at(object_index))
        .numeric_values_.at(value_keys.at(value_index))
        .reset(new double(value.toDouble()));
    break;
  }
  case (QMetaType::QString): {
    data_.at(object_keys.at(object_index))
        .string_values_.at(value_keys.at(value_index))
        .reset(new QString(value.toString()));
    break;
  }
  default: {
    return false;
  }
  }

  return true;
}

Storage::Status Storage::getValue(const size_t &object_index,
                                  const size_t &value_index,
                                  QVariant &value) const {
  const auto object_keys = getAllObjectKeys();
  if (object_index >= object_keys.size())
    return {false, false, false};

  if (!data_.contains(object_keys.at(object_index)))
    return {false, false, false};

  const auto value_keys = getAllValueKeys();
  if (value_index >= value_keys.size())
    return {true, false, false};

  // Binary
  if (data_.at(object_keys.at(object_index))
          .binary_values_.contains(value_keys.at(value_index))) {

    if (!data_.at(object_keys.at(object_index))
             .binary_values_.at(value_keys.at(value_index)))
      return {true, true, false};

    value = *data_.at(object_keys.at(object_index))
                 .binary_values_.at(value_keys.at(value_index));
    return {true, true, true};
  }

  // Numeric
  if (data_.at(object_keys.at(object_index))
          .numeric_values_.contains(value_keys.at(value_index))) {

    if (!data_.at(object_keys.at(object_index))
             .numeric_values_.at(value_keys.at(value_index)))
      return {true, true, false};

    value = *data_.at(object_keys.at(object_index))
                 .numeric_values_.at(value_keys.at(value_index));
    return {true, true, true};
  }

  // String
  if (data_.at(object_keys.at(object_index))
          .string_values_.contains(value_keys.at(value_index))) {

    if (!data_.at(object_keys.at(object_index))
             .string_values_.at(value_keys.at(value_index)))
      return {true, true, false};

    value = *data_.at(object_keys.at(object_index))
                 .string_values_.at(value_keys.at(value_index));
    return {true, true, true};
  }

  return {true, false, false};
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
