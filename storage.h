#pragma once

#include <map>
#include <memory>
#include <set>
#include <vector>

#include <QMetaType>
#include <QString>

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

  Storage() = delete;
  Storage(std::map<QString, Object> &data);
  ~Storage() = default;

  void setValue(const QString &object_key, const QString &value_key,
                const bool value);
  void setValue(const QString &object_key, const QString &value_key,
                const double value);
  void setValue(const QString &object_key, const QString &value_key,
                const QString &value);

  Status getBinaryValue(const QString &object_key, const QString &value_key,
                        bool &value) const;
  Status getNumericValue(const QString &object_key, const QString &value_key,
                         double &value) const;
  Status getStringValue(const QString &object_key, const QString &value_key,
                        QString &value) const;

  std::set<QString> getAllObjectKeys() const;
  std::set<QString> getAllValueKeys() const;

  QMetaType::Type getValueType(const QString &object_key,
                               const QString &value_key) const;

  QMetaType::Type getValueType(const size_t &object_index,
                               const size_t &value_index) const;

private:
  std::map<QString, Object> data_;
};
