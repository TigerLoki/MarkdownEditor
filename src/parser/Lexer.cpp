#include "parser/Lexer.h"
#include <QRegularExpression>

Lexer::Lexer(const QString &source) : lines_(source.split('\n')) {}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    for (int i = 0; i < lines_.size(); ++i) {
        tokens.push_back(lexLine(lines_[i], i + 1));
    }
    tokens.push_back({TokenType::EndOfFile});
    return tokens;
}

Token Lexer::lexLine(QString const &line, int const lineNumber) {
    if (inCodeBlock_) {
        if (auto const token = tryLexFencedCodeBlockEnd(line, lineNumber)) {
            return *token;
        }

        return Token(TokenType::FencedCodeBlockContent, lineNumber, line);
    }
    if (isBlankLine(line)) {
        return Token(TokenType::BlankLine, lineNumber);
    }
    if (auto const token = tryLexHeading(line, lineNumber)) {
        return *token;
    }
    if (auto const token = tryLexFencedCodeBlockStart(line, lineNumber)) {
        return *token;
    }
    if (auto const token = tryLexBlockquote(line, lineNumber)) {
        return *token;
    }
    if (auto const token = tryLexUnorderedListItem(line, lineNumber)) {
        return *token;
    }
    if (auto const token = tryLexOrderedListItem(line, lineNumber)) {
        return *token;
    }
    if (auto const token = tryLexThematicBreak(line, lineNumber)) {
        return *token;
    }
    return Token(TokenType::Paragraph, lineNumber, line);
}

std::optional<Token> Lexer::tryLex(QString const &line,
                                    QRegularExpression const &re,
                                    TokenBuilder const &builder) {
    QRegularExpressionMatch const match = re.match(line.trimmed());
    if (!match.hasMatch()) return std::nullopt;
    return builder(match);
}

bool Lexer::isBlankLine(QString const &line) {
    return line.trimmed().isEmpty();
}

std::optional<Token> Lexer::tryLexHeading(QString const &line, int const lineNumber) {
    static QRegularExpression const re(R"(^(#{1,6})\s(.*)$)");
    return tryLex(line, re, [lineNumber](auto const &match) {
        return Token(
            TokenType::Heading,
            lineNumber,
            match.captured(2),
            HeadingData(static_cast<int>(match.captured(1).length()))
            );
    });
}

std::optional<Token> Lexer::tryLexFencedCodeBlockStart(QString const &line, int const lineNumber) {
    static QRegularExpression const re(R"(^`{3}(.*))");
    return tryLex(line, re, [this, lineNumber](auto const &match) {
        inCodeBlock_ = true;
        return Token(
            TokenType::FencedCodeBlockStart,
            lineNumber,
            std::nullopt,
            CodeBlockData(match.captured(1))
            );
    });
}

std::optional<Token> Lexer::tryLexFencedCodeBlockEnd(QString const &line, int const lineNumber) {
    static QRegularExpression const re(R"(^`{3}\s*$)");
    return tryLex(line, re, [this, lineNumber](auto const &) {
        inCodeBlock_ = false;
        return Token(
            TokenType::FencedCodeBlockEnd,
            lineNumber
            );
    });
}

std::optional<Token> Lexer::tryLexBlockquote(QString const &line, int const lineNumber) {
    static QRegularExpression const re(R"(^>\s*(.*)$)");
    return tryLex(line, re, [lineNumber](auto const &match) {
        return Token(
            TokenType::Blockquote,
            lineNumber,
            match.captured(1)
            );
    });
}

std::optional<Token> Lexer::tryLexUnorderedListItem(QString const &line, int const lineNumber) {
    static QRegularExpression const re(R"(^(-|\*)\h(.+)$)");
    return tryLex(line, re, [lineNumber](auto const &match) {
        return Token(
            TokenType::UnorderedListItem,
            lineNumber,
            match.captured(2)
            );
    });
}

std::optional<Token> Lexer::tryLexOrderedListItem(QString const &line, int const lineNumber) {
    static QRegularExpression const re(R"((\d+)\.\h(.+)$)");
    return tryLex(line, re, [lineNumber](auto const &match) {
        return Token(
            TokenType::OrderedListItem,
            lineNumber,
            match.captured(2),
            OrderedData(match.captured(1).toInt())
            );
    });
}

std::optional<Token> Lexer::tryLexThematicBreak(QString const &line, int const lineNumber) {
    static QRegularExpression const re(R"(^(\-\s*){3,}$|^(\*\s*){3,}$|^(_\s*){3,}$)");
    return tryLex(line, re, [lineNumber](auto const &) {
        return Token(TokenType::ThematicBreak, lineNumber);
    });
}