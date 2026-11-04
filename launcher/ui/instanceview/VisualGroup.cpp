// SPDX-License-Identifier: GPL-3.0-only
/*
 *  HyperCore Launcher - Minecraft Launcher
 *  Copyright (C) 2022 Sefa Eyeoglu <contact@scrumplex.net>
 *  Copyright (C) 2023 Tayou <git@tayou.org>
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

#include "VisualGroup.h"

#include <QApplication>
#include <QDebug>
#include <QLinearGradient>
#include <QModelIndex>
#include <QPainter>
#include <QtMath>
#include <utility>

#include "InstanceView.h"

VisualGroup::VisualGroup(QString text, InstanceView* view) : view(view), text(std::move(text)), collapsed(false) {}

VisualGroup::VisualGroup(const VisualGroup* other) : view(other->view), text(other->text), collapsed(other->collapsed) {}

void VisualGroup::update()
{
    auto temp_items = items();
    // never zero, a narrow window must not divide by zero
    const int itemsPerRow = qMax(1, view->itemsPerRow());

    int numRows = qMax(1, qCeil((qreal)temp_items.size() / (qreal)itemsPerRow));
    rows = QList<VisualRow>(numRows);

    int maxRowHeight = 0;
    int positionInRow = 0;
    int currentRow = 0;
    int offsetFromTop = 0;
    for (auto item : temp_items) {
        if (positionInRow == itemsPerRow) {
            rows[currentRow].height = maxRowHeight;
            rows[currentRow].top = offsetFromTop;
            currentRow++;
            if (currentRow >= rows.size()) {
                currentRow = rows.size() - 1;
            }
            offsetFromTop += maxRowHeight + 5;
            positionInRow = 0;
            maxRowHeight = 0;
        }
        QStyleOptionViewItem viewItemOption;
        view->initViewItemOption(&viewItemOption);
        if (view->selectionModel() && view->selectionModel()->isSelected(item)) {
            viewItemOption.state |= QStyle::State_Selected;
        }

        auto itemHeight = view->itemDelegate()->sizeHint(viewItemOption, item).height();
        if (itemHeight > maxRowHeight) {
            maxRowHeight = itemHeight;
        }
        rows[currentRow].items.append(item);
        positionInRow++;
    }
    rows[currentRow].height = maxRowHeight;
    rows[currentRow].top = offsetFromTop;
}

QPair<int, int> VisualGroup::positionOf(const QModelIndex& index) const
{
    int y = 0;
    for (auto& row : rows) {
        for (auto x = 0; x < row.items.size(); x++) {
            if (row.items[x] == index) {
                return qMakePair(x, y);
            }
        }
        y++;
    }
    qWarning() << "Item" << index.row() << index.data(Qt::DisplayRole).toString() << "not found in visual group" << text;
    return qMakePair(0, 0);
}

int VisualGroup::rowTopOf(const QModelIndex& index) const
{
    auto position = positionOf(index);
    return rows[position.second].top;
}

int VisualGroup::rowHeightOf(const QModelIndex& index) const
{
    auto position = positionOf(index);
    return rows[position.second].height;
}

VisualGroup::HitResults VisualGroup::hitScan(const QPoint& pos) const
{
    VisualGroup::HitResults results = VisualGroup::NoHit;
    int y_start = verticalPosition();
    int body_start = y_start + headerHeight();
    int body_end = body_start + contentHeight();
    int y = pos.y();
    const int x = pos.x();
    if (x < m_horizontalPosition || x >= m_horizontalPosition + view->columnWidth()) {
        results = VisualGroup::NoHit;
    } else if (y < y_start) {
        results = VisualGroup::NoHit;
    } else if (y < body_start) {
        results = VisualGroup::HeaderHit;
        int collapseSize = headerHeight() - 4;

        // the icon
        QRect iconRect = QRect(m_horizontalPosition + 2, 2 + y_start, view->columnWidth() - 4, collapseSize);
        if (iconRect.contains(pos)) {
            results |= VisualGroup::CheckboxHit;
        }
    } else if (y < body_end) {
        results |= VisualGroup::BodyHit;
    }
    return results;
}

void VisualGroup::drawHeader(QPainter* painter, const QStyleOptionViewItem& option) const
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    QFont font(QApplication::font());
    font.setBold(true);
    painter->setFont(font);
    const QFontMetrics fm(font);

    const QColor textColor = option.palette.text().color();
    const bool light = option.palette.base().color().lightness() > 128;
    const QString count = QString::number(items().size());
    const int badgeMin = fm.height() + 2;
    const int maxName = qMax(20, option.rect.width() - 32 - 10 - badgeMin - 12);
    const QString name = fm.elidedText(text.isEmpty() ? QObject::tr("Ungrouped") : text, Qt::ElideRight, maxName);

    // pill with chevron + name + count badge
    const int pillHeight = fm.height() + 10;
    const int pillTop = option.rect.top() + 8;
    const int nameWidth = fm.horizontalAdvance(name);
    const int badgeWidth = qMax(pillHeight - 8, fm.horizontalAdvance(count) + 12);
    const int pillWidth = 14 + 10 + 8 + nameWidth + 10 + badgeWidth + 6;
    const QRect pill(option.rect.left() + 8, pillTop, pillWidth, pillHeight);

    QColor pillColor = textColor;
    pillColor.setAlphaF(light ? 0.08f : 0.10f);
    painter->setPen(Qt::NoPen);
    painter->setBrush(pillColor);
    painter->drawRoundedRect(pill, pillHeight / 2.0, pillHeight / 2.0);

    // chevron
    QColor chevronColor = textColor;
    chevronColor.setAlphaF(0.7f);
    painter->setPen(QPen(chevronColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->setBrush(Qt::NoBrush);
    const QPoint c(pill.left() + 16, pill.center().y());
    QPolygon chevron;
    if (collapsed) {
        chevron << QPoint(c.x() - 2, c.y() - 4) << QPoint(c.x() + 2, c.y()) << QPoint(c.x() - 2, c.y() + 4);
    } else {
        chevron << QPoint(c.x() - 4, c.y() - 2) << QPoint(c.x(), c.y() + 2) << QPoint(c.x() + 4, c.y() - 2);
    }
    painter->drawPolyline(chevron);

    // name
    painter->setPen(textColor);
    const QRect nameRect(pill.left() + 32, pill.top(), nameWidth + 2, pill.height());
    painter->drawText(nameRect, Qt::AlignLeft | Qt::AlignVCenter, name);

    // count badge
    const QRect badge(nameRect.right() + 10, pill.top() + 4, badgeWidth, pill.height() - 8);
    QColor badgeColor = textColor;
    badgeColor.setAlphaF(0.18f);
    painter->setPen(Qt::NoPen);
    painter->setBrush(badgeColor);
    painter->drawRoundedRect(badge, badge.height() / 2.0, badge.height() / 2.0);
    QFont small(font);
    small.setPointSizeF(qMax(7.0, font.pointSizeF() - 1.0));
    painter->setFont(small);
    painter->setPen(textColor);
    painter->drawText(badge, Qt::AlignCenter, count);

    // fading line
    const int lineY = pill.center().y();
    const int lineStart = pill.right() + 14;
    const int lineEnd = option.rect.right() - 8;
    if (lineEnd > lineStart) {
        QLinearGradient grad(lineStart, 0, lineEnd, 0);
        QColor from = textColor;
        from.setAlphaF(0.22f);
        QColor to = textColor;
        to.setAlphaF(0.0f);
        grad.setColorAt(0, from);
        grad.setColorAt(1, to);
        painter->setPen(QPen(QBrush(grad), 1));
        painter->drawLine(lineStart, lineY, lineEnd, lineY);
    }

    painter->restore();
}

int VisualGroup::totalHeight() const
{
    return headerHeight() + contentHeight();
}

int VisualGroup::headerHeight()
{
    QFont font(QApplication::font());
    font.setBold(true);
    QFontMetrics fontMetrics(font);

    const int height = fontMetrics.height() + 10 /* pill padding */ + 8 /* top */ + 8 /* bottom */;
    return height;
    /*
    int raw = view->viewport()->fontMetrics().height() + 4;
    // add english. maybe. depends on font height.
    if (raw % 2 == 0)
        raw++;
    return std::min(raw, 25);
    */
}

int VisualGroup::contentHeight() const
{
    if (collapsed) {
        return 0;
    }
    auto last = rows[numRows() - 1];
    return last.top + last.height;
}

int VisualGroup::numRows() const
{
    return (int)rows.size();
}

int VisualGroup::verticalPosition() const
{
    return m_verticalPosition;
}

QList<QModelIndex> VisualGroup::items() const
{
    QList<QModelIndex> indices;
    for (int i = 0; i < view->model()->rowCount(); ++i) {
        const QModelIndex index = view->model()->index(i, 0);
        if (index.data(InstanceViewRoles::GroupRole).toString() == text) {
            indices.append(index);
        }
    }
    return indices;
}
