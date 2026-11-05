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
#include <utility>

namespace {
// HyperCore palette: deep slate chrome, a lighter content panel, near-black cards and a cool gray accent.
constexpr QLatin1String kChrome("#1b1d23");       // window background behind everything
constexpr QLatin1String kRail("#252831");         // the rail on the left
constexpr QLatin1String kChromeHover("#343843");  // hover on rail buttons and list items
constexpr QLatin1String kDivider("#3a3e4a");      // thin borders
constexpr QLatin1String kContent("#2a2d36");      // the big rounded panel
constexpr QLatin1String kField("#1f2128");        // inputs and recessed areas
constexpr QLatin1String kCard("#14161b");         // instance card, menus
constexpr QLatin1String kCardHover("#2f333d");    // hover inside menus
constexpr QLatin1String kAccent("#aab2c0");       // cool gray accent
constexpr QLatin1String kSelected("#4a505e");     // selected item in lists and tabs
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
    return QObject::tr("The default HyperCore look: deep slate with rounded panels and a side rail.");
}

QPalette HyperCoreTheme::colorScheme()
{
    QPalette p;
    p.setColor(QPalette::Window, QColor(kChrome));
    p.setColor(QPalette::WindowText, QColor(232, 234, 240));
    p.setColor(QPalette::Base, QColor(kContent));
    p.setColor(QPalette::AlternateBase, QColor(36, 39, 47));
    p.setColor(QPalette::ToolTipBase, QColor(kCard));
    p.setColor(QPalette::ToolTipText, Qt::white);
    p.setColor(QPalette::Text, QColor(232, 234, 240));
    p.setColor(QPalette::Button, QColor(52, 56, 67));
    p.setColor(QPalette::ButtonText, QColor(232, 234, 240));
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
    // Qt style sheets. One window holds everything: a rounded rail, a rounded content panel and pages that open inside it.
    QString css = QStringLiteral(
        "QToolTip { color: #f0f2f6; background-color: @CARD@; border: 1px solid @DIVIDER@; border-radius: 6px; padding: 5px 8px; }"

        "QMainWindow, #hcRoot { background: @CHROME@; }"
        "InstanceView { background: transparent; border: none; }"

        // the rail
        "#railFrame { background: @RAIL@; border-radius: 30px; }"
        "#railButton { background: transparent; border: none; border-radius: 18px; }"
        "#railButton:hover { background: @HOVER@; }"
        "#railButton:pressed { background: @SELECTED@; }"
        "#railButton::menu-indicator { image: none; width: 0px; }"

        // top right: playtime, title
        "#titleLabel { font-size: 19px; font-weight: 600; color: #eceef3; }"
        "#playtimeLabel { font-size: 13px; color: #9aa1af; }"

        // pages that open inside the window (settings, instance editor, new instance)
        "#embeddedPanel { background: @CONTENT@; border-radius: 30px; }"
        "#embeddedPage { background: transparent; }"

        // menus
        "QMenu { background: @CARD@; border: 1px solid @DIVIDER@; border-radius: 14px; padding: 8px; }"
        "QMenu::item { padding: 8px 28px 8px 16px; border-radius: 9px; margin: 1px 2px; }"
        "QMenu::item:selected { background: @CARDHOVER@; }"
        "QMenu::item:disabled { color: #636978; }"
        "QMenu::separator { height: 1px; background: @DIVIDER@; margin: 6px 12px; }"

        // page navigation strip
        "#pageSidebar { background: @FIELD@; border: none; border-radius: 20px; padding: 5px 7px; outline: 0; }"
        "#pageSidebar::item { border-radius: 15px; padding: 8px 16px; margin: 2px 3px; color: #c4c9d4; }"
        "#pageSidebar::item:hover { background: @HOVER@; }"
        "#pageSidebar::item:selected { background: @SELECTED@; color: #ffffff; }"
        "#pageHeader { font-size: 20px; font-weight: 600; padding: 6px 2px 8px 2px; }"

        "QPushButton { background: @HOVER@; border: none; border-radius: 12px; padding: 8px 20px; min-height: 18px; }"
        "QPushButton:hover { background: @SELECTED@; }"
        "QPushButton:pressed { background: @FIELD@; }"
        "QPushButton:disabled { color: #636978; background: @FIELD@; }"
        "QPushButton:default { background: @SELECTED@; }"

        "QLineEdit, QSpinBox, QDoubleSpinBox, QComboBox, QPlainTextEdit, QTextEdit {"
        "  background: @FIELD@; border: 1px solid @DIVIDER@; border-radius: 10px; padding: 5px 10px; selection-background-color: @SELECTED@; }"
        "QLineEdit:focus, QSpinBox:focus, QDoubleSpinBox:focus, QComboBox:focus, QPlainTextEdit:focus, QTextEdit:focus {"
        "  border: 1px solid @ACCENT@; }"
        "QComboBox::drop-down { border: none; width: 24px; }"
        "QComboBox QAbstractItemView { background: @CARD@; border: 1px solid @DIVIDER@; border-radius: 10px; selection-background-color: @SELECTED@; outline: 0; }"

        "QListView, QTreeView, QTableView { background: @FIELD@; border: 1px solid @DIVIDER@; border-radius: 12px; outline: 0; }"
        "QListView::item:selected, QTreeView::item:selected, QTableView::item:selected { background: @SELECTED@; color: #ffffff; }"
        "QHeaderView::section { background: @RAIL@; border: none; padding: 6px 8px; }"

        "QGroupBox { border: 1px solid @DIVIDER@; border-radius: 14px; margin-top: 16px; padding: 12px; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 16px; padding: 0 6px; }"

        "QTabWidget::pane { border: 1px solid @DIVIDER@; border-radius: 12px; top: -1px; }"
        "QTabBar::tab { background: transparent; padding: 8px 18px; margin: 3px; border-radius: 12px; }"
        "QTabBar::tab:hover { background: @HOVER@; }"
        "QTabBar::tab:selected { background: @SELECTED@; color: #ffffff; }"

        "QCheckBox::indicator, QRadioButton::indicator { width: 16px; height: 16px; border-radius: 5px; border: 1px solid #5a6070; background: @FIELD@; }"
        "QRadioButton::indicator { border-radius: 9px; }"
        "QCheckBox::indicator:checked, QRadioButton::indicator:checked { background: @ACCENT@; border: 1px solid @ACCENT@; }"

        "QProgressBar { background: @FIELD@; border: none; border-radius: 9px; text-align: center; min-height: 18px; }"
        "QProgressBar::chunk { background: @ACCENT@; border-radius: 9px; }"

        "QScrollBar:vertical { background: transparent; width: 12px; margin: 2px; }"
        "QScrollBar::handle:vertical { background: #4a505e; border-radius: 4px; min-height: 30px; }"
        "QScrollBar::handle:vertical:hover { background: #6a7284; }"
        "QScrollBar:horizontal { background: transparent; height: 12px; margin: 2px; }"
        "QScrollBar::handle:horizontal { background: #4a505e; border-radius: 4px; min-width: 30px; }"
        "QScrollBar::handle:horizontal:hover { background: #6a7284; }"
        "QScrollBar::add-line, QScrollBar::sub-line { width: 0; height: 0; }"
        "QScrollBar::add-page, QScrollBar::sub-page { background: transparent; }");

    const std::pair<QLatin1String, QLatin1String> colors[] = {
        { QLatin1String("@CARDHOVER@"), kCardHover }, { QLatin1String("@CARD@"), kCard },       { QLatin1String("@CHROME@"), kChrome },
        { QLatin1String("@CONTENT@"), kContent },     { QLatin1String("@DIVIDER@"), kDivider }, { QLatin1String("@RAIL@"), kRail },
        { QLatin1String("@HOVER@"), kChromeHover },   { QLatin1String("@FIELD@"), kField },     { QLatin1String("@SELECTED@"), kSelected },
        { QLatin1String("@ACCENT@"), kAccent },
    };
    for (const auto& [key, value] : colors) {
        css.replace(key, value);
    }
    return css;
}
