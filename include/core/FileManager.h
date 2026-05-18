#pragma once

#include <QObject>
#include "core/Document.h"

class FileManager : public QObject {
    Q_OBJECT

public:
    explicit FileManager(Document& document, QObject* parent = nullptr);

    void newFile() const;
    void openFile(const QString& path);
    void saveFile();
    void saveFileAs(const QString& path);
    void exportHtml(const QString& path);


signals:
        void errorOccurred(const QString& message);

    private:
        Document& document_;
        void doSaveFile(const QString& path, const QString& string);
        static QString wrapHtml(const QString &body);
};