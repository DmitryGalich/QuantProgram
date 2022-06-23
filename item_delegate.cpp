#include "item_delegate.h"

#include <QIntValidator>
#include <QLineEdit>

#include <iostream>

ItemDelegate::ItemDelegate(const Storage &storage, QObject *parent)
    : QItemDelegate(parent), kStorage_(storage) {}

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
                                        const QModelIndex & /*index*/) const {
  editor->setGeometry(option.rect);
}
