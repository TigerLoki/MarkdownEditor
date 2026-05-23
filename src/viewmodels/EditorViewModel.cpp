#include "viewmodels/EditorViewModel.h"

#include "parser/HtmlExporter.h"

EditorViewModel::EditorViewModel(QObject *parent)
    : QObject(parent)
    , document_()
    , fileManager_(document_)
{
    connect(&fileManager_, &FileManager::errorOccurred,
        this,          &EditorViewModel::errorOccurred);
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