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
    object.numeric_values_.insert({"key_3", std::make_unique<double>(0.0)});

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

TEST_F(StorageTest, getBinaryValueSuccess) {
  bool value(false);

  auto value_status = storage_->getBinaryValue("object_0", "key_0", value);
  EXPECT_TRUE(value_status.is_object_exists_);
  EXPECT_TRUE(value_status.is_value_exists_);
  EXPECT_TRUE(value_status.is_value_set_);
  EXPECT_TRUE(value);
}

TEST_F(StorageTest, getBinaryValueUnsetValue) {
  bool value(false);

  auto value_status = storage_->getBinaryValue("object_0", "key_1", value);
  EXPECT_TRUE(value_status.is_object_exists_);
  EXPECT_TRUE(value_status.is_value_exists_);
  EXPECT_FALSE(value_status.is_value_set_);
}

TEST_F(StorageTest, getBinaryValueUnexistingObject) {
  bool value(false);

  auto value_status = storage_->getBinaryValue("hesoyam", {}, value);
  EXPECT_FALSE(value_status.is_object_exists_);
}

TEST_F(StorageTest, getBinaryValueUnexistingValue) {
  bool value(false);

  auto value_status = storage_->getBinaryValue("object_0", "aezakmi", value);
  EXPECT_TRUE(value_status.is_object_exists_);
  EXPECT_FALSE(value_status.is_value_exists_);
}

TEST_F(StorageTest, setBinaryValueSuccessWithUnsetValue) {
  const QString kObjectTitle("object_0");
  const QString kValueKey("key_1");
  const bool kNecessaryValue(true);

  bool value(false);

  auto value_status = storage_->getBinaryValue(kObjectTitle, kValueKey, value);
  EXPECT_TRUE(value_status.is_object_exists_);
  EXPECT_TRUE(value_status.is_value_exists_);
  EXPECT_FALSE(value_status.is_value_set_);

  storage_->setValue(kObjectTitle, kValueKey, kNecessaryValue);

  value_status = storage_->getBinaryValue(kObjectTitle, kValueKey, value);
  EXPECT_TRUE(value_status.is_object_exists_);
  EXPECT_TRUE(value_status.is_value_exists_);
  EXPECT_TRUE(value_status.is_value_set_);
  EXPECT_EQ(kNecessaryValue, value);
}

TEST_F(StorageTest, setBinaryValueSuccessWithSetValue) {
  const QString kObjectTitle("object_0");
  const QString kValueKey("key_0");
  const bool kNecessaryValue(false);

  bool value(false);

  auto value_status = storage_->getBinaryValue(kObjectTitle, kValueKey, value);
  EXPECT_TRUE(value_status.is_object_exists_);
  EXPECT_TRUE(value_status.is_value_exists_);
  EXPECT_TRUE(value_status.is_value_set_);
  EXPECT_NE(kNecessaryValue, value);

  storage_->setValue(kObjectTitle, kValueKey, kNecessaryValue);

  value_status = storage_->getBinaryValue(kObjectTitle, kValueKey, value);
  EXPECT_TRUE(value_status.is_object_exists_);
  EXPECT_TRUE(value_status.is_value_exists_);
  EXPECT_TRUE(value_status.is_value_set_);
  EXPECT_EQ(kNecessaryValue, value);
}

TEST_F(StorageTest, setBinaryValueUnexistingObject) {
  const QString kObjectTitle("hesoyam");
  bool value(true);

  storage_->setValue(kObjectTitle, {}, value);

  auto value_status = storage_->getBinaryValue(kObjectTitle, {}, value);
  EXPECT_FALSE(value_status.is_object_exists_);
}

TEST_F(StorageTest, setBinaryValueUnexistingValue) {
  const QString kObjectTitle("object_0");
  const QString kValueKey("hesoyam");
  bool value(true);

  storage_->setValue(kObjectTitle, kValueKey, value);

  auto value_status = storage_->getBinaryValue(kObjectTitle, kValueKey, value);
  EXPECT_TRUE(value_status.is_object_exists_);
  EXPECT_FALSE(value_status.is_value_exists_);
}

// Simular bunch of tests for numeric and string values functions

TEST_F(StorageTest, getObjectKeys) {
  EXPECT_EQ(storage_->getAllObjectKeys().size(), 2);
}

TEST_F(StorageTest, getValueKeys) {
  auto keys = storage_->getAllValueKeys();

  EXPECT_EQ(storage_->getAllValueKeys().size(), 6);
}

TEST_F(StorageTest, getValueType) {
  EXPECT_EQ(QMetaType::Bool, storage_->getValueType("object_0", "key_0"));
  EXPECT_EQ(QMetaType::Bool, storage_->getValueType("object_0", "key_1"));

  EXPECT_EQ(QMetaType::Double, storage_->getValueType("object_0", "key_2"));
  EXPECT_EQ(QMetaType::Double, storage_->getValueType("object_0", "key_3"));

  EXPECT_EQ(QMetaType::QString, storage_->getValueType("object_0", "key_4"));
  EXPECT_EQ(QMetaType::QString, storage_->getValueType("object_0", "key_5"));

  EXPECT_EQ(QMetaType::Bool, storage_->getValueType(0, 0));
  EXPECT_EQ(QMetaType::Bool, storage_->getValueType(0, 1));

  EXPECT_EQ(QMetaType::Double, storage_->getValueType(0, 2));
  EXPECT_EQ(QMetaType::Double, storage_->getValueType(0, 3));

  EXPECT_EQ(QMetaType::QString, storage_->getValueType(0, 4));
  EXPECT_EQ(QMetaType::QString, storage_->getValueType(0, 5));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
