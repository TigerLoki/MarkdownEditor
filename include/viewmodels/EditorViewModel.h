#pragma once

#include "core/Document.h"
#include "core/FileManager.h"

class EditorViewModel : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString text     READ text     WRITE setText  NOTIFY textChanged)
    Q_PROPERTY(QString title    READ title                   NOTIFY titleChanged)
    Q_PROPERTY(bool    isDirty  READ isDirty                 NOTIFY isDirtyChanged)
    Q_PROPERTY(QString filePath READ filePath                NOTIFY titleChanged)

public:
    explicit EditorViewModel(QObject* parent = nullptr);

    [[nodiscard]] QString text()    const;
    [[nodiscard]] QString title()   const;
    [[nodiscard]] bool    isDirty() const;
    [[nodiscard]] QString filePath() const;

public slots:
    void newFile();
    void openFile(const QString& path);
    void saveFile();
    void saveFileAs(const QString& path);
    void exportHtml(const QString& path);
    void setText(const QString& text);

signals:
    void textChanged();
    void titleChanged();
    void isDirtyChanged();
    void errorOccurred(const QString& message);

private:
    Document    document_;
    FileManager fileManager_;
};