#include "mainwindow.h"
#include "texteditor.h"
#include "formatbar.h"
#include "documentmanager.h"

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
#include <QInputDialog>
#include <QDialog>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QStandardPaths>
#include <QDir>
#include <QAction>
#include <QApplication>
#include <QComboBox>
#include <QFontComboBox>
#include <QMenuBar>
#include <QPrinter>
#include <QSettings>
#include <QStatusBar>
#include <QStringConverter>
#include <QToolBar>
#include <QTextEdit>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_textEditor(new TextEditor(this))
    , m_formatBar(new FormatBar(this))
    , m_documentManager(new DocumentManager(this))
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
    
    // Check for recovery files on startup
    checkForRecoveryFiles();
}

MainWindow::~MainWindow()
{
    saveSettings();
}

void MainWindow::setupUI()
{
    resize(1024, 768);// slightly smaller overall window for compact design
    // Improve menu bar spacing and add a subtle bold and padding for better visual appeal
    menuBar()->setStyleSheet("QMenuBar::item { margin-right: 10px; padding: 6px; font-weight: bold; }");;
    
    // Update main window style for separators and status bar with a modern look
    setStyleSheet(
        "QMainWindow::separator { width: 8px; height: 8px; background: #dcdcdc; }"
        "QStatusBar { background: #f0f0f0; border-top: 1px solid #ccc; }"dding: 1px; }"
        "QStatusBar::item { border: none; padding: 2px 4px; }"
    );
    
    setToolButtonStyle(Qt::ToolButtonIconOnly);
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
    
    m_documentPropertiesAction = new QAction(QIcon::fromTheme("document-properties"), tr("Document Proper&ties..."), this);
    m_documentPropertiesAction->setStatusTip(tr("View and edit document properties"));
    
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
    // Set menu bar spacing
    menuBar()->setDefaultUp(false);
    menuBar()->setNativeMenuBar(false); // Use non-native menu bar for consistent styling
    
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
    m_fileMenu->addAction(m_documentPropertiesAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_exitAction);
    
    // Add spacing widget between File and Edit
    QWidget* menuSpacer1 = new QWidget(this);
    menuSpacer1->setFixedWidth(10);
    menuBar()->setCornerWidget(menuSpacer1, Qt::TopLeftCorner);
    
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
    // Use compact icon size and reduced spacing
    setToolButtonStyle(Qt::ToolButtonIconOnly);
    
    // File Toolbar - reduce icon size for compactness
    m_fileToolBar = addToolBar(tr("File"));
    m_fileToolBar->setIconSize(QSize(24, 24)); // smaller iconsct design
    m_fileToolBar->setMovable(true);
    m_fileToolBar->setFloatable(true);
    m_fileToolBar->setAllowedAreas(Qt::AllToolBarAreas);
    
    // Use a tighter spacer
    QWidget* fileToolbarSpacer = new QWidget(m_fileToolBar);
    fileToolbarSpacer->setMinimumWidth(4); // Reduce spacer width
    
    m_fileToolBar->addAction(m_newAction);
    m_fileToolBar->addWidget(fileToolbarSpacer);
    m_fileToolBar->addAction(m_openAction);
    m_fileToolBar->addAction(m_saveAction);
    m_fileToolBar->addAction(m_printAction);
    
    // Edit Toolbar
    m_editToolBar = addToolBar(tr("Edit"));
    m_editToolBar->setIconSize(QSize(24, 24)); // Reduce icon size for compact design
    m_editToolBar->setMovable(true);
    m_editToolBar->setFloatable(true);
    m_editToolBar->setAllowedAreas(Qt::AllToolBarAreas);
    
    m_editToolBar->addAction(m_undoAction);
    m_editToolBar->addAction(m_redoAction);
    m_editToolBar->addSeparator();
    m_editToolBar->addAction(m_cutAction);
    m_editToolBar->addAction(m_copyAction);
    m_editToolBar->addAction(m_pasteAction);
    
    // Format Toolbar
    m_formatToolBar = addToolBar(tr("Format"));
    m_formatToolBar->setIconSize(QSize(24, 24)); // Reduce icon size for compact design
    m_formatToolBar->setMovable(true);
    m_formatToolBar->setFloatable(true);
    m_formatToolBar->setAllowedAreas(Qt::AllToolBarAreas);
    
    m_formatToolBar->addAction(m_boldAction);
    m_formatToolBar->addAction(m_italicAction);
    m_formatToolBar->addAction(m_underlineAction);
    m_formatToolBar->addAction(m_colorAction);
    m_formatToolBar->addSeparator();
    m_formatToolBar->addAction(m_alignLeftAction);
    m_formatToolBar->addAction(m_alignCenterAction);
    m_formatToolBar->addAction(m_alignRightAction);
    m_formatToolBar->addAction(m_alignJustifyAction);
    
    // Font combo box with reduced width for a compact UI
    QFontComboBox *fontComboBox = new QFontComboBox(m_formatToolBar);
    fontComboBox->setMinimumWidth(140); // Reduce width
    m_formatToolBar->addWidget(fontComboBox);
    
    // Compact spacer between widgets
    QWidget* spacer = new QWidget(m_formatToolBar);
    spacer->setMinimumWidth(4);// Reduce spacer width
    m_formatToolBar->addWidget(spacer);
    
    // Font size combo box with reduced minimum width
    QComboBox *fontSizeComboBox = new QComboBox(m_formatToolBar);
    fontSizeComboBox->setEditable(true);
    fontSizeComboBox->setMinimumWidth(50); // Reduce width
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
    connect(m_documentPropertiesAction, &QAction::triggered, this, &MainWindow::documentProperties);
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
    connect(m_textEditor, &QTextEdit::cursorPositionChanged, [this]() {
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
        
        // Stop auto-save
        m_documentManager->stopAutoSave();
    }
}

void MainWindow::openDocument()
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open Document"), "",
                           tr("All Supported Files (*.txt *.html *.htm *.rtf);;Text Documents (*.txt);;HTML Documents (*.html *.htm);;Rich Text Documents (*.rtf);;All Files (*)"));
        
        if (!fileName.isEmpty()) {
            QFile file(fileName);
            if (file.open(QFile::ReadOnly | QFile::Text)) {
                QTextStream in(&file);
                in.setEncoding(QStringConverter::Utf8);
                
                if (fileName.endsWith(".html", Qt::CaseInsensitive) || fileName.endsWith(".htm", Qt::CaseInsensitive)) {
                    m_textEditor->setHtml(in.readAll());
                } else if (fileName.endsWith(".rtf", Qt::CaseInsensitive)) {
                    m_textEditor->setHtml(in.readAll());
                } else {
                    m_textEditor->setPlainText(in.readAll());
                }
                
                file.close();
                m_currentFile = fileName;
                updateWindowTitle();
                m_textEditor->document()->setModified(false);
                
                // Start auto-save for crash recovery
                m_documentManager->startAutoSave(m_textEditor->document(), m_currentFile);
                
                statusBar()->showMessage(tr("File loaded"), 2000);
            } else {
                QMessageBox::warning(this, tr("Open Error"),
                                   tr("Could not open file %1: %2")
                                   .arg(fileName, file.errorString()));
            }
        }
    }
}

bool MainWindow::saveDocument()
{
    if (m_currentFile.isEmpty()) {
        return saveAsDocument();
    }
    
    // Determine the file format
    QString format;
    if (m_currentFile.endsWith(".html", Qt::CaseInsensitive) || m_currentFile.endsWith(".htm", Qt::CaseInsensitive)) {
        format = "html";
    } else if (m_currentFile.endsWith(".rtf", Qt::CaseInsensitive)) {
        format = "rtf";
    } else {
        format = "txt";
    }
    
    bool success = false;
    
    if (format == "html" || format == "rtf") {
        QTextDocumentWriter writer(m_currentFile);
        writer.setFormat(format.toUtf8());
        success = writer.write(m_textEditor->document());
    } else {
        QFile file(m_currentFile);
        if (file.open(QFile::WriteOnly | QFile::Text)) {
            QTextStream out(&file);
            out.setEncoding(QStringConverter::Utf8);
            out << m_textEditor->toPlainText();
            success = true;
            file.close();
        }
    }
    
    if (success) {
        m_textEditor->document()->setModified(false);
        statusBar()->showMessage(tr("File saved"), 2000);
        
        // Update auto-save
        m_documentManager->startAutoSave(m_textEditor->document(), m_currentFile);
    } else {
        QMessageBox::warning(this, tr("Save Error"),
                           tr("Could not save file %1")
                           .arg(m_currentFile));
    }
    
    return success;
}

bool MainWindow::saveAsDocument()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"), "",
                        tr("Text Documents (*.txt);;HTML Documents (*.html *.htm);;Rich Text Documents (*.rtf);;All Files (*)"));
    
    if (fileName.isEmpty()) {
        return false;
    }
    
    // Add extension if not present
    QFileInfo fileInfo(fileName);
    if (fileInfo.suffix().isEmpty()) {
        fileName += ".txt"; // Default to .txt
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
        // Stop auto-save and clean up
        m_documentManager->stopAutoSave();
        
        // Save current settings
        saveSettings();
        
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::checkForRecoveryFiles()
{
    QStringList recoveryFiles = m_documentManager->pendingRecoveryFiles();
    
    if (recoveryFiles.isEmpty()) {
        return;
    }
    
    QMessageBox msgBox(this);
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setWindowTitle(tr("Document Recovery"));
    msgBox.setText(tr("The application was not closed properly last time."));
    msgBox.setInformativeText(tr("Would you like to recover unsaved documents?"));
    QPushButton *recoverButton = msgBox.addButton(tr("Recover"), QMessageBox::ActionRole);
    msgBox.addButton(tr("Discard"), QMessageBox::RejectRole);
    
    msgBox.exec();
    
    if (msgBox.clickedButton() == recoverButton) {
        if (recoveryFiles.size() == 1) {
            recoverDocument(recoveryFiles.first());
        } else {
            // If there are multiple recovery files, let user choose
            QStringList items;
            for (const QString &file : recoveryFiles) {
                QFileInfo fileInfo(file);
                items << fileInfo.fileName();
            }
            
            bool ok;
            QString item = QInputDialog::getItem(this, tr("Select Document to Recover"),
                                               tr("Document:"), items, 0, false, &ok);
            if (ok && !item.isEmpty()) {
                int index = items.indexOf(item);
                if (index >= 0 && index < recoveryFiles.size()) {
                    recoverDocument(recoveryFiles.at(index));
                }
            }
        }
    } else {
        // Discard all recovery files
        for (const QString &file : recoveryFiles) {
            m_documentManager->clearRecoveryFile(file);
        }
    }
}

bool MainWindow::recoverDocument(const QString &filePath)
{
    if (m_documentManager->hasRecoveryFile(filePath)) {
        if (maybeSave()) {
            bool recovered = m_documentManager->recoverDocument(m_textEditor->document(), filePath);
            
            if (recovered) {
                m_currentFile = filePath;
                updateWindowTitle();
                m_textEditor->document()->setModified(true);
                
                // Start auto-save for this document
                m_documentManager->startAutoSave(m_textEditor->document(), m_currentFile);
                
                // Clear the recovery file
                m_documentManager->clearRecoveryFile(filePath);
                
                statusBar()->showMessage(tr("Document recovered"), 2000);
                return true;
            }
        }
    }
    
    return false;
}

void MainWindow::documentProperties()
{
    if (m_currentFile.isEmpty()) {
        QMessageBox::information(this, tr("Document Properties"), 
                                tr("Please save the document first."));
        return;
    }
    
    // Create a dialog for document properties
    QDialog dialog(this);
    dialog.setWindowTitle(tr("Document Properties"));
    dialog.resize(400, 300);
    
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    
    // Create a table widget for properties
    QTableWidget *tableWidget = new QTableWidget(&dialog);
    tableWidget->setColumnCount(2);
    tableWidget->setHorizontalHeaderLabels({tr("Property"), tr("Value")});
    tableWidget->horizontalHeader()->setStretchLastSection(true);
    
    // Get current properties
    QMap<QString, QVariant> properties = m_documentManager->allProperties();
    
    // Add system properties
    QFileInfo fileInfo(m_currentFile);
    properties["File Name"] = fileInfo.fileName();
    properties["File Path"] = fileInfo.absolutePath();
    properties["Size"] = QString::number(fileInfo.size()) + " bytes";
    properties["Created"] = fileInfo.birthTime().toString();
    properties["Modified"] = fileInfo.lastModified().toString();
    
    // Populate the table
    tableWidget->setRowCount(properties.size());
    int row = 0;
    for (auto it = properties.constBegin(); it != properties.constEnd(); ++it, ++row) {
        tableWidget->setItem(row, 0, new QTableWidgetItem(it.key()));
        tableWidget->setItem(row, 1, new QTableWidgetItem(it.value().toString()));
    }
    
    layout->addWidget(tableWidget);
    
    // Add custom property section
    QHBoxLayout *addPropLayout = new QHBoxLayout();
    QLabel *propLabel = new QLabel(tr("Add Property:"), &dialog);
    QLineEdit *propNameEdit = new QLineEdit(&dialog);
    QLineEdit *propValueEdit = new QLineEdit(&dialog);
    QPushButton *addButton = new QPushButton(tr("Add"), &dialog);
    
    addPropLayout->addWidget(propLabel);
    addPropLayout->addWidget(propNameEdit);
    addPropLayout->addWidget(propValueEdit);
    addPropLayout->addWidget(addButton);
    
    layout->addLayout(addPropLayout);
    
    // Add remove property button
    QPushButton *removeButton = new QPushButton(tr("Remove Selected Property"), &dialog);
    layout->addWidget(removeButton);
    
    // Add standard dialog buttons
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    layout->addWidget(buttonBox);
    
    // Connect signals/slots
    connect(addButton, &QPushButton::clicked, [&]() {
        QString name = propNameEdit->text().trimmed();
        QString value = propValueEdit->text().trimmed();
        
        if (!name.isEmpty()) {
            // Skip system properties
            QStringList systemProps = {"File Name", "File Path", "Size", "Created", "Modified"};
            if (systemProps.contains(name)) {
                QMessageBox::warning(&dialog, tr("Invalid Property"), 
                                    tr("Cannot add or modify system properties."));
                return;
            }
            
            // Add to the table
            bool found = false;
            for (int i = 0; i < tableWidget->rowCount(); ++i) {
                if (tableWidget->item(i, 0)->text() == name) {
                    tableWidget->item(i, 1)->setText(value);
                    found = true;
                    break;
                }
            }
            
            if (!found) {
                int row = tableWidget->rowCount();
                tableWidget->setRowCount(row + 1);
                tableWidget->setItem(row, 0, new QTableWidgetItem(name));
                tableWidget->setItem(row, 1, new QTableWidgetItem(value));
            }
            
            propNameEdit->clear();
            propValueEdit->clear();
        }
    });
    
    connect(removeButton, &QPushButton::clicked, [&]() {
        int currentRow = tableWidget->currentRow();
        if (currentRow >= 0) {
            QString propName = tableWidget->item(currentRow, 0)->text();
            
            // Skip system properties
            QStringList systemProps = {"File Name", "File Path", "Size", "Created", "Modified"};
            if (systemProps.contains(propName)) {
                QMessageBox::warning(&dialog, tr("Invalid Operation"), 
                                    tr("Cannot remove system properties."));
                return;
            }
            
            tableWidget->removeRow(currentRow);
        }
    });
    
    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    
    // Show the dialog
    if (dialog.exec() == QDialog::Accepted) {
        // Save the properties
        m_documentManager->clearProperties();
        
        // Skip system properties when saving
        QStringList systemProps = {"File Name", "File Path", "Size", "Created", "Modified"};
        
        for (int i = 0; i < tableWidget->rowCount(); ++i) {
            QString name = tableWidget->item(i, 0)->text();
            QString value = tableWidget->item(i, 1)->text();
            
            if (!systemProps.contains(name)) {
                m_documentManager->setProperty(name, value);
            }
        }
    }
}