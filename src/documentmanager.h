#ifndef DOCUMENTMANAGER_H
#define DOCUMENTMANAGER_H

#include <QObject>
#include <QDateTime>
#include <QTimer>
#include <QMap>
#include <QString>
#include <QVariant>

class QTextDocument;

class DocumentManager : public QObject
{
    Q_OBJECT

public:
    explicit DocumentManager(QObject *parent = nullptr);
    ~DocumentManager();

    // Document properties
    void setProperty(const QString &key, const QVariant &value);
    QVariant property(const QString &key) const;
    QMap<QString, QVariant> allProperties() const;
    void clearProperties();

    // Document recovery
    void startAutoSave(QTextDocument *document, const QString &filePath);
    void stopAutoSave();
    bool hasRecoveryFile(const QString &filePath) const;
    bool recoverDocument(QTextDocument *document, const QString &filePath);
    void clearRecoveryFile(const QString &filePath);
    QStringList pendingRecoveryFiles() const;

private slots:
    void autoSave();

private:
    QString recoveryFilePath(const QString &originalPath) const;
    QString propertiesFilePath(const QString &originalPath) const;
    bool saveProperties(const QString &filePath);
    bool loadProperties(const QString &filePath);

    QMap<QString, QVariant> m_properties;
    QTimer *m_autoSaveTimer;
    QTextDocument *m_document;
    QString m_currentFilePath;
    static const int AUTO_SAVE_INTERVAL = 30000; // 30 seconds
};

#endif // DOCUMENTMANAGER_H 