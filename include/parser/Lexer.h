#pragma once

#include "parser/Token.h"
#include <QStringList>
#include <vector>

class Lexer {
public:
    explicit Lexer(QString const &source);

    [[nodiscard]] std::vector<Token> tokenize();

private:
    [[nodiscard]] Token lexLine(QString const &line, int lineNumber);

    using TokenBuilder = std::function<Token(QRegularExpressionMatch const &)>;
    static std::optional<Token> tryLex(QString const &line, QRegularExpression const &re, TokenBuilder const &builder);

    static bool isBlankLine(QString const &line);
    static std::optional<Token> tryLexHeading(QString const &line, int lineNumber) ;
    std::optional<Token> tryLexFencedCodeBlockStart(QString const &line, int lineNumber);
    std::optional<Token> tryLexFencedCodeBlockEnd(QString const &line, int lineNumber);
    static std::optional<Token> tryLexBlockquote(QString const &line, int lineNumber);
    static std::optional<Token> tryLexUnorderedListItem(QString const &line, int lineNumber);
    static std::optional<Token> tryLexOrderedListItem(QString const &line, int lineNumber);
    static std::optional<Token> tryLexThematicBreak(QString const &line, int lineNumber);

    QStringList  lines_;
    bool inCodeBlock_{false};
};