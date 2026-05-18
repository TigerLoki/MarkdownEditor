#include "core/FileManager.h"

#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QUrl>

FileManager::FileManager(Document& document, QObject* parent)
    : QObject(parent)
    , document_(document)
{
}

void FileManager::newFile() const {
    document_.reset();
}

void FileManager::openFile(const QString& path) {
    const QString localPath = QUrl(path).toLocalFile();
    QFile file(localPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        emit errorOccurred(tr("Cannot open file %1").arg(path));
        return;
    }
    QTextStream text_stream(&file);
    text_stream.setEncoding(QStringConverter::Utf8);
    document_.setText(text_stream.readAll());
    document_.markClean(path);
}

void FileManager::saveFile() {
    if (document_.filePath().isEmpty()) {
        emit errorOccurred("No file path");
        return;
    }
    doSaveFile(document_.filePath(), document_.text());
    document_.markClean(document_.filePath());
}

void FileManager::saveFileAs(const QString& path) {
    doSaveFile(path, document_.text());
    document_.markClean(path);
}

void FileManager::exportHtml(const QString& path) {
    doSaveFile(path, wrapHtml(document_.text()));
}

QString FileManager::wrapHtml(const QString& body) {
    return QString(
        "<!DOCTYPE html>\n"
        "<html><head><meta charset=\"utf-8\"></head>\n"
        "<body>\n%1\n</body></html>"
    ).arg(body);
}

void FileManager::doSaveFile(const QString& path, const QString& string) {
    const QString localPath = QUrl(path).toLocalFile();
    const QFileInfo fileInfo(localPath);
    if (const QDir dir = fileInfo.absoluteDir(); !dir.exists()) {
        if (!dir.mkpath(".")) {
            emit errorOccurred(tr("Cannot create directory: %1").arg(dir.absolutePath()));
            return;
        }
    }

    QFile file(localPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        emit errorOccurred("Cannot open or create file");
        return;
    }

    QTextStream out(&file);
    out << string;
    file.close();
}