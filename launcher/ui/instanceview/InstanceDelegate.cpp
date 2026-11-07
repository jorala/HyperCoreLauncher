// SPDX-License-Identifier: GPL-3.0-only
/*
 *  HyperCore Launcher - Minecraft Launcher
 *  Copyright (C) 2022 Sefa Eyeoglu <contact@scrumplex.net>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, version 3.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * This file incorporates work covered by the following copyright and
 * permission notice:
 *
 *      Copyright 2013-2021 MultiMC Contributors
 *
 *      Licensed under the Apache License, Version 2.0 (the "License");
 *      you may not use this file except in compliance with the License.
 *      You may obtain a copy of the License at
 *
 *          http://www.apache.org/licenses/LICENSE-2.0
 *
 *      Unless required by applicable law or agreed to in writing, software
 *      distributed under the License is distributed on an "AS IS" BASIS,
 *      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *      See the License for the specific language governing permissions and
 *      limitations under the License.
 */

#include "InstanceDelegate.h"
#include <QApplication>
#include <QDebug>
#include <QPainter>
#include <QTextLayout>
#include <QTextOption>
#include <QtMath>

#include <QIcon>
#include <QMenu>
#include <QTextEdit>
#include "Application.h"
#include "BaseInstance.h"
#include "InstanceList.h"
#include "MMCTime.h"

// Origin: Qt
static void viewItemTextLayout(QTextLayout& textLayout, int lineWidth, qreal& height, qreal& widthUsed)
{
    height = 0;
    widthUsed = 0;
    textLayout.beginLayout();
    QString str = textLayout.text();
    while (true) {
        QTextLine line = textLayout.createLine();
        if (!line.isValid())
            break;
        if (line.textLength() == 0)
            break;
        line.setLineWidth(lineWidth);
        line.setPosition(QPointF(0, height));
        height += line.height();
        widthUsed = qMax(widthUsed, line.naturalTextWidth());
    }
    textLayout.endLayout();
}

ListViewDelegate::ListViewDelegate(QObject* parent) : QStyledItemDelegate(parent) {}

void drawSelectionRect(QPainter* painter, const QStyleOptionViewItem& option, const QRect& rect)
{
    if ((option.state & QStyle::State_Selected))
        painter->fillRect(rect, option.palette.brush(QPalette::Highlight));
    else {
        QColor backgroundColor = option.palette.color(QPalette::Window);
        backgroundColor.setAlpha(160);
        painter->fillRect(rect, QBrush(backgroundColor));
    }
}

void drawBadges(QPainter* painter, const QStyleOptionViewItem& option, BaseInstance* instance, QIcon::Mode mode, QIcon::State state)
{
    QList<QString> pixmaps;
    if (instance->isRunning()) {
        pixmaps.append("status-running");
    } else if (instance->hasCrashed() || instance->hasVersionBroken()) {
        pixmaps.append("status-bad");
    }

    static const int itemSide = 24;
    static const int spacing = 1;
    const int itemsPerRow = qMax(1, qFloor(double(option.rect.width() + spacing) / double(itemSide + spacing)));
    const int rows = qCeil((double)pixmaps.size() / (double)itemsPerRow);
    QListIterator<QString> it(pixmaps);
    painter->translate(option.rect.topLeft());
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < itemsPerRow; ++x) {
            if (!it.hasNext()) {
                return;
            }
            // FIXME: inject this.
            auto icon = QIcon::fromTheme(it.next());
            // opt.icon.paint(painter, iconbox, Qt::AlignCenter, mode, state);
            const QPixmap pixmap;
            // itemSide
            QRect badgeRect(option.rect.width() - x * itemSide + qMax(x - 1, 0) * spacing - itemSide,
                            y * itemSide + qMax(y - 1, 0) * spacing, itemSide, itemSide);
            icon.paint(painter, badgeRect, Qt::AlignCenter, mode, state);
        }
    }
    painter->translate(-option.rect.topLeft());
}

namespace {
// card geometry (pixels)
constexpr int kCardRadius = 22;
constexpr int kPadding = 14;
constexpr int kIconSize = 56;
constexpr int kNameHeight = 36;
constexpr int kPlaytimeHeight = 18;
constexpr int kRowHeight = 28;
constexpr int kSeparatorHeight = 14;
constexpr int kArrowWidth = 28;
// where the action list starts inside a selected card
constexpr int kActionsTop = kPadding + kIconSize + 6 + kNameHeight + kPlaytimeHeight + 10;
// an unselected card only shows its icon and name
constexpr int kCompactHeight = kPadding + kIconSize + 6 + kNameHeight + kPadding;
}  // namespace

int ListViewDelegate::cardHeight() const
{
    int height = kActionsTop + kPadding;
    for (auto* action : m_cardActions) {
        height += action ? kRowHeight : kSeparatorHeight;
    }
    return qMax(height, kCompactHeight);
}

QRect ListViewDelegate::nameRect(const QRect& card) const
{
    return QRect(card.left() + kPadding, card.top() + kPadding + kIconSize + 6, card.width() - 2 * kPadding, kNameHeight);
}

QList<QRect> ListViewDelegate::actionRects(const QRect& card) const
{
    QList<QRect> rects;
    int y = card.top() + kActionsTop;
    for (auto* action : m_cardActions) {
        const int height = action ? kRowHeight : kSeparatorHeight;
        rects.append(QRect(card.left() + kPadding, y, card.width() - 2 * kPadding, height));
        y += height;
    }
    return rects;
}

QAction* ListViewDelegate::actionAt(const QRect& itemRect, const QPoint& pos, QRect* rowRect, bool* onMenuArrow) const
{
    const QList<QRect> rects = actionRects(itemRect.adjusted(2, 2, -2, -2));
    for (qsizetype i = 0; i < m_cardActions.size() && i < rects.size(); ++i) {
        QAction* action = m_cardActions[i];
        if (!action || !action->isVisible() || !action->isEnabled() || !rects[i].contains(pos)) {
            continue;
        }
        if (rowRect) {
            *rowRect = rects[i];
        }
        if (onMenuArrow) {
            *onMenuArrow = action->menu() != nullptr && pos.x() >= rects[i].right() - kArrowWidth;
        }
        return action;
    }
    return nullptr;
}

void ListViewDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);
    painter->save();
    painter->setClipRect(opt.rect);
    painter->setRenderHint(QPainter::Antialiasing, true);

    opt.features |= QStyleOptionViewItem::WrapText;
    opt.text = index.data().toString();
    opt.textElideMode = Qt::ElideRight;
    opt.displayAlignment = Qt::AlignTop | Qt::AlignHCenter;

    const bool selected = opt.state.testFlag(QStyle::State_Selected);
    const QRect card = opt.rect.adjusted(2, 2, -2, -2);

    // colors: the card is darker than the view background, the selected card is a bit lighter and gets a gray outline
    const QColor base = opt.palette.color(QPalette::Base);
    const bool lightTheme = base.lightness() > 128;
    QColor cardColor = lightTheme ? base.darker(105) : base.darker(170);
    if (selected) {
        cardColor = lightTheme ? cardColor.darker(103) : cardColor.lighter(135);
    } else if (opt.state.testFlag(QStyle::State_MouseOver)) {
        cardColor = lightTheme ? cardColor.darker(106) : cardColor.lighter(160);
    }
    QPalette::ColorGroup cg = opt.state & QStyle::State_Enabled ? QPalette::Normal : QPalette::Disabled;
    if (cg == QPalette::Normal && !(opt.state & QStyle::State_Active))
        cg = QPalette::Inactive;
    const QColor textColor = opt.palette.color(cg, QPalette::Text);

    // draw the card
    painter->setBrush(cardColor);
    if (selected) {
        QColor outline = textColor;
        outline.setAlphaF(0.45f);
        painter->setPen(QPen(outline, 2));
    } else {
        painter->setPen(Qt::NoPen);
    }
    painter->drawRoundedRect(card, kCardRadius, kCardRadius);

    // icon mode and state, also used for badges
    QIcon::Mode mode = QIcon::Normal;
    if (!(opt.state & QStyle::State_Enabled))
        mode = QIcon::Disabled;
    QIcon::State state = opt.state & QStyle::State_Open ? QIcon::On : QIcon::Off;

    // draw the icon
    {
        QRect iconbox(card.left(), card.top() + kPadding, card.width(), kIconSize);
        opt.icon.paint(painter, iconbox, Qt::AlignCenter, mode, state);
    }

    // draw the name
    {
        painter->setPen(textColor);
        QTextOption textOption;
        textOption.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
        textOption.setTextDirection(opt.direction);
        textOption.setAlignment(QStyle::visualAlignment(opt.direction, opt.displayAlignment));
        QTextLayout textLayout;
        textLayout.setTextOption(textOption);
        textLayout.setFont(opt.font);
        textLayout.setText(opt.text);

        const QRect textRect = nameRect(card);
        qreal width, height;
        viewItemTextLayout(textLayout, textRect.width(), height, width);

        const QRect layoutRect = QStyle::alignedRect(opt.direction, opt.displayAlignment, QSize(textRect.width(), int(height)), textRect);
        const QPointF position = layoutRect.topLeft();
        painter->save();
        painter->setClipRect(textRect);
        for (int i = 0; i < textLayout.lineCount(); ++i) {
            textLayout.lineAt(i).draw(painter, position);
        }
        painter->restore();
    }

    // the selected card shows how long the instance was played
    auto* instance = static_cast<BaseInstance*>(index.data(InstanceList::InstancePointerRole).value<void*>());
    if (selected && instance) {
        const int64_t total = instance->totalTimePlayed();
        const QString playtime =
            total > 0 ? QObject::tr("Played %1").arg(Time::prettifyDuration(total, APPLICATION->settings()->get("ShowGameTimeWithoutDays").toBool()))
                      : QObject::tr("Not played yet");
        QColor dim = textColor;
        dim.setAlphaF(0.6f);
        painter->setPen(dim);
        QFont small = opt.font;
        small.setPointSizeF(qMax(7.0, small.pointSizeF() - 1.0));
        painter->setFont(small);
        const QRect nameBox = nameRect(card);
        const QRect playtimeBox(nameBox.left(), nameBox.bottom() + 1, nameBox.width(), kPlaytimeHeight);
        painter->drawText(playtimeBox, Qt::AlignCenter, painter->fontMetrics().elidedText(playtime, Qt::ElideRight, playtimeBox.width()));
        painter->setFont(opt.font);
    }

    // the selected card shows its actions
    if (selected && !m_cardActions.isEmpty()) {
        const QList<QRect> rects = actionRects(card);
        for (qsizetype i = 0; i < m_cardActions.size() && i < rects.size(); ++i) {
            QAction* action = m_cardActions[i];
            const QRect& row = rects[i];
            if (!action) {
                QColor line = textColor;
                line.setAlphaF(0.15f);
                painter->setPen(line);
                painter->drawLine(row.left(), row.center().y(), row.right(), row.center().y());
                continue;
            }
            if (!action->isVisible()) {
                continue;
            }

            const bool enabled = action->isEnabled();

            // the row under the cursor gets a highlight bar so it is clear what a click will do
            if (enabled && opt.state.testFlag(QStyle::State_MouseOver) && row.contains(m_hoverPos)) {
                QColor bar = textColor;
                bar.setAlphaF(0.16f);
                painter->setPen(Qt::NoPen);
                painter->setBrush(bar);
                painter->drawRoundedRect(row.adjusted(-4, 1, 4, -1), 9, 9);
            }
            const QColor rowColor =
                enabled ? opt.palette.color(QPalette::Active, QPalette::Text) : opt.palette.color(QPalette::Disabled, QPalette::Text);
            painter->setPen(rowColor);

            const QRect iconRect(row.left() + 8, row.center().y() - 9, 18, 18);
            action->icon().paint(painter, iconRect, Qt::AlignCenter, enabled ? QIcon::Normal : QIcon::Disabled);

            QRect labelRect = row.adjusted(34, 0, action->menu() ? -kArrowWidth : -4, 0);
            painter->drawText(labelRect, Qt::AlignVCenter | Qt::AlignLeft,
                              painter->fontMetrics().elidedText(action->iconText(), Qt::ElideRight, labelRect.width()));

            if (action->menu()) {
                QColor divider = rowColor;
                divider.setAlphaF(0.2f);
                painter->setPen(divider);
                painter->drawLine(row.right() - kArrowWidth, row.top() + 5, row.right() - kArrowWidth, row.bottom() - 5);

                painter->setPen(QPen(rowColor, 1.5));
                const QPoint c(row.right() - kArrowWidth / 2, row.center().y());
                const QPoint chevron[3] = { c + QPoint(-3, -1), c + QPoint(0, 2), c + QPoint(3, -1) };
                painter->drawPolyline(chevron, 3);
            }
        }
    }

    // FIXME: this really has no business of being here. Make generic.
    if (instance) {
        drawBadges(painter, opt, instance, mode, state);
    }

    painter->restore();
}

QSize ListViewDelegate::sizeHint(const QStyleOptionViewItem& option, [[maybe_unused]] const QModelIndex& index) const
{
    // an unselected card is small (icon + name), the selected one grows to fit its playtime and actions
    return QSize(CardWidth, option.state.testFlag(QStyle::State_Selected) ? cardHeight() : kCompactHeight);
}

class NoReturnTextEdit : public QTextEdit {
    Q_OBJECT
   public:
    explicit NoReturnTextEdit(QWidget* parent) : QTextEdit(parent)
    {
        setTextInteractionFlags(Qt::TextEditorInteraction);
        setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    }
    bool event(QEvent* event) override
    {
        auto eventType = event->type();
        if (eventType == QEvent::KeyPress || eventType == QEvent::KeyRelease) {
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
            auto key = keyEvent->key();
            if ((key == Qt::Key_Return || key == Qt::Key_Enter) && eventType == QEvent::KeyPress) {
                emit editingDone();
                return true;
            }
            if (key == Qt::Key_Tab) {
                return true;
            }
        }
        return QTextEdit::event(event);
    }
   signals:
    void editingDone();
};

void ListViewDelegate::updateEditorGeometry(QWidget* editor,
                                            const QStyleOptionViewItem& option,
                                            [[maybe_unused]] const QModelIndex& index) const
{
    editor->setGeometry(nameRect(option.rect.adjusted(2, 2, -2, -2)));
}

void ListViewDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    auto text = index.data(Qt::EditRole).toString();
    QTextEdit* realEditor = qobject_cast<NoReturnTextEdit*>(editor);
    realEditor->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    realEditor->append(text);
    realEditor->selectAll();
    realEditor->document()->clearUndoRedoStacks();
}

void ListViewDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    QTextEdit* realEditor = qobject_cast<NoReturnTextEdit*>(editor);
    QString text = realEditor->toPlainText();
    const auto before = model->data(index).toString();
    if (model->setData(index, text)) {
        emit textChanged(before, model->data(index).toString());
    }
}

QWidget* ListViewDelegate::createEditor(QWidget* parent,
                                        [[maybe_unused]] const QStyleOptionViewItem& option,
                                        [[maybe_unused]] const QModelIndex& index) const
{
    auto editor = new NoReturnTextEdit(parent);
    connect(editor, &NoReturnTextEdit::editingDone, this, &ListViewDelegate::editingDone);
    return editor;
}

void ListViewDelegate::editingDone()
{
    NoReturnTextEdit* editor = qobject_cast<NoReturnTextEdit*>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}

#include "InstanceDelegate.moc"
