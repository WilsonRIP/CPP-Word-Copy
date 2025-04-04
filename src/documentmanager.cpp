#include "documentmanager.h"

#include <QTextDocument>
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QApplication>
#include <QDebug>
#include <QStringConverter>

DocumentManager::DocumentManager(QObject *parent)
    : QObject(parent)
    , m_autoSaveTimer(new QTimer(this))
    , m_document(nullptr)
{
    connect(m_autoSaveTimer, &QTimer::timeout, this, &DocumentManager::autoSave);
}

DocumentManager::~DocumentManager()
{
    stopAutoSave();
}

void DocumentManager::setProperty(const QString &key, const QVariant &value)
{
    m_properties[key] = value;
    
    if (!m_currentFilePath.isEmpty()) {
        saveProperties(m_currentFilePath);
    }
}

QVariant DocumentManager::property(const QString &key) const
{
    return m_properties.value(key);
}

QMap<QString, QVariant> DocumentManager::allProperties() const
{
    return m_properties;
}

void DocumentManager::clearProperties()
{
    m_properties.clear();
    
    if (!m_currentFilePath.isEmpty()) {
        saveProperties(m_currentFilePath);
    }
}

void DocumentManager::startAutoSave(QTextDocument *document, const QString &filePath)
{
    stopAutoSave();
    
    if (!document || filePath.isEmpty()) {
        return;
    }
    
    m_document = document;
    m_currentFilePath = filePath;
    
    // Load properties if exists
    loadProperties(filePath);
    
    // Start auto-save timer
    m_autoSaveTimer->start(AUTO_SAVE_INTERVAL);
}

void DocumentManager::stopAutoSave()
{
    m_autoSaveTimer->stop();
    m_document = nullptr;
    m_currentFilePath.clear();
}

bool DocumentManager::hasRecoveryFile(const QString &filePath) const
{
    return QFile::exists(recoveryFilePath(filePath));
}

bool DocumentManager::recoverDocument(QTextDocument *document, const QString &filePath)
{
    if (!document || !hasRecoveryFile(filePath)) {
        return false;
    }
    
    QFile file(recoveryFilePath(filePath));
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        return false;
    }
    
    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);
    
    if (filePath.endsWith(".txt", Qt::CaseInsensitive)) {
        document->setPlainText(in.readAll());
    } else {
        document->setHtml(in.readAll());
    }
    
    file.close();
    
    // Also load document properties
    loadProperties(filePath);
    
    return true;
}

void DocumentManager::clearRecoveryFile(const QString &filePath)
{
    QFile::remove(recoveryFilePath(filePath));
}

QStringList DocumentManager::pendingRecoveryFiles() const
{
    QStringList result;
    
    QDir recoveryDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/recovery");
    if (!recoveryDir.exists()) {
        return result;
    }
    
    // Find all recovery files
    QStringList filters;
    filters << "*.recovery";
    QFileInfoList files = recoveryDir.entryInfoList(filters, QDir::Files);
    
    for (const QFileInfo &fileInfo : files) {
        QString fileName = fileInfo.fileName();
        // Remove the .recovery extension to get the original path
        result.append(fileName.left(fileName.length() - 9)); // ".recovery" is 9 chars
    }
    
    return result;
}

void DocumentManager::autoSave()
{
    try {
        if (!m_document || m_currentFilePath.isEmpty()) {
            return;
        }
        
        QFile file(recoveryFilePath(m_currentFilePath));
        QDir dir = QFileInfo(file).dir();
        
        if (!dir.exists()) {
            dir.mkpath(".");
        }
        
        if (file.open(QFile::WriteOnly | QFile::Text)) {
            QTextStream out(&file);
            out.setEncoding(QStringConverter::Utf8);
            
            if (m_currentFilePath.endsWith(".txt", Qt::CaseInsensitive)) {
                // Create a copy of text to avoid concurrent access issues
                QString text = m_document->toPlainText();
                out << text;
            } else {
                // Create a copy of HTML to avoid concurrent access issues
                QString html = m_document->toHtml();
                out << html;
            }
            
            file.close();
        }
    } catch (const std::exception& e) {
        qDebug() << "Exception in DocumentManager::autoSave:" << e.what();
    } catch (...) {
        qDebug() << "Unknown exception in DocumentManager::autoSave";
    }
}

QString DocumentManager::recoveryFilePath(const QString &originalPath) const
{
    QDir recoveryDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/recovery");
    if (!recoveryDir.exists()) {
        recoveryDir.mkpath(".");
    }
    
    // Create a safe filename based on the original path
    QString safeName = originalPath;
    safeName.replace("/", "_");
    safeName.replace("\\", "_");
    safeName.replace(":", "_");
    
    return recoveryDir.filePath(safeName + ".recovery");
}

QString DocumentManager::propertiesFilePath(const QString &originalPath) const
{
    QFileInfo fi(originalPath);
    return fi.path() + "/" + fi.completeBaseName() + ".meta";
}

bool DocumentManager::saveProperties(const QString &filePath)
{
    if (m_properties.isEmpty()) {
        // If properties are empty, just remove the file if it exists
        QFile::remove(propertiesFilePath(filePath));
        return true;
    }
    
    QJsonObject jsonObj;
    for (auto it = m_properties.constBegin(); it != m_properties.constEnd(); ++it) {
        jsonObj[it.key()] = QJsonValue::fromVariant(it.value());
    }
    
    QJsonDocument doc(jsonObj);
    
    QFile file(propertiesFilePath(filePath));
    if (!file.open(QFile::WriteOnly)) {
        return false;
    }
    
    file.write(doc.toJson());
    file.close();
    
    return true;
}

bool DocumentManager::loadProperties(const QString &filePath)
{
    m_properties.clear();
    
    QString propFile = propertiesFilePath(filePath);
    if (!QFile::exists(propFile)) {
        return false;
    }
    
    QFile file(propFile);
    if (!file.open(QFile::ReadOnly)) {
        return false;
    }
    
    QByteArray data = file.readAll();
    file.close();
    
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isObject()) {
        return false;
    }
    
    QJsonObject obj = doc.object();
    for (auto it = obj.constBegin(); it != obj.constEnd(); ++it) {
        m_properties[it.key()] = it.value().toVariant();
    }
    
    return true;
} 