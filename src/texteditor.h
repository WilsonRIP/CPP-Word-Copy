#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QTextEdit>
#include <QTextCharFormat>

class TextEditor : public QTextEdit
{
    Q_OBJECT
    
public:
    TextEditor(QWidget *parent = nullptr);
    
    void mergeFormatOnWordOrSelection(const QTextCharFormat &format);
    void setAlignment(Qt::Alignment alignment);
    Qt::Alignment alignment() const;
    QColor textColor() const;
    void resetZoom();
    
protected:
    // Override keyPressEvent to handle exceptions
    void keyPressEvent(QKeyEvent *event) override;
    
private:
    float m_zoomFactor;
};

#endif // TEXTEDITOR_H 