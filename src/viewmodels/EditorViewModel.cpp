#include "viewmodels/EditorViewModel.h"

#include "parser/HtmlExporter.h"

EditorViewModel::EditorViewModel(QObject *parent)
    : QObject(parent)
    , document_()
    , fileManager_(document_)
{
    connect(&fileManager_, &FileManager::errorOccurred, this, &EditorViewModel::errorOccurred);
}

QString EditorViewModel::text() const {
    return document_.text();
}

QString EditorViewModel::title() const {
    return document_.title();
}

bool EditorViewModel::isDirty() const {
    return document_.isDirty();
}

QString EditorViewModel::filePath() const {
    return document_.filePath();
}

void EditorViewModel::setText(QString const &text) {
    if (text == document_.text()) return;
    document_.setText(text);
    emit textChanged();
    emit isDirtyChanged();
}

void EditorViewModel::newFile() {
    fileManager_.newFile();
    emit textChanged();
    emit titleChanged();
    emit isDirtyChanged();
}

void EditorViewModel::openFile(QString const &path) {
    fileManager_.openFile(path);
    emit textChanged();
    emit titleChanged();
    emit isDirtyChanged();
}

void EditorViewModel::saveFile() {
    fileManager_.saveFile();
    emit titleChanged();
    emit isDirtyChanged();
}

void EditorViewModel::saveFileAs(QString const &path) {
    fileManager_.saveFileAs(path);
    emit titleChanged();
    emit isDirtyChanged();
}

void EditorViewModel::exportHtml(QString const &path) {
    HtmlExporter exporter;
    fileManager_.exportHtml(path);
}

void EditorViewModel::applyFormatting(int const selectionStart, int const selectionEnd, QString const &prefix, QString const &suffix)
{
    QString const currentText = document_.text();
    int const start = qBound(0, selectionStart, currentText.size());
    int const end = qBound(start, selectionEnd, currentText.size());
    bool const hasSelection = (start != end);

    QString newText;
    int newCursorPos = -1;

    if (hasSelection) {
        QString const selected = currentText.mid(start, end - start);
        newText = currentText.left(start) + prefix + selected + suffix + currentText.mid(end);
        newCursorPos = start + prefix.length() + selected.length() + suffix.length();
    } else {
        newText = currentText.left(start) + prefix + suffix + currentText.mid(end);
        newCursorPos = start + prefix.length();
    }

    if (newText != currentText) {
        document_.setText(newText);
        emit textChanged();
        emit isDirtyChanged();
        emit cursorPositionChanged(newCursorPos);
    }
}

void EditorViewModel::applyLinePrefix(int const cursorPosition, QString const &prefix)
{
    QString const text = document_.text();

    int lineStart = cursorPosition;
    while (lineStart > 0 && text[lineStart - 1] != '\n')
        --lineStart;

    QString const newText = text.left(lineStart) + prefix + text.mid(lineStart);
    document_.setText(newText);
    emit textChanged();
    emit isDirtyChanged();

    emit cursorPositionChanged(cursorPosition + prefix.length());
}