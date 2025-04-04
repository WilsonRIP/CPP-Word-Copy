#include "mainwindow.h"
#include "texteditor.h"
#include "formatbar.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QTextDocumentWriter>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QColorDialog>
#include <QTextDocument>
#include <QTextCursor>
#include <QCloseEvent>
#include <QDebug>
#include <QLabel>
#include <QRegularExpression>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_textEditor(new TextEditor(this))
    , m_formatBar(new FormatBar(this))
    , m_currentFile("")
{
    setupUI();
    createActions();
    createMenus();
    createToolbars();
    setupConnections();
    setupStatusBar();
    
    loadSettings();
    
    setCentralWidget(m_textEditor);
    setWindowIcon(QIcon(":/icons/word.png"));
}

MainWindow::~MainWindow()
{
    saveSettings();
}

void MainWindow::setupUI()
{
    resize(1024, 768);
}

void MainWindow::createActions()
{
    // File actions
    m_newAction = new QAction(QIcon::fromTheme("document-new"), tr("&New"), this);
    m_newAction->setShortcut(QKeySequence::New);
    m_newAction->setStatusTip(tr("Create a new document"));
    
    m_openAction = new QAction(QIcon::fromTheme("document-open"), tr("&Open..."), this);
    m_openAction->setShortcut(QKeySequence::Open);
    m_openAction->setStatusTip(tr("Open an existing document"));
    
    m_saveAction = new QAction(QIcon::fromTheme("document-save"), tr("&Save"), this);
    m_saveAction->setShortcut(QKeySequence::Save);
    m_saveAction->setStatusTip(tr("Save the document"));
    
    m_saveAsAction = new QAction(QIcon::fromTheme("document-save-as"), tr("Save &As..."), this);
    m_saveAsAction->setShortcut(QKeySequence::SaveAs);
    m_saveAsAction->setStatusTip(tr("Save the document under a new name"));
    
    m_printAction = new QAction(QIcon::fromTheme("document-print"), tr("&Print..."), this);
    m_printAction->setShortcut(QKeySequence::Print);
    m_printAction->setStatusTip(tr("Print the document"));
    
    m_printPreviewAction = new QAction(QIcon::fromTheme("document-print-preview"), tr("Print Preview..."), this);
    m_printPreviewAction->setStatusTip(tr("Preview the document before printing"));
    
    m_exitAction = new QAction(QIcon::fromTheme("application-exit"), tr("E&xit"), this);
    m_exitAction->setShortcut(QKeySequence::Quit);
    m_exitAction->setStatusTip(tr("Exit the application"));
    
    // Edit actions
    m_undoAction = new QAction(QIcon::fromTheme("edit-undo"), tr("&Undo"), this);
    m_undoAction->setShortcut(QKeySequence::Undo);
    m_undoAction->setStatusTip(tr("Undo the last operation"));
    
    m_redoAction = new QAction(QIcon::fromTheme("edit-redo"), tr("&Redo"), this);
    m_redoAction->setShortcut(QKeySequence::Redo);
    m_redoAction->setStatusTip(tr("Redo the last operation"));
    
    m_cutAction = new QAction(QIcon::fromTheme("edit-cut"), tr("Cu&t"), this);
    m_cutAction->setShortcut(QKeySequence::Cut);
    m_cutAction->setStatusTip(tr("Cut the current selection to clipboard"));
    
    m_copyAction = new QAction(QIcon::fromTheme("edit-copy"), tr("&Copy"), this);
    m_copyAction->setShortcut(QKeySequence::Copy);
    m_copyAction->setStatusTip(tr("Copy the current selection to clipboard"));
    
    m_pasteAction = new QAction(QIcon::fromTheme("edit-paste"), tr("&Paste"), this);
    m_pasteAction->setShortcut(QKeySequence::Paste);
    m_pasteAction->setStatusTip(tr("Paste the clipboard's contents"));
    
    // Format actions
    m_boldAction = new QAction(QIcon::fromTheme("format-text-bold"), tr("&Bold"), this);
    m_boldAction->setShortcut(QKeySequence::Bold);
    m_boldAction->setStatusTip(tr("Make the text bold"));
    m_boldAction->setCheckable(true);
    
    m_italicAction = new QAction(QIcon::fromTheme("format-text-italic"), tr("&Italic"), this);
    m_italicAction->setShortcut(QKeySequence::Italic);
    m_italicAction->setStatusTip(tr("Make the text italic"));
    m_italicAction->setCheckable(true);
    
    m_underlineAction = new QAction(QIcon::fromTheme("format-text-underline"), tr("&Underline"), this);
    m_underlineAction->setShortcut(QKeySequence::Underline);
    m_underlineAction->setStatusTip(tr("Underline the text"));
    m_underlineAction->setCheckable(true);
    
    m_colorAction = new QAction(QIcon::fromTheme("format-text-color"), tr("&Color..."), this);
    m_colorAction->setStatusTip(tr("Change the text color"));
    
    m_alignLeftAction = new QAction(QIcon::fromTheme("format-justify-left"), tr("Align &Left"), this);
    m_alignLeftAction->setStatusTip(tr("Align text left"));
    m_alignLeftAction->setCheckable(true);
    
    m_alignCenterAction = new QAction(QIcon::fromTheme("format-justify-center"), tr("Align &Center"), this);
    m_alignCenterAction->setStatusTip(tr("Align text center"));
    m_alignCenterAction->setCheckable(true);
    
    m_alignRightAction = new QAction(QIcon::fromTheme("format-justify-right"), tr("Align &Right"), this);
    m_alignRightAction->setStatusTip(tr("Align text right"));
    m_alignRightAction->setCheckable(true);
    
    m_alignJustifyAction = new QAction(QIcon::fromTheme("format-justify-fill"), tr("&Justify"), this);
    m_alignJustifyAction->setStatusTip(tr("Justify text"));
    m_alignJustifyAction->setCheckable(true);
    
    // View actions
    m_zoomInAction = new QAction(QIcon::fromTheme("zoom-in"), tr("Zoom &In"), this);
    m_zoomInAction->setShortcut(QKeySequence::ZoomIn);
    m_zoomInAction->setStatusTip(tr("Zoom in"));
    
    m_zoomOutAction = new QAction(QIcon::fromTheme("zoom-out"), tr("Zoom &Out"), this);
    m_zoomOutAction->setShortcut(QKeySequence::ZoomOut);
    m_zoomOutAction->setStatusTip(tr("Zoom out"));
    
    m_resetZoomAction = new QAction(QIcon::fromTheme("zoom-original"), tr("&Reset Zoom"), this);
    m_resetZoomAction->setStatusTip(tr("Reset zoom to original size"));
}

void MainWindow::createMenus()
{
    // File Menu
    m_fileMenu = menuBar()->addMenu(tr("&File"));
    m_fileMenu->addAction(m_newAction);
    m_fileMenu->addAction(m_openAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_saveAction);
    m_fileMenu->addAction(m_saveAsAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_printAction);
    m_fileMenu->addAction(m_printPreviewAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_exitAction);
    
    // Edit Menu
    m_editMenu = menuBar()->addMenu(tr("&Edit"));
    m_editMenu->addAction(m_undoAction);
    m_editMenu->addAction(m_redoAction);
    m_editMenu->addSeparator();
    m_editMenu->addAction(m_cutAction);
    m_editMenu->addAction(m_copyAction);
    m_editMenu->addAction(m_pasteAction);
    
    // Format Menu
    m_formatMenu = menuBar()->addMenu(tr("F&ormat"));
    m_formatMenu->addAction(m_boldAction);
    m_formatMenu->addAction(m_italicAction);
    m_formatMenu->addAction(m_underlineAction);
    m_formatMenu->addAction(m_colorAction);
    m_formatMenu->addSeparator();
    
    QMenu *alignMenu = m_formatMenu->addMenu(tr("&Alignment"));
    alignMenu->addAction(m_alignLeftAction);
    alignMenu->addAction(m_alignCenterAction);
    alignMenu->addAction(m_alignRightAction);
    alignMenu->addAction(m_alignJustifyAction);
    
    // View Menu
    m_viewMenu = menuBar()->addMenu(tr("&View"));
    m_viewMenu->addAction(m_zoomInAction);
    m_viewMenu->addAction(m_zoomOutAction);
    m_viewMenu->addAction(m_resetZoomAction);
    
    // Help Menu
    m_helpMenu = menuBar()->addMenu(tr("&Help"));
    m_helpMenu->addAction(tr("&About"), this, [this]() {
        QMessageBox::about(this, tr("About CPP Word"),
                           tr("A simple Word clone created with Qt and C++."));
    });
}

void MainWindow::createToolbars()
{
    // File Toolbar
    m_fileToolBar = addToolBar(tr("File"));
    m_fileToolBar->addAction(m_newAction);
    m_fileToolBar->addAction(m_openAction);
    m_fileToolBar->addAction(m_saveAction);
    m_fileToolBar->addAction(m_printAction);
    
    // Edit Toolbar
    m_editToolBar = addToolBar(tr("Edit"));
    m_editToolBar->addAction(m_undoAction);
    m_editToolBar->addAction(m_redoAction);
    m_editToolBar->addSeparator();
    m_editToolBar->addAction(m_cutAction);
    m_editToolBar->addAction(m_copyAction);
    m_editToolBar->addAction(m_pasteAction);
    
    // Format Toolbar
    m_formatToolBar = addToolBar(tr("Format"));
    m_formatToolBar->addAction(m_boldAction);
    m_formatToolBar->addAction(m_italicAction);
    m_formatToolBar->addAction(m_underlineAction);
    m_formatToolBar->addAction(m_colorAction);
    m_formatToolBar->addSeparator();
    m_formatToolBar->addAction(m_alignLeftAction);
    m_formatToolBar->addAction(m_alignCenterAction);
    m_formatToolBar->addAction(m_alignRightAction);
    m_formatToolBar->addAction(m_alignJustifyAction);
    
    // Font combo box
    QFontComboBox *fontComboBox = new QFontComboBox(m_formatToolBar);
    m_formatToolBar->addWidget(fontComboBox);
    
    // Font size combo box
    QComboBox *fontSizeComboBox = new QComboBox(m_formatToolBar);
    fontSizeComboBox->setEditable(true);
    QStringList sizes;
    for (int i = 8; i <= 72; i += 2) {
        sizes.append(QString::number(i));
    }
    fontSizeComboBox->addItems(sizes);
    fontSizeComboBox->setCurrentText("12");
    m_formatToolBar->addWidget(fontSizeComboBox);
    
    // Connect font changes
    connect(fontComboBox, &QFontComboBox::currentFontChanged, [this](const QFont &font) {
        QTextCharFormat fmt;
        fmt.setFontFamilies(QStringList() << font.family());
        m_textEditor->mergeFormatOnWordOrSelection(fmt);
    });
    
    connect(fontSizeComboBox, &QComboBox::currentTextChanged, [this](const QString &size) {
        QTextCharFormat fmt;
        fmt.setFontPointSize(size.toFloat());
        m_textEditor->mergeFormatOnWordOrSelection(fmt);
    });
}

void MainWindow::setupConnections()
{
    // File actions
    connect(m_newAction, &QAction::triggered, this, &MainWindow::newDocument);
    connect(m_openAction, &QAction::triggered, this, &MainWindow::openDocument);
    connect(m_saveAction, &QAction::triggered, this, &MainWindow::saveDocument);
    connect(m_saveAsAction, &QAction::triggered, this, &MainWindow::saveAsDocument);
    connect(m_printAction, &QAction::triggered, this, &MainWindow::printDocument);
    connect(m_printPreviewAction, &QAction::triggered, this, &MainWindow::printPreviewDialog);
    connect(m_exitAction, &QAction::triggered, this, &QWidget::close);

    // Edit actions
    connect(m_undoAction, &QAction::triggered, this, &MainWindow::undoAction);
    connect(m_redoAction, &QAction::triggered, this, &MainWindow::redoAction);
    connect(m_cutAction, &QAction::triggered, this, &MainWindow::cutText);
    connect(m_copyAction, &QAction::triggered, this, &MainWindow::copyText);
    connect(m_pasteAction, &QAction::triggered, this, &MainWindow::pasteText);

    // Format actions
    connect(m_boldAction, &QAction::triggered, this, &MainWindow::textBold);
    connect(m_italicAction, &QAction::triggered, this, &MainWindow::textItalic);
    connect(m_underlineAction, &QAction::triggered, this, &MainWindow::textUnderline);
    connect(m_colorAction, &QAction::triggered, this, &MainWindow::textColor);
    
    connect(m_alignLeftAction, &QAction::triggered, [this]() {
        m_textEditor->setAlignment(Qt::AlignLeft);
        m_alignLeftAction->setChecked(true);
        m_alignCenterAction->setChecked(false);
        m_alignRightAction->setChecked(false);
        m_alignJustifyAction->setChecked(false);
    });
    
    connect(m_alignCenterAction, &QAction::triggered, [this]() {
        m_textEditor->setAlignment(Qt::AlignCenter);
        m_alignLeftAction->setChecked(false);
        m_alignCenterAction->setChecked(true);
        m_alignRightAction->setChecked(false);
        m_alignJustifyAction->setChecked(false);
    });
    
    connect(m_alignRightAction, &QAction::triggered, [this]() {
        m_textEditor->setAlignment(Qt::AlignRight);
        m_alignLeftAction->setChecked(false);
        m_alignCenterAction->setChecked(false);
        m_alignRightAction->setChecked(true);
        m_alignJustifyAction->setChecked(false);
    });
    
    connect(m_alignJustifyAction, &QAction::triggered, [this]() {
        m_textEditor->setAlignment(Qt::AlignJustify);
        m_alignLeftAction->setChecked(false);
        m_alignCenterAction->setChecked(false);
        m_alignRightAction->setChecked(false);
        m_alignJustifyAction->setChecked(true);
    });

    // View actions
    connect(m_zoomInAction, &QAction::triggered, this, &MainWindow::zoomIn);
    connect(m_zoomOutAction, &QAction::triggered, this, &MainWindow::zoomOut);
    connect(m_resetZoomAction, &QAction::triggered, this, &MainWindow::resetZoom);
    
    // Text editor connections for updating UI
    connect(m_textEditor, &TextEditor::cursorPositionChanged, [this]() {
        QTextCursor cursor = m_textEditor->textCursor();
        QTextCharFormat fmt = cursor.charFormat();
        
        m_boldAction->setChecked(fmt.fontWeight() == QFont::Bold);
        m_italicAction->setChecked(fmt.fontItalic());
        m_underlineAction->setChecked(fmt.fontUnderline());
        
        Qt::Alignment alignment = m_textEditor->alignment();
        m_alignLeftAction->setChecked(alignment == Qt::AlignLeft);
        m_alignCenterAction->setChecked(alignment == Qt::AlignCenter);
        m_alignRightAction->setChecked(alignment == Qt::AlignRight);
        m_alignJustifyAction->setChecked(alignment == Qt::AlignJustify);
    });
    
    connect(m_textEditor->document(), &QTextDocument::modificationChanged, [this](bool changed) {
        setWindowModified(changed);
        updateWindowTitle();
    });
}

void MainWindow::setupStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
    
    // Word count label
    QLabel *wordCountLabel = new QLabel(this);
    wordCountLabel->setText(tr("Words: 0"));
    statusBar()->addPermanentWidget(wordCountLabel);
    
    // Update word count when document changes
    connect(m_textEditor->document(), &QTextDocument::contentsChanged, [=]() {
        QString text = m_textEditor->toPlainText();
        int wordCount = text.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts).count();
        wordCountLabel->setText(tr("Words: %1").arg(wordCount));
    });
}

void MainWindow::updateWindowTitle()
{
    QString title = tr("CPP Word");
    if (!m_currentFile.isEmpty()) {
        QFileInfo fileInfo(m_currentFile);
        title = fileInfo.fileName() + "[*] - " + tr("CPP Word");
    }
    setWindowTitle(title);
}

void MainWindow::saveSettings()
{
    QSettings settings;
    settings.beginGroup("MainWindow");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("state", saveState());
    settings.endGroup();
}

void MainWindow::loadSettings()
{
    QSettings settings;
    settings.beginGroup("MainWindow");
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("state").toByteArray());
    settings.endGroup();
}

// File operations
void MainWindow::newDocument()
{
    if (maybeSave()) {
        m_textEditor->clear();
        m_currentFile.clear();
        updateWindowTitle();
        m_textEditor->document()->setModified(false);
    }
}

void MainWindow::openDocument()
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open Document"), "",
                           tr("HTML Documents (*.html *.htm);;Text Documents (*.txt);;Rich Text Documents (*.rtf);;All Files (*)"));
        
        if (!fileName.isEmpty()) {
            QFile file(fileName);
            if (file.open(QFile::ReadOnly | QFile::Text)) {
                QTextStream in(&file);
                
                if (fileName.endsWith(".html") || fileName.endsWith(".htm")) {
                    m_textEditor->setHtml(in.readAll());
                } else if (fileName.endsWith(".rtf")) {
                    m_textEditor->setHtml(in.readAll());
                } else {
                    m_textEditor->setPlainText(in.readAll());
                }
                
                file.close();
                m_currentFile = fileName;
                updateWindowTitle();
                m_textEditor->document()->setModified(false);
                statusBar()->showMessage(tr("File loaded"), 2000);
            }
        }
    }
}

bool MainWindow::saveDocument()
{
    if (m_currentFile.isEmpty()) {
        return saveAsDocument();
    }
    
    QTextDocumentWriter writer(m_currentFile);
    bool success = false;
    
    if (m_currentFile.endsWith(".html") || m_currentFile.endsWith(".htm")) {
        writer.setFormat("html");
        success = writer.write(m_textEditor->document());
    } else if (m_currentFile.endsWith(".rtf")) {
        writer.setFormat("rtf");
        success = writer.write(m_textEditor->document());
    } else {
        QFile file(m_currentFile);
        if (file.open(QFile::WriteOnly | QFile::Text)) {
            QTextStream out(&file);
            out << m_textEditor->toPlainText();
            success = true;
            file.close();
        }
    }
    
    if (success) {
        m_textEditor->document()->setModified(false);
        statusBar()->showMessage(tr("File saved"), 2000);
    }
    
    return success;
}

bool MainWindow::saveAsDocument()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"), "",
                        tr("HTML Documents (*.html *.htm);;Text Documents (*.txt);;Rich Text Documents (*.rtf)"));
    
    if (fileName.isEmpty()) {
        return false;
    }
    
    m_currentFile = fileName;
    updateWindowTitle();
    return saveDocument();
}

bool MainWindow::maybeSave()
{
    if (!m_textEditor->document()->isModified()) {
        return true;
    }
    
    QMessageBox::StandardButton ret = QMessageBox::warning(this, tr("Application"),
                                                          tr("The document has been modified.\nDo you want to save your changes?"),
                                                          QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    
    if (ret == QMessageBox::Save) {
        return saveDocument();
    } else if (ret == QMessageBox::Cancel) {
        return false;
    }
    
    return true;
}

void MainWindow::printDocument()
{
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog dialog(&printer, this);
    
    if (dialog.exec() == QDialog::Accepted) {
        m_textEditor->print(&printer);
    }
}

void MainWindow::printPreviewDialog()
{
    QPrinter printer(QPrinter::HighResolution);
    QPrintPreviewDialog preview(&printer, this);
    
    connect(&preview, &QPrintPreviewDialog::paintRequested, m_textEditor, &TextEditor::print);
    preview.exec();
}

// Edit operations
void MainWindow::cutText()
{
    m_textEditor->cut();
}

void MainWindow::copyText()
{
    m_textEditor->copy();
}

void MainWindow::pasteText()
{
    m_textEditor->paste();
}

void MainWindow::undoAction()
{
    m_textEditor->undo();
}

void MainWindow::redoAction()
{
    m_textEditor->redo();
}

// Format operations
void MainWindow::textBold()
{
    QTextCharFormat fmt;
    fmt.setFontWeight(m_boldAction->isChecked() ? QFont::Bold : QFont::Normal);
    m_textEditor->mergeFormatOnWordOrSelection(fmt);
}

void MainWindow::textItalic()
{
    QTextCharFormat fmt;
    fmt.setFontItalic(m_italicAction->isChecked());
    m_textEditor->mergeFormatOnWordOrSelection(fmt);
}

void MainWindow::textUnderline()
{
    QTextCharFormat fmt;
    fmt.setFontUnderline(m_underlineAction->isChecked());
    m_textEditor->mergeFormatOnWordOrSelection(fmt);
}

void MainWindow::textColor()
{
    QColor color = QColorDialog::getColor(m_textEditor->textColor(), this);
    if (color.isValid()) {
        QTextCharFormat fmt;
        fmt.setForeground(color);
        m_textEditor->mergeFormatOnWordOrSelection(fmt);
    }
}

void MainWindow::paragraphAlign()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action) {
        if (action == m_alignLeftAction) {
            m_textEditor->setAlignment(Qt::AlignLeft);
        } else if (action == m_alignCenterAction) {
            m_textEditor->setAlignment(Qt::AlignCenter);
        } else if (action == m_alignRightAction) {
            m_textEditor->setAlignment(Qt::AlignRight);
        } else if (action == m_alignJustifyAction) {
            m_textEditor->setAlignment(Qt::AlignJustify);
        }
    }
}

// View operations
void MainWindow::zoomIn()
{
    m_textEditor->zoomIn();
}

void MainWindow::zoomOut()
{
    m_textEditor->zoomOut();
}

void MainWindow::resetZoom()
{
    m_textEditor->resetZoom();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        saveSettings();
        event->accept();
    } else {
        event->ignore();
    }
} 