#include "mainwindow.h"

#include <QApplication>

#include "storage.h"

Storage createStorage() {
  std::map<QString, Storage::Object> data;

  {
    Storage::Object object;
    object.binary_values_.insert({"key_0", nullptr});
    object.binary_values_.insert({"key_1", std::make_shared<bool>(false)});
    object.numeric_values_.insert({"key_3", nullptr});
    object.string_values_.insert({"key_5", nullptr});
    data.insert({"object_0", object});
  }

  {
    Storage::Object object;
    object.binary_values_.insert({"key_0", nullptr});
    object.numeric_values_.insert({"key_2", nullptr});
    object.numeric_values_.insert({"key_4", std::make_unique<double>(999)});
    object.string_values_.insert(
        {"key_6", std::make_shared<QString>("hesoyam")});

    data.insert({"object_1", object});
  }

  return {data};
}

int main(int argc, char *argv[]) {
  Storage storage = createStorage();

  QApplication a(argc, argv);
  MainWindow w(storage);
  w.show();
  return a.exec();
}
