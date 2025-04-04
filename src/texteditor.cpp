#include "texteditor.h"

#include <QTextCursor>
#include <QTextBlock>
#include <QTextList>
#include <QDebug>
#include <QApplication>
#include <QKeyEvent>

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
    
    // Remove the problematic connection that causes infinite recursion
    // The TextEdit's cursorPositionChanged signal is being connected to itself
    // This was causing stack overflow
}

void TextEditor::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    // Add error checking to prevent crashes
    try {
        QTextCursor cursor = textCursor();
        if (!cursor.isNull()) {
            if (!cursor.hasSelection()) {
                cursor.select(QTextCursor::WordUnderCursor);
            }
            cursor.mergeCharFormat(format);
            setTextCursor(cursor);
        }
    } catch (const std::exception& e) {
        qDebug() << "Exception in mergeFormatOnWordOrSelection:" << e.what();
    }
}

void TextEditor::setAlignment(Qt::Alignment alignment)
{
    try {
        QTextCursor cursor = textCursor();
        if (cursor.isNull()) {
            return;
        }
        
        QTextBlockFormat blockFormat = cursor.blockFormat();
        blockFormat.setAlignment(alignment);
        
        cursor.beginEditBlock();
        cursor.mergeBlockFormat(blockFormat);
        cursor.endEditBlock();
    } catch (const std::exception& e) {
        qDebug() << "Exception in setAlignment:" << e.what();
    }
}

Qt::Alignment TextEditor::alignment() const
{
    try {
        QTextCursor cursor = textCursor();
        if (!cursor.isNull()) {
            return cursor.blockFormat().alignment();
        }
    } catch (const std::exception& e) {
        qDebug() << "Exception in alignment:" << e.what();
    }
    
    // Default alignment if we can't get it
    return Qt::AlignLeft;
}

QColor TextEditor::textColor() const
{
    try {
        QTextCursor cursor = textCursor();
        if (!cursor.isNull()) {
            return cursor.charFormat().foreground().color();
        }
    } catch (const std::exception& e) {
        qDebug() << "Exception in textColor:" << e.what();
    }
    
    // Default color if we can't get it
    return QColor(Qt::black);
}

void TextEditor::resetZoom()
{
    m_zoomFactor = 1.0;
    zoomIn(0); // Reset zoom 
}

// Override keyPressEvent to catch and handle exceptions during typing
void TextEditor::keyPressEvent(QKeyEvent *event)
{
    try {
        // Call the base class implementation
        QTextEdit::keyPressEvent(event);
    } catch (const std::exception& e) {
        qDebug() << "Exception in keyPressEvent:" << e.what();
        event->accept(); // Mark the event as handled
    } catch (...) {
        qDebug() << "Unknown exception in keyPressEvent";
        event->accept(); // Mark the event as handled
    }
} 