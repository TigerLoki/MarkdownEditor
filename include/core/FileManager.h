#pragma once

#include <QObject>
#include "core/Document.h"

class FileManager : public QObject {
    Q_OBJECT

public:
    explicit FileManager(Document &document, QObject *parent = nullptr);

    void newFile() const;
    void openFile(QString const &path);
    void saveFile();
    void saveFileAs(QString const &path);
    void exportHtml(QString const &path);

signals:
    void errorOccurred(QString const &message);

    private:
        Document &document_;
        void doSaveFile(QString const &path, QString const &string);
};