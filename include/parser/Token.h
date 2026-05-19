#pragma once

#include <QString>

enum class TokenType {
    Heading,
    Paragraph,
    BlankLine,
    FencedCodeBlockStart,
    FencedCodeBlockContent,
    FencedCodeBlockEnd,
    ThematicBreak,
    Blockquote,
    UnorderedListItem,
    OrderedListItem,

    EndOfFile,
};

struct HeadingData { int level; }; // heading level
struct CodeBlockData { QString lang; }; // fenced code type
struct OrderedData { int number; }; // ordered list

using TokenData = std::variant<std::monostate, HeadingData, CodeBlockData, OrderedData>;

struct Token {
    TokenType type;
    int line{0};
    std::optional<QString> content = std::nullopt;
    TokenData data{std::monostate{}};
};