#include "formatbar.h"

#include <QHBoxLayout>
#include <QIcon>
#include <QAction>

FormatBar::FormatBar(QWidget *parent)
    : QWidget(parent)
{
    m_toolbar = new QToolBar(this);
    m_toolbar->setIconSize(QSize(16, 16));
    
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_toolbar);
    setLayout(layout);
    
    // Font family
    m_fontComboBox = new QFontComboBox(m_toolbar);
    m_toolbar->addWidget(m_fontComboBox);
    
    // Font size
    m_fontSizeComboBox = new QComboBox(m_toolbar);
    m_fontSizeComboBox->setEditable(true);
    QStringList sizes;
    for (int i = 8; i <= 72; i += 2) {
        sizes.append(QString::number(i));
    }
    m_fontSizeComboBox->addItems(sizes);
    m_fontSizeComboBox->setCurrentText("12");
    m_toolbar->addWidget(m_fontSizeComboBox);
    
    m_toolbar->addSeparator();
    
    // Bold
    m_boldButton = new QToolButton(m_toolbar);
    m_boldButton->setIcon(QIcon::fromTheme("format-text-bold"));
    m_boldButton->setCheckable(true);
    m_toolbar->addWidget(m_boldButton);
    
    // Italic
    m_italicButton = new QToolButton(m_toolbar);
    m_italicButton->setIcon(QIcon::fromTheme("format-text-italic"));
    m_italicButton->setCheckable(true);
    m_toolbar->addWidget(m_italicButton);
    
    // Underline
    m_underlineButton = new QToolButton(m_toolbar);
    m_underlineButton->setIcon(QIcon::fromTheme("format-text-underline"));
    m_underlineButton->setCheckable(true);
    m_toolbar->addWidget(m_underlineButton);
    
    // Color
    m_colorButton = new QToolButton(m_toolbar);
    m_colorButton->setIcon(QIcon::fromTheme("format-text-color"));
    m_toolbar->addWidget(m_colorButton);
    
    m_toolbar->addSeparator();
    
    // Alignment
    m_alignLeftButton = new QToolButton(m_toolbar);
    m_alignLeftButton->setIcon(QIcon::fromTheme("format-justify-left"));
    m_alignLeftButton->setCheckable(true);
    m_toolbar->addWidget(m_alignLeftButton);
    
    m_alignCenterButton = new QToolButton(m_toolbar);
    m_alignCenterButton->setIcon(QIcon::fromTheme("format-justify-center"));
    m_alignCenterButton->setCheckable(true);
    m_toolbar->addWidget(m_alignCenterButton);
    
    m_alignRightButton = new QToolButton(m_toolbar);
    m_alignRightButton->setIcon(QIcon::fromTheme("format-justify-right"));
    m_alignRightButton->setCheckable(true);
    m_toolbar->addWidget(m_alignRightButton);
    
    m_alignJustifyButton = new QToolButton(m_toolbar);
    m_alignJustifyButton->setIcon(QIcon::fromTheme("format-justify-fill"));
    m_alignJustifyButton->setCheckable(true);
    m_toolbar->addWidget(m_alignJustifyButton);
} 