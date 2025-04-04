#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QFontComboBox>
#include <QComboBox>
#include <QAction>
#include <QTextEdit>
#include <QDockWidget>
#include <QPrinter>
#include <QSettings>

class TextEditor;
class FormatBar;
class QCloseEvent;
class DocumentManager;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // File operations
    void newDocument();
    void openDocument();
    bool saveDocument();
    bool saveAsDocument();
    void printDocument();
    void printPreviewDialog();
    void documentProperties();
    
    // Edit operations
    void cutText();
    void copyText();
    void pasteText();
    void undoAction();
    void redoAction();
    
    // Format operations
    void textBold();
    void textItalic();
    void textUnderline();
    void textColor();
    void paragraphAlign();
    
    // View operations
    void zoomIn();
    void zoomOut();
    void resetZoom();

    // Recovery
    void checkForRecoveryFiles();
    bool recoverDocument(const QString &filePath);

private:
    void setupUI();
    void createActions();
    void createMenus();
    void createToolbars();
    void setupConnections();
    void setupStatusBar();
    
    void saveSettings();
    void loadSettings();
    void updateWindowTitle();
    bool maybeSave();
    
    // Override
    void closeEvent(QCloseEvent *event) override;

    TextEditor *m_textEditor;
    FormatBar *m_formatBar;
    DocumentManager *m_documentManager;
    
    // Menus
    QMenu *m_fileMenu;
    QMenu *m_editMenu;
    QMenu *m_formatMenu;
    QMenu *m_viewMenu;
    QMenu *m_helpMenu;
    
    // Toolbars
    QToolBar *m_fileToolBar;
    QToolBar *m_editToolBar;
    QToolBar *m_formatToolBar;
    
    // Actions
    QAction *m_newAction;
    QAction *m_openAction;
    QAction *m_saveAction;
    QAction *m_saveAsAction;
    QAction *m_printAction;
    QAction *m_printPreviewAction;
    QAction *m_documentPropertiesAction;
    QAction *m_exitAction;
    
    QAction *m_undoAction;
    QAction *m_redoAction;
    QAction *m_cutAction;
    QAction *m_copyAction;
    QAction *m_pasteAction;
    
    QAction *m_boldAction;
    QAction *m_italicAction;
    QAction *m_underlineAction;
    QAction *m_colorAction;
    QAction *m_alignLeftAction;
    QAction *m_alignCenterAction;
    QAction *m_alignRightAction;
    QAction *m_alignJustifyAction;
    
    QAction *m_zoomInAction;
    QAction *m_zoomOutAction;
    QAction *m_resetZoomAction;
    
    QString m_currentFile;
};

#endif // MAINWINDOW_H 