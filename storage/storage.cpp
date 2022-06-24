#include "storage.h"

#include <algorithm>

Storage::Storage(std::map<QString, Object> &data) : data_(data) {

  const auto getAllObjectKeys = [&]() {
    for (const auto &key : data_) {
      if (std::find(keys_.object_keys_.begin(), keys_.object_keys_.end(),
                    key.first) == keys_.object_keys_.end())
        keys_.object_keys_.emplace_back(key.first);
    }
  };

  const auto getAllValueKeys = [&]() {
    for (const auto &object_key : data_) {
      for (const auto &value_key : data_.at(object_key.first).binary_values_) {
        if (std::find(keys_.value_keys_.begin(), keys_.value_keys_.end(),
                      value_key.first) == keys_.value_keys_.end())
          keys_.value_keys_.emplace_back(value_key.first);
      }

      for (const auto &value_key : data_.at(object_key.first).numeric_values_) {
        if (std::find(keys_.value_keys_.begin(), keys_.value_keys_.end(),
                      value_key.first) == keys_.value_keys_.end())
          keys_.value_keys_.emplace_back(value_key.first);
      }

      for (const auto &value_key : data_.at(object_key.first).string_values_) {
        if (std::find(keys_.value_keys_.begin(), keys_.value_keys_.end(),
                      value_key.first) == keys_.value_keys_.end())
          keys_.value_keys_.emplace_back(value_key.first);
      }
    }
  };

  getAllObjectKeys();
  getAllValueKeys();
}

std::vector<QString> Storage::getAllObjectKeys() const {
  return keys_.object_keys_;
}

std::vector<QString> Storage::getAllValueKeys() const {
  return keys_.value_keys_;
}

bool Storage::setValue(const size_t &object_index, const size_t &value_index,
                       const QVariant &value) {
  const auto object_keys = getAllObjectKeys();
  if (object_index >= object_keys.size())
    return false;

  const auto value_keys = getAllValueKeys();
  if (value_index >= value_keys.size())
    return false;

  try {
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
      throw;
    }
    }
  } catch (...) {
    return false;
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
  if (object_index >= keys_.object_keys_.size() && (object_index < 0))
    return QMetaType::Type::UnknownType;

  if (value_index >= keys_.value_keys_.size() && (value_index < 0))
    return QMetaType::Type::UnknownType;

  if (!data_.contains(keys_.object_keys_.at(object_index)))
    return QMetaType::Type::UnknownType;

  if (data_.at(keys_.object_keys_.at(object_index))
          .binary_values_.contains(keys_.value_keys_.at(value_index)))
    return QMetaType::Type::Bool;

  if (data_.at(keys_.object_keys_.at(object_index))
          .numeric_values_.contains(keys_.value_keys_.at(value_index)))
    return QMetaType::Type::Double;

  if (data_.at(keys_.object_keys_.at(object_index))
          .string_values_.contains(keys_.value_keys_.at(value_index)))
    return QMetaType::Type::QString;

  return QMetaType::Type::UnknownType;
}
