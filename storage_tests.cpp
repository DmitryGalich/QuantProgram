#include "storage.h"
#include "gtest/gtest.h"

#include <iostream>

class StorageTest : public ::testing::Test {
protected:
  void SetUp() {
    std::map<QString, Storage::Object> data;

    Storage::Object object;
    object.binary_values_.insert({"key_0", std::make_shared<bool>(true)});
    object.binary_values_.insert({"key_1", nullptr});

    object.numeric_values_.insert({"key_2", nullptr});
    object.numeric_values_.insert({"key_3", std::make_unique<double>(9.9)});

    object.string_values_.insert({"key_4", nullptr});
    object.string_values_.insert(
        {"key_5", std::make_shared<QString>("object_string")});

    data.insert({"object_0", object});
    data.insert({"object_9", object});

    storage_ = std::make_shared<Storage>(data);
  }
  void TearDown() { storage_.reset(); }

  std::shared_ptr<Storage> storage_;
};

TEST_F(StorageTest, getObjectKeys) {
  EXPECT_EQ(storage_->getAllObjectKeys().size(), 2);
}

TEST_F(StorageTest, getValueKeys) {
  EXPECT_EQ(storage_->getAllValueKeys().size(), 6);
}

TEST_F(StorageTest, getBinaryValueSuccess) {
  QVariant value(false);

  auto value_status = storage_->getValue(0, 0, value);
  EXPECT_TRUE(value_status.is_object_exists_);
  EXPECT_TRUE(value_status.is_value_exists_);
  EXPECT_TRUE(value_status.is_value_set_);
  EXPECT_TRUE(value.toBool());
}

TEST_F(StorageTest, getBinaryValueUnsetValue) {
  QVariant value(false);

  auto value_status = storage_->getValue(0, 1, value);
  EXPECT_TRUE(value_status.is_object_exists_);
  EXPECT_TRUE(value_status.is_value_exists_);
  EXPECT_FALSE(value_status.is_value_set_);
}

TEST_F(StorageTest, getBinaryValueUnexistingObject) {
  QVariant value(false);

  auto value_status = storage_->getValue(9999, 0, value);
  EXPECT_FALSE(value_status.is_object_exists_);
}

TEST_F(StorageTest, getBinaryValueUnexistingValue) {
  QVariant value(false);

  auto value_status = storage_->getValue(1, 9999, value);
  EXPECT_TRUE(value_status.is_object_exists_);
  EXPECT_FALSE(value_status.is_value_exists_);
}

TEST_F(StorageTest, getNumericValueSuccess) {
  QVariant value(0.0);

  auto value_status = storage_->getValue(0, 3, value);
  EXPECT_TRUE(value_status.is_object_exists_);
  EXPECT_TRUE(value_status.is_value_exists_);
  EXPECT_TRUE(value_status.is_value_set_);
  EXPECT_EQ(value.toDouble(), 9.9);
}

TEST_F(StorageTest, getNumericValueUnsetValue) {
  QVariant value(0.0);

  auto value_status = storage_->getValue(0, 2, value);
  EXPECT_TRUE(value_status.is_object_exists_);
  EXPECT_TRUE(value_status.is_value_exists_);
  EXPECT_FALSE(value_status.is_value_set_);
}

TEST_F(StorageTest, getNumericValueUnexistingObject) {
  QVariant value(0.0);

  auto value_status = storage_->getValue(9999, 0, value);
  EXPECT_FALSE(value_status.is_object_exists_);
}

TEST_F(StorageTest, getNumericValueUnexistingValue) {
  QVariant value(0.0);

  auto value_status = storage_->getValue(1, 9999, value);
  EXPECT_TRUE(value_status.is_object_exists_);
  EXPECT_FALSE(value_status.is_value_exists_);
}

TEST_F(StorageTest, getStringValueSuccess) {
  QVariant value("");

  auto value_status = storage_->getValue(0, 5, value);
  EXPECT_TRUE(value_status.is_object_exists_);
  EXPECT_TRUE(value_status.is_value_exists_);
  EXPECT_TRUE(value_status.is_value_set_);
  EXPECT_EQ(value.toString(), "object_string");
}

TEST_F(StorageTest, getStringValueUnsetValue) {
  QVariant value("");

  auto value_status = storage_->getValue(0, 4, value);
  EXPECT_TRUE(value_status.is_object_exists_);
  EXPECT_TRUE(value_status.is_value_exists_);
  EXPECT_FALSE(value_status.is_value_set_);
}

TEST_F(StorageTest, getStringValueUnexistingObject) {
  QVariant value("");

  auto value_status = storage_->getValue(9999, 0, value);
  EXPECT_FALSE(value_status.is_object_exists_);
}

TEST_F(StorageTest, getStringValueUnexistingValue) {
  QVariant value("");

  auto value_status = storage_->getValue(1, 9999, value);
  EXPECT_TRUE(value_status.is_object_exists_);
  EXPECT_FALSE(value_status.is_value_exists_);
}

TEST_F(StorageTest, setBinaryValueSuccessWithUnsetValue) {
  const size_t kObjectIndex(0);
  const size_t kValueIndex(1);
  const bool kNecessaryValue(true);

  QVariant value(false);

  auto value_status = storage_->getValue(kObjectIndex, kValueIndex, value);
  EXPECT_TRUE(value_status.is_object_exists_);
  EXPECT_TRUE(value_status.is_value_exists_);
  EXPECT_FALSE(value_status.is_value_set_);

  storage_->setValue(kObjectIndex, kValueIndex, kNecessaryValue);

  value_status = storage_->getValue(kObjectIndex, kValueIndex, value);
  EXPECT_TRUE(value_status.is_object_exists_);
  EXPECT_TRUE(value_status.is_value_exists_);
  EXPECT_TRUE(value_status.is_value_set_);
  EXPECT_EQ(kNecessaryValue, value);
}

TEST_F(StorageTest, setBinaryValueSuccessWithSetValue) {
  const size_t kObjectIndex(0);
  const size_t kValueIndex(0);
  const bool kNecessaryValue(true);

  QVariant value(false);

  auto value_status = storage_->getValue(kObjectIndex, kValueIndex, value);
  EXPECT_TRUE(value_status.is_object_exists_);
  EXPECT_TRUE(value_status.is_value_exists_);
  EXPECT_TRUE(value_status.is_value_set_);

  storage_->setValue(kObjectIndex, kValueIndex, kNecessaryValue);

  value_status = storage_->getValue(kObjectIndex, kValueIndex, value);
  EXPECT_TRUE(value_status.is_object_exists_);
  EXPECT_TRUE(value_status.is_value_exists_);
  EXPECT_TRUE(value_status.is_value_set_);
  EXPECT_EQ(kNecessaryValue, value);
}

TEST_F(StorageTest, setNumericValueSuccessWithUnsetValue) {
  const size_t kObjectIndex(0);
  const size_t kValueIndex(2);
  const double kNecessaryValue(42);

  QVariant value(0.0);

  auto value_status = storage_->getValue(kObjectIndex, kValueIndex, value);
  EXPECT_TRUE(value_status.is_object_exists_);
  EXPECT_TRUE(value_status.is_value_exists_);
  EXPECT_FALSE(value_status.is_value_set_);

  storage_->setValue(kObjectIndex, kValueIndex, kNecessaryValue);

  value_status = storage_->getValue(kObjectIndex, kValueIndex, value);
  EXPECT_TRUE(value_status.is_object_exists_);
  EXPECT_TRUE(value_status.is_value_exists_);
  EXPECT_TRUE(value_status.is_value_set_);
  EXPECT_EQ(kNecessaryValue, value.toDouble());
}

TEST_F(StorageTest, setNumericValueSuccessWithSetValue) {
  const size_t kObjectIndex(0);
  const size_t kValueIndex(3);
  const double kNecessaryValue(42);

  QVariant value(0.0);

  auto value_status = storage_->getValue(kObjectIndex, kValueIndex, value);
  EXPECT_TRUE(value_status.is_object_exists_);
  EXPECT_TRUE(value_status.is_value_exists_);
  EXPECT_TRUE(value_status.is_value_set_);

  storage_->setValue(kObjectIndex, kValueIndex, kNecessaryValue);

  value_status = storage_->getValue(kObjectIndex, kValueIndex, value);
  EXPECT_TRUE(value_status.is_object_exists_);
  EXPECT_TRUE(value_status.is_value_exists_);
  EXPECT_TRUE(value_status.is_value_set_);
  EXPECT_EQ(kNecessaryValue, value.toDouble());
}

TEST_F(StorageTest, setSrtingValueSuccessWithUnsetValue) {
  const size_t kObjectIndex(0);
  const size_t kValueIndex(4);
  const QString kNecessaryValue("hesoyam");

  QVariant value("");

  auto value_status = storage_->getValue(kObjectIndex, kValueIndex, value);
  EXPECT_TRUE(value_status.is_object_exists_);
  EXPECT_TRUE(value_status.is_value_exists_);
  EXPECT_FALSE(value_status.is_value_set_);

  storage_->setValue(kObjectIndex, kValueIndex, kNecessaryValue);

  value_status = storage_->getValue(kObjectIndex, kValueIndex, value);
  EXPECT_TRUE(value_status.is_object_exists_);
  EXPECT_TRUE(value_status.is_value_exists_);
  EXPECT_TRUE(value_status.is_value_set_);
  EXPECT_EQ(kNecessaryValue, value.toString());
}

// TEST_F(StorageTest, setBinaryValueSuccessWithSetValue) {
//  const QString kObjectTitle("object_0");
//  const size_t kObjectIndex(0);
//  const QString kValueKey("key_0");
//  const size_t kValueIndex(0);
//  const bool kNecessaryValue(false);

//  bool value(false);

//  auto value_status = storage_->getBinaryValue(kObjectTitle, kValueKey,
//  value); EXPECT_TRUE(value_status.is_object_exists_);
//  EXPECT_TRUE(value_status.is_value_exists_);
//  EXPECT_TRUE(value_status.is_value_set_);
//  EXPECT_NE(kNecessaryValue, value);

//  storage_->setValue(kObjectIndex, kValueIndex, kNecessaryValue);

//  value_status = storage_->getBinaryValue(kObjectTitle, kValueKey, value);
//  EXPECT_TRUE(value_status.is_object_exists_);
//  EXPECT_TRUE(value_status.is_value_exists_);
//  EXPECT_TRUE(value_status.is_value_set_);
//  EXPECT_EQ(kNecessaryValue, value);
//}

// TEST_F(StorageTest, getValueType) {
//  EXPECT_EQ(QMetaType::Bool, storage_->getValueType(0, 0));
//  EXPECT_EQ(QMetaType::Bool, storage_->getValueType(0, 1));

//  EXPECT_EQ(QMetaType::Double, storage_->getValueType(0, 2));
//  EXPECT_EQ(QMetaType::Double, storage_->getValueType(0, 3));

//  EXPECT_EQ(QMetaType::QString, storage_->getValueType(0, 4));
//  EXPECT_EQ(QMetaType::QString, storage_->getValueType(0, 5));
//}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
