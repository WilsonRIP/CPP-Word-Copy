#include <QApplication>
#include <QGuiApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    // Must be called before QApplication
    QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    
    // Remove deprecated high DPI scaling attributes
    // In Qt 6.10.0, these are always enabled by default
    
    QApplication app(argc, argv);
    app.setApplicationName("CPP Word");
    app.setOrganizationName("CPP Word");
    
    // Set application-wide stylesheet to improve spacing
    app.setStyleSheet(
        "QToolBar { spacing: 8px; padding: 4px; }"
        "QToolButton { padding: 4px; margin: 2px; }"
        "QComboBox { padding-right: 6px; }"
        "QMenuBar { spacing: 10px; }"
        "QMenuBar::item { padding: 6px 12px; margin: 2px; }"
        "QMenu::item { padding: 4px 20px 4px 20px; }"
    );
    
    MainWindow mainWindow;
    mainWindow.setWindowTitle("CPP Word");
    mainWindow.resize(1024, 768);
    mainWindow.show();
    
    return app.exec();
} 