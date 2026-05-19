#include "core/Document.h"
#include <QFileInfo>

Document::Document() = default;

QString Document::text() const {
    return text_;
}

QString Document::filePath() const {
    return filePath_;
}

bool Document::isDirty() const {
    return isDirty_;
}

QString Document::title() const {
    if (filePath_.isEmpty()) {
        return "Untitled";
    }
    QFileInfo const fileInfo(filePath_);
    return fileInfo.fileName();
}

void Document::setText(QString const &text) {
    if (text == text_) return;
    text_ = text;
    isDirty_ = true;
}

void Document::markClean(QString const &savedPath) {
    filePath_ = savedPath;
    isDirty_ = false;
}

void Document::reset() {
    *this = Document{};
}
