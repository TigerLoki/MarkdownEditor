#pragma once

#include <QString>

class Document {
public:
    Document();

    [[nodiscard]] QString     text()     const;
    [[nodiscard]] QString     filePath() const;
    [[nodiscard]] bool        isDirty()  const;
    [[nodiscard]] QString     title()    const;

    void setText(const QString& text);

    void markClean(const QString& savedPath);

    void reset();

private:
    QString text_;
    QString filePath_;
    bool    isDirty_{false};
};
