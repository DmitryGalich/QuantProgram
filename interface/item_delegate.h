#pragma once

#include <QItemDelegate>
#include <QStyledItemDelegate>

#include "storage/storage.h"

class ItemDelegate : public QStyledItemDelegate {
  Q_OBJECT
public:
  explicit ItemDelegate(const Storage &storage, QObject *parent = 0);

protected:
  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                        const QModelIndex &index) const;
  void setEditorData(QWidget *editor, const QModelIndex &index) const;
  void setModelData(QWidget *editor, QAbstractItemModel *model,
                    const QModelIndex &index) const;
  void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;

  void initStyleOption(QStyleOptionViewItem *option,
                       const QModelIndex &index) const;

private:
  const Storage &kStorage_;
};
