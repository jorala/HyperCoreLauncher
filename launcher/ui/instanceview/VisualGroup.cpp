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
    int body_end = body_start + containerHeight();
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
    font.setPointSizeF(font.pointSizeF() + 1.0);
    painter->setFont(font);
    const QFontMetrics fm(font);

    const QColor base = option.palette.base().color();
    const QColor textColor = option.palette.text().color();
    const bool light = base.lightness() > 128;

    // the group label: a rounded bar as wide as the column
    const QRect label(option.rect.left(), option.rect.top(), option.rect.width(), fm.height() + 14);
    painter->setPen(Qt::NoPen);
    painter->setBrush(light ? base.darker(112) : base.lighter(155));
    painter->drawRoundedRect(label, 14, 14);

    // collapse chevron on the left
    QColor chevronColor = textColor;
    chevronColor.setAlphaF(0.65f);
    painter->setPen(QPen(chevronColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    const QPoint c(label.left() + 20, label.center().y());
    QPolygon chevron;
    if (collapsed) {
        chevron << QPoint(c.x() - 2, c.y() - 4) << QPoint(c.x() + 2, c.y()) << QPoint(c.x() - 2, c.y() + 4);
    } else {
        chevron << QPoint(c.x() - 4, c.y() - 2) << QPoint(c.x(), c.y() + 2) << QPoint(c.x() + 4, c.y() - 2);
    }
    painter->drawPolyline(chevron);

    // instance count on the right
    const QString count = QString::number(items().size());
    painter->setPen(chevronColor);
    const QRect countRect(label.right() - 36, label.top(), 28, label.height());
    painter->drawText(countRect, Qt::AlignRight | Qt::AlignVCenter, count);

    // the name, centered
    painter->setPen(textColor);
    const QRect nameRect = label.adjusted(40, 0, -40, 0);
    painter->drawText(nameRect, Qt::AlignCenter, fm.elidedText(text.isEmpty() ? QObject::tr("Ungrouped") : text, Qt::ElideRight, nameRect.width()));

    // the container that holds the group's cards
    if (!collapsed) {
        const QRect container(option.rect.left(), label.bottom() + 12, option.rect.width(), containerHeight());
        painter->setPen(Qt::NoPen);
        painter->setBrush(light ? base.darker(104) : base.darker(128));
        painter->drawRoundedRect(container, 28, 28);
    }

    painter->restore();
}

int VisualGroup::totalHeight() const
{
    return headerHeight() + containerHeight();
}

int VisualGroup::containerHeight() const
{
    return collapsed ? 0 : contentHeight() + 2 * ContainerPadding;
}

int VisualGroup::headerHeight()
{
    QFont font(QApplication::font());
    font.setBold(true);
    QFontMetrics fontMetrics(font);

    const int height = fontMetrics.height() + 14 /* label */ + 12 /* gap to the container */;
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
