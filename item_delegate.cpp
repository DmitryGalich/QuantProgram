#include "item_delegate.h"

#include <QIntValidator>
#include <QLineEdit>

#include <iostream>

ItemDelegate::ItemDelegate(const Storage &storage, QObject *parent)
    : QStyledItemDelegate(parent), kStorage_(storage) {}

QWidget *ItemDelegate::createEditor(QWidget *parent,
                                    const QStyleOptionViewItem & /*option*/,
                                    const QModelIndex & /*index*/) const {
  QLineEdit *editor = new QLineEdit(parent);
  return editor;
}

void ItemDelegate::setEditorData(QWidget *editor,
                                 const QModelIndex &index) const {
  QString value = index.model()->data(index, Qt::EditRole).toString();
  QLineEdit *line = static_cast<QLineEdit *>(editor);
  line->setText(value);
}

void ItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                const QModelIndex &index) const {
  QLineEdit *line = static_cast<QLineEdit *>(editor);

  auto type = kStorage_.getValueType(index.column(), index.row());
  if (type == QMetaType::Type::Bool || type == QMetaType::Type::Double) {

    QDoubleValidator validator;
    int pos = 0;
    QString data = line->text();

    if (validator.validate(data, pos) != QValidator::Acceptable)
      return;

    if (type == QMetaType::Type::Bool) {
      model->setData(index, (data.toDouble() > 0));
      return;
    }

    model->setData(index, data);

  } else {
    model->setData(index, line->text());
  }
}

void ItemDelegate::updateEditorGeometry(QWidget *editor,
                                        const QStyleOptionViewItem &option,
                                        const QModelIndex &index) const {
  std::cout << kStorage_.getAllObjectKeys().at(index.column()).toStdString()
            << " : "
            << kStorage_.getAllValueKeys().at(index.row()).toStdString()
            << " : " << kStorage_.getValueType(index.column(), index.row())
            << std::endl;

  editor->setGeometry(option.rect);
}

void ItemDelegate::initStyleOption(QStyleOptionViewItem *option,
                                   const QModelIndex &index) const {
  //  std::cout << index.column() << " : " << index.row() << std::endl;

  auto type = kStorage_.getValueType(index.column(), index.row());

  QStyledItemDelegate::initStyleOption(option, index);

  switch (type) {
  case QMetaType::Type::Bool: {
    option->backgroundBrush = {Qt::green};
    return;
  }
  case QMetaType::Type::Double: {
    option->backgroundBrush = {Qt::blue};
    return;
  }
  case QMetaType::Type::QString: {
    option->backgroundBrush = {Qt::yellow};
    return;
  }
  default:
    option->backgroundBrush = {Qt::red};
    return;
  }
}
