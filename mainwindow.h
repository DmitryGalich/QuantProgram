#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "item_delegate.h"
#include "table_model.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(Storage &storage, QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void on_tableView_clicked(const QModelIndex &index);

private:
  Ui::MainWindow *ui;
  ItemDelegate *item_delegate_;
  TableModel *table_model_;
};
#endif // MAINWINDOW_H
