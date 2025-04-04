#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("CPP Word");
    app.setOrganizationName("CPP Word");
    
    MainWindow mainWindow;
    mainWindow.setWindowTitle("CPP Word");
    mainWindow.resize(1024, 768);
    mainWindow.show();
    
    return app.exec();
} 