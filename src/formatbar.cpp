#include "formatbar.h"

#include <QHBoxLayout>
#include <QIcon>
#include <QAction>

FormatBar::FormatBar(QWidget *parent)
    : QWidget(parent)
{
    m_toolbar = new QToolBar(this);
    m_toolbar->setIconSize(QSize(28, 28));
    m_toolbar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    // Increase margins around the toolbar for a more balanced look
    m_toolbar->setContentsMargins(10, 6, 10, 6);
    m_toolbar->setStyleSheet("QToolButton { margin: 4px; padding: 4px; }");
    
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(4, 4, 4, 4);
    layout->setSpacing(4);
    layout->addWidget(m_toolbar);
    setLayout(layout);
    
    // Font family
    m_fontComboBox = new QFontComboBox(m_toolbar);
    m_fontComboBox->setMinimumWidth(180);
    m_toolbar->addWidget(m_fontComboBox);
    
    // Add a larger spacer
    QWidget* spacer = new QWidget(m_toolbar);
    spacer->setFixedWidth(10);
    m_toolbar->addWidget(spacer);
    
    // Font size
    m_fontSizeComboBox = new QComboBox(m_toolbar);
    m_fontSizeComboBox->setEditable(true);
    m_fontSizeComboBox->setMinimumWidth(70);
    QStringList sizes;
    for (int i = 8; i <= 72; i += 2) {
        sizes.append(QString::number(i));
    }
    m_fontSizeComboBox->addItems(sizes);
    m_fontSizeComboBox->setCurrentText("12");
    m_toolbar->addWidget(m_fontSizeComboBox);
    
    // Add more space between sections
    QWidget* sectionSpacer1 = new QWidget(m_toolbar);
    sectionSpacer1->setFixedWidth(15);
    m_toolbar->addWidget(sectionSpacer1);
    
    // Bold
    m_boldButton = new QToolButton(m_toolbar);
    m_boldButton->setIcon(QIcon::fromTheme("format-text-bold"));
    m_boldButton->setCheckable(true);
    m_boldButton->setIconSize(QSize(24, 24));
    m_toolbar->addWidget(m_boldButton);
    
    // Italic
    m_italicButton = new QToolButton(m_toolbar);
    m_italicButton->setIcon(QIcon::fromTheme("format-text-italic"));
    m_italicButton->setCheckable(true);
    m_italicButton->setIconSize(QSize(24, 24));
    m_toolbar->addWidget(m_italicButton);
    
    // Underline
    m_underlineButton = new QToolButton(m_toolbar);
    m_underlineButton->setIcon(QIcon::fromTheme("format-text-underline"));
    m_underlineButton->setCheckable(true);
    m_underlineButton->setIconSize(QSize(24, 24));
    m_toolbar->addWidget(m_underlineButton);
    
    // Color
    m_colorButton = new QToolButton(m_toolbar);
    m_colorButton->setIcon(QIcon::fromTheme("format-text-color"));
    m_colorButton->setIconSize(QSize(24, 24));
    m_toolbar->addWidget(m_colorButton);
    
    // Add more space between sections
    QWidget* sectionSpacer2 = new QWidget(m_toolbar);
    sectionSpacer2->setFixedWidth(15);
    m_toolbar->addWidget(sectionSpacer2);
    
    // Alignment
    m_alignLeftButton = new QToolButton(m_toolbar);
    m_alignLeftButton->setIcon(QIcon::fromTheme("format-justify-left"));
    m_alignLeftButton->setCheckable(true);
    m_alignLeftButton->setIconSize(QSize(24, 24));
    m_toolbar->addWidget(m_alignLeftButton);
    
    m_alignCenterButton = new QToolButton(m_toolbar);
    m_alignCenterButton->setIcon(QIcon::fromTheme("format-justify-center"));
    m_alignCenterButton->setCheckable(true);
    m_alignCenterButton->setIconSize(QSize(24, 24));
    m_toolbar->addWidget(m_alignCenterButton);
    
    m_alignRightButton = new QToolButton(m_toolbar);
    m_alignRightButton->setIcon(QIcon::fromTheme("format-justify-right"));
    m_alignRightButton->setCheckable(true);
    m_alignRightButton->setIconSize(QSize(24, 24));
    m_toolbar->addWidget(m_alignRightButton);
    
    m_alignJustifyButton = new QToolButton(m_toolbar);
    m_alignJustifyButton->setIcon(QIcon::fromTheme("format-justify-fill"));
    m_alignJustifyButton->setCheckable(true);
    m_alignJustifyButton->setIconSize(QSize(24, 24));
    m_toolbar->addWidget(m_alignJustifyButton);
}