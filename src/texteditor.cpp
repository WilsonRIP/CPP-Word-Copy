#include "texteditor.h"

#include <QTextCursor>
#include <QTextBlock>
#include <QTextList>
#include <QDebug>

TextEditor::TextEditor(QWidget *parent)
    : QTextEdit(parent)
    , m_zoomFactor(1.0)
{
    // Set default settings
    setAcceptRichText(true);
    setAutoFormatting(QTextEdit::AutoAll);
    setUndoRedoEnabled(true);
    
    // Create a default document
    document()->setDefaultFont(QFont("Arial", 12));
    document()->setDocumentMargin(10);
    
    // Connect the cursor position changed signal to parent for updating UI
    connect(this, &QTextEdit::cursorPositionChanged, this, &TextEditor::cursorPositionChanged);
}

void TextEditor::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    QTextCursor cursor = textCursor();
    if (!cursor.hasSelection()) {
        cursor.select(QTextCursor::WordUnderCursor);
    }
    cursor.mergeCharFormat(format);
    setTextCursor(cursor);
}

void TextEditor::setAlignment(Qt::Alignment alignment)
{
    QTextCursor cursor = textCursor();
    QTextBlockFormat blockFormat = cursor.blockFormat();
    blockFormat.setAlignment(alignment);
    
    cursor.beginEditBlock();
    cursor.mergeBlockFormat(blockFormat);
    cursor.endEditBlock();
}

Qt::Alignment TextEditor::alignment() const
{
    return textCursor().blockFormat().alignment();
}

QColor TextEditor::textColor() const
{
    return textCursor().charFormat().foreground().color();
}

void TextEditor::resetZoom()
{
    m_zoomFactor = 1.0;
    zoomIn(0); // Reset zoom 
} 