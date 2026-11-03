/* Copyright 2013-2021 MultiMC Contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <QAction>
#include <QCache>
#include <QList>
#include <QRect>
#include <QStyledItemDelegate>

class ListViewDelegate : public QStyledItemDelegate {
    Q_OBJECT

   public:
    explicit ListViewDelegate(QObject* parent = 0);
    virtual ~ListViewDelegate() {}

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

    void setEditorData(QWidget* editor, const QModelIndex& index) const override;
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;

    /// Width of an instance card in pixels (the view lays its cells out with this).
    static constexpr int CardWidth = 200;

    /// The actions shown inside the selected card, top to bottom. A nullptr entry draws a separator.
    void setCardActions(const QList<QAction*>& actions) { m_cardActions = actions; }
    const QList<QAction*>& cardActions() const { return m_cardActions; }

    /// The enabled action row under \a pos for the card drawn in \a itemRect, or nullptr.
    /// \a rowRect receives the row's rectangle, \a onMenuArrow whether the arrow of an action with a menu was hit.
    QAction* actionAt(const QRect& itemRect, const QPoint& pos, QRect* rowRect = nullptr, bool* onMenuArrow = nullptr) const;

   signals:
    void textChanged(QString before, QString after) const;

   private slots:
    void editingDone();

   private:
    int cardHeight() const;
    QList<QRect> actionRects(const QRect& card) const;
    QRect nameRect(const QRect& card) const;

    QList<QAction*> m_cardActions;
};
