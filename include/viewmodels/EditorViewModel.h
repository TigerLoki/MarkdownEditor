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

    Q_INVOKABLE void applyFormatting(int selectionStart, int selectionEnd, const QString &prefix, const QString &suffix);
    Q_INVOKABLE void applyLinePrefix(int cursorPosition, const QString &prefix);

signals:
    void textChanged();
    void titleChanged();
    void isDirtyChanged();
    void darkThemeChanged();
    void cursorPositionChanged(int newPos);
    void errorOccurred(QString const &message);

private:
    Document document_;
    FileManager fileManager_;
    bool darkTheme_ = false;
};