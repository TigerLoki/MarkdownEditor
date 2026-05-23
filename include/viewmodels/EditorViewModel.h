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
    explicit EditorViewModel(QObject *parent = nullptr);

    [[nodiscard]] QString text()    const;
    [[nodiscard]] QString title()   const;
    [[nodiscard]] bool isDirty() const;
    [[nodiscard]] QString filePath() const;

public slots:
    void newFile();
    void openFile(QString const &path);
    void saveFile();
    void saveFileAs(QString const &path);
    void exportHtml(QString const &path);
    void setText(QString const &text);

signals:
    void textChanged();
    void titleChanged();
    void isDirtyChanged();
    void errorOccurred(QString const &message);

private:
    Document    document_;
    FileManager fileManager_;
};