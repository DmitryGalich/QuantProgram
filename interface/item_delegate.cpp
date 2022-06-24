#include "item_delegate.h"

#include <QIntValidator>
#include <QLineEdit>

ItemDelegate::ItemDelegate(const Storage &storage, QObject *parent)
    : QStyledItemDelegate(parent), kStorage_(storage) {}

QWidget *ItemDelegate::createEditor(QWidget *parent,
                                    const QStyleOptionViewItem & /*option*/,
                                    const QModelIndex &index) const {
  QLineEdit *editor = new QLineEdit(parent);

  auto type = kStorage_.getValueType(index.column(), index.row());
  if (type == QMetaType::Type::Bool)
    editor->setValidator(new QIntValidator());

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
  if (type == QMetaType::Type::Bool) {
    QString data = line->text();
    QIntValidator validator;
    int pos = 0;

    if (validator.validate(data, pos) != QValidator::Acceptable)
      return;

    model->setData(index, (data.toDouble() > 0));
  } else {
    model->setData(index, line->text());
  }
}

void ItemDelegate::updateEditorGeometry(QWidget *editor,
                                        const QStyleOptionViewItem &option,
                                        const QModelIndex & /*index*/) const {
  editor->setGeometry(option.rect);
}

void ItemDelegate::initStyleOption(QStyleOptionViewItem *option,
                                   const QModelIndex &index) const {

  auto type = kStorage_.getValueType(index.column(), index.row());

  QStyledItemDelegate::initStyleOption(option, index);

  switch (type) {
  case QMetaType::Type::Bool: {
    option->backgroundBrush = {QColor(255, 69, 58)};
    return;
  }
  case QMetaType::Type::Double: {
    option->backgroundBrush = {QColor(50, 215, 75)};
    return;
  }
  case QMetaType::Type::QString: {
    option->backgroundBrush = {QColor(255, 159, 10)};
    return;
  }
  default:
    option->backgroundBrush = {QColor(229, 229, 234)};
    return;
  }
}
