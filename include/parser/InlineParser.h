#pragma once

#include "parser/AstNode.h"
#include <QString>

class InlineParser {
public:
    explicit InlineParser(QString  text);

    [[nodiscard]] AstNode::Children parse();

private:
    AstNode::Children parseInlines(const QString &text);

    AstNode::Ptr tryParseStrong(const QString &text, int &pos);
    AstNode::Ptr tryParseEmphasis(const QString &text, int &pos);
    AstNode::Ptr tryParseStrongEmphasis(const QString &text, int &pos);
    static AstNode::Ptr tryParseInlineCode(const QString &text, int &pos);
    AstNode::Ptr tryParseLink(const QString &text, int &pos);
    static AstNode::Ptr tryParseImage(const QString &text, int &pos);
    static QString collectText(const QString &text, int &pos);

    QString text_;
};