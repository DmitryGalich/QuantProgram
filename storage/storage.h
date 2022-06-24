#pragma once

#include <map>
#include <memory>
#include <set>
#include <vector>

#include <QMetaType>
#include <QString>
#include <QVariant>

class Storage {
public:
  struct Object {
    std::map<QString, std::shared_ptr<bool>> binary_values_;
    std::map<QString, std::shared_ptr<double>> numeric_values_;
    std::map<QString, std::shared_ptr<QString>> string_values_;
  };

  struct Status {
    bool is_object_exists_{false};
    bool is_value_exists_{false};
    bool is_value_set_{false};
  };

  struct Keys {
    std::vector<QString> object_keys_;
    std::vector<QString> value_keys_;
  };

  Storage() = delete;
  Storage(std::map<QString, Object> &data);
  ~Storage() = default;

  std::vector<QString> getAllObjectKeys() const;
  std::vector<QString> getAllValueKeys() const;

  bool setValue(const size_t &object_index, const size_t &value_index,
                const QVariant &value);

  Status getValue(const size_t &object_index, const size_t &value_index,
                  QVariant &value) const;

  QMetaType::Type getValueType(const size_t &object_index,
                               const size_t &value_index) const;

private:
  std::map<QString, Object> data_;
  Keys keys_;
};
