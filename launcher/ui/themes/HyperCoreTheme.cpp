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
constexpr QLatin1String kAccent("#9a9a9a");       // gray accent
constexpr QLatin1String kSelected("#5c5c5c");     // selected item in lists and tabs
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
    p.setColor(QPalette::Highlight, QColor(kSelected));
    p.setColor(QPalette::HighlightedText, Qt::white);
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

               "QStatusBar { background: %3; color: #b8b8b8; }"

               // menus
               "QMenu { background: %7; border: 1px solid #3a3a3a; border-radius: 12px; padding: 6px; }"
               "QMenu::item { padding: 7px 26px 7px 14px; border-radius: 8px; margin: 1px 2px; }"
               "QMenu::item:selected { background: %8; }"
               "QMenu::item:disabled { color: #6a6a6a; }"
               "QMenu::separator { height: 1px; background: #333333; margin: 5px 10px; }"

               // dialogs: settings, instance editor, new instance
               "QDialog { background: %4; }"
               "#pageSidebar { background: %3; border: none; border-radius: 14px; padding: 6px; outline: 0; }"
               "#pageSidebar::item { border-radius: 10px; padding: 8px 10px; margin: 2px 0; }"
               "#pageSidebar::item:hover { background: %6; }"
               "#pageSidebar::item:selected { background: %9; color: #ffffff; }"
               "#pageHeader { font-size: 18px; padding: 4px 0 8px 0; }"

               "QPushButton { background: %6; border: 1px solid #5a5a5a; border-radius: 9px; padding: 6px 16px; min-height: 18px; }"
               "QPushButton:hover { background: #5a5a5a; }"
               "QPushButton:pressed { background: %7; }"
               "QPushButton:disabled { color: #7a7a7a; background: %4; border-color: #4a4a4a; }"
               "QPushButton:default { border: 1px solid #9a9a9a; }"

               "QLineEdit, QSpinBox, QDoubleSpinBox, QComboBox, QPlainTextEdit, QTextEdit {"
               "  background: %3; border: 1px solid #555555; border-radius: 8px; padding: 4px 8px; selection-background-color: %9; }"
               "QLineEdit:focus, QSpinBox:focus, QDoubleSpinBox:focus, QComboBox:focus, QPlainTextEdit:focus, QTextEdit:focus {"
               "  border: 1px solid #9a9a9a; }"
               "QComboBox QAbstractItemView { background: %7; border: 1px solid #3a3a3a; selection-background-color: %9; outline: 0; }"

               "QGroupBox { border: 1px solid #505050; border-radius: 12px; margin-top: 14px; padding: 10px; }"
               "QGroupBox::title { subcontrol-origin: margin; left: 14px; padding: 0 6px; }"

               "QTabWidget::pane { border: 1px solid #505050; border-radius: 10px; top: -1px; }"
               "QTabBar::tab { background: transparent; padding: 7px 16px; margin: 2px; border-radius: 9px; }"
               "QTabBar::tab:hover { background: %6; }"
               "QTabBar::tab:selected { background: %9; color: #ffffff; }"

               "QProgressBar { background: %3; border: none; border-radius: 8px; text-align: center; min-height: 16px; }"
               "QProgressBar::chunk { background: %2; border-radius: 8px; }"

               "QScrollBar:vertical { background: transparent; width: 12px; margin: 2px; }"
               "QScrollBar::handle:vertical { background: #5a5a5a; border-radius: 4px; min-height: 30px; }"
               "QScrollBar::handle:vertical:hover { background: #7a7a7a; }"
               "QScrollBar:horizontal { background: transparent; height: 12px; margin: 2px; }"
               "QScrollBar::handle:horizontal { background: #5a5a5a; border-radius: 4px; min-width: 30px; }"
               "QScrollBar::handle:horizontal:hover { background: #7a7a7a; }"
               "QScrollBar::add-line, QScrollBar::sub-line { width: 0; height: 0; }"
               "QScrollBar::add-page, QScrollBar::sub-page { background: transparent; }")
        .arg(kCard, kAccent, kContent, kChrome, kDivider, kChromeHover, kCard, kCardHover, kSelected);
}
