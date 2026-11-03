// SPDX-License-Identifier: GPL-3.0-only
/*
 *  HyperCore Launcher - Minecraft Launcher
 *  Copyright (C) 2026 HyperCore Launcher Contributors
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
 */

#include "HyperCoreTheme.h"

#include <QLatin1String>
#include <QObject>

namespace {
// Colors sampled from the HyperCore layout mockup and the logo.
constexpr QLatin1String kChrome("#3c3c3c");       // top bar and bottom bars
constexpr QLatin1String kChromeHover("#4b4b4b");  // button hover on the bars
constexpr QLatin1String kDivider("#7a7a7a");      // line under the top bar
constexpr QLatin1String kContent("#222222");      // area behind the instance cards
constexpr QLatin1String kCard("#141414");         // instance card
constexpr QLatin1String kCardHover("#262626");    // button hover inside a card
constexpr QLatin1String kAccent("#65a56c");       // logo green
}  // namespace

QString HyperCoreTheme::id()
{
    return "hypercore";
}

QString HyperCoreTheme::name()
{
    return QObject::tr("HyperCore");
}

QString HyperCoreTheme::tooltip()
{
    return QObject::tr("The default HyperCore look: dark chrome with rounded instance cards.");
}

QPalette HyperCoreTheme::colorScheme()
{
    QPalette p;
    p.setColor(QPalette::Window, QColor(kChrome));
    p.setColor(QPalette::WindowText, QColor(235, 235, 235));
    p.setColor(QPalette::Base, QColor(kContent));
    p.setColor(QPalette::AlternateBase, QColor(30, 30, 30));
    p.setColor(QPalette::ToolTipBase, QColor(kCard));
    p.setColor(QPalette::ToolTipText, Qt::white);
    p.setColor(QPalette::Text, QColor(235, 235, 235));
    p.setColor(QPalette::Button, QColor(52, 52, 52));
    p.setColor(QPalette::ButtonText, QColor(235, 235, 235));
    p.setColor(QPalette::BrightText, Qt::red);
    p.setColor(QPalette::Link, QColor(kAccent));
    p.setColor(QPalette::Highlight, QColor(kAccent));
    p.setColor(QPalette::HighlightedText, Qt::black);
    p.setColor(QPalette::PlaceholderText, QColor(140, 140, 140));
    return fadeInactive(p, fadeAmount(), fadeColor());
}

double HyperCoreTheme::fadeAmount()
{
    return 0.5;
}

QColor HyperCoreTheme::fadeColor()
{
    return QColor(kChrome);
}

bool HyperCoreTheme::hasStyleSheet()
{
    return true;
}

QString HyperCoreTheme::appStyleSheet()
{
    // Qt style sheets: only the main window chrome is restyled here, dialogs keep the palette-based Fusion look.
    return QStringLiteral(
               "QToolTip { color: #ffffff; background-color: %1; border: 1px solid %2; padding: 4px; }"

               "QMainWindow { background: %3; }"
               "InstanceView { background: %3; border: none; }"

               // top bar: Minecraft version | account | total playtime
               "QToolBar#headerToolBar { background: %4; border: none; border-bottom: 4px solid %5; padding: 10px 8px; spacing: 48px; }"
               "QToolBar#headerToolBar QLabel { font-size: 15px; color: #e6e6e6; }"
               "QToolBar#headerToolBar QToolButton { font-size: 15px; }"

               // bottom bars: actions and news
               "QToolBar#mainToolBar, QToolBar#newsToolBar { background: %4; border: none; padding: 4px; spacing: 10px; }"
               "QToolBar#mainToolBar QToolButton, QToolBar#newsToolBar QToolButton, QToolBar#headerToolBar QToolButton {"
               "  background: transparent; border: none; border-radius: 10px; padding: 6px 10px; }"
               "QToolBar#mainToolBar QToolButton:hover, QToolBar#newsToolBar QToolButton:hover, QToolBar#headerToolBar QToolButton:hover {"
               "  background: %6; }"

               // the selected instance as a rounded card
               "QToolBar#instanceToolBar { background: %7; border: none; border-radius: 28px; padding: 16px 10px; spacing: 4px; }"
               "QToolBar#instanceToolBar QToolButton { background: transparent; border: none; border-radius: 8px; padding: 7px 10px; }"
               "QToolBar#instanceToolBar QToolButton:hover { background: %8; }"
               "QToolBar#instanceToolBar QToolButton:disabled { color: #6a6a6a; }"

               "QStatusBar { background: %3; color: #b8b8b8; }")
        .arg(kCard, kAccent, kContent, kChrome, kDivider, kChromeHover, kCard, kCardHover);
}
