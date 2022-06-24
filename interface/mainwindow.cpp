#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <iostream>

MainWindow::MainWindow(Storage &storage, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
      item_delegate_(new ItemDelegate(storage, this)),
      table_model_(new TableModel(storage, this)) {
  ui->setupUi(this);

  ui->tableView->setItemDelegate(item_delegate_);
  ui->tableView->setModel(table_model_);
  ui->tableView->horizontalHeader()->setVisible(true);
  ui->tableView->verticalHeader()->setVisible(true);
  ui->tableView->show();

  ui->tableView->setStyleSheet("QTableView{ selection-color: white; }"
                               "QLineEdit{ background-color: #E5E5EA }");

  ui->boolLabel->setStyleSheet("color: #409CFF");
  ui->doubleLabel->setStyleSheet("color: #32D74B");
  ui->stringLabel->setStyleSheet("color: #FF9F0A");

  for (int c = 0; c < ui->tableView->horizontalHeader()->count(); ++c)
    ui->tableView->horizontalHeader()->setSectionResizeMode(
        c, QHeaderView::Stretch);

  for (int r = 0; r < ui->tableView->verticalHeader()->count(); ++r)
    ui->tableView->verticalHeader()->setSectionResizeMode(r,
                                                          QHeaderView::Stretch);
}

MainWindow::~MainWindow() { delete ui; }
