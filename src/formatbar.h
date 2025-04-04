#ifndef FORMATBAR_H
#define FORMATBAR_H

#include <QWidget>
#include <QToolBar>
#include <QFontComboBox>
#include <QComboBox>
#include <QSpinBox>
#include <QToolButton>

class FormatBar : public QWidget
{
    Q_OBJECT
    
public:
    FormatBar(QWidget *parent = nullptr);
    
private:
    QToolBar *m_toolbar;
    QFontComboBox *m_fontComboBox;
    QComboBox *m_fontSizeComboBox;
    QToolButton *m_boldButton;
    QToolButton *m_italicButton;
    QToolButton *m_underlineButton;
    QToolButton *m_colorButton;
    QToolButton *m_alignLeftButton;
    QToolButton *m_alignCenterButton;
    QToolButton *m_alignRightButton;
    QToolButton *m_alignJustifyButton;
};

#endif // FORMATBAR_H 