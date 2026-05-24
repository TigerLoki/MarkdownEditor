#include "parser/InlineParser.h"
#include "parser/InlineNodes.h"

InlineParser::InlineParser(QString text) : text_(std::move(text)) {}

AstNode::Children InlineParser::parse() {
    return parseInlines(text_);
}

AstNode::Children InlineParser::parseInlines(const QString &text) {
    AstNode::Children result;
    int pos = 0;

    while (pos < text.size()) {
        QChar c = text[pos];

        if (c == '`') {
            if (auto node = tryParseInlineCode(text, pos)) {
                result.push_back(std::move(node));
                continue;
            }
        }
        if (c == '*' || c == '_') {
            if (auto node = tryParseStrongEmphasis(text, pos)) {
                result.push_back(std::move(node));
                continue;
            }
            if (auto node = tryParseStrong(text, pos)) {
                result.push_back(std::move(node));
                continue;
            }
            if (auto node = tryParseEmphasis(text, pos)) {
                result.push_back(std::move(node));
                continue;
            }
        }
        if (c == '[') {
            if (auto node = tryParseLink(text, pos)) {
                result.push_back(std::move(node));
                continue;
            }
        }
        if (c == '!' && pos + 1 < text.size() && text[pos+1] == '[') {
            if (auto node = tryParseImage(text, pos)) {
                result.push_back(std::move(node));
                continue;
            }
        }

        auto textNode = std::make_unique<TextNode>();

        int const oldPos = pos;
        textNode->text = collectText(text, pos);
        if (pos == oldPos) {
            textNode->text = text[pos];
            ++pos;
        }
        result.push_back(std::move(textNode));
    }

    return result;
}

QString InlineParser::collectText(const QString &text, int &pos) {
    QString result;
    while (pos < text.size()) {
        QChar c = text[pos];

        if (c == '\\' && pos + 1 < text.size()) {
            QChar const next = text[pos + 1];
            if (next == '*' || next == '_' || next == '`' ||
                next == '[' || next == '!' || next == '\\') {
                result += next;
                pos += 2;
                continue;
            }
        }

        if (c == '*' || c == '_' || c == '`' || c == '[' || c == '!') break;
        result += c;
        ++pos;
    }
    return result;
}

AstNode::Ptr InlineParser::tryParseInlineCode(const QString &text, int &pos) {
    int const start = pos + 1;
    int const end = static_cast<int>(text.indexOf('`', start));
    if (end == -1) return nullptr;

    auto node = std::make_unique<InlineCodeNode>();
    node->code = text.mid(start, end - start);
    pos = end + 1;
    return node;
}

AstNode::Ptr InlineParser::tryParseStrongEmphasis(const QString &text, int &pos) {
    QChar const marker = text[pos];
    if (pos + 2 >= text.size() || text[pos+1] != marker || text[pos+2] != marker) return nullptr;

    int const start = pos + 3;
    auto const closing = QString(3, marker);
    int const end = static_cast<int>(text.indexOf(closing, start));
    if (end == -1) return nullptr;

    auto node = std::make_unique<BoldItalicNode>();
    node->children = parseInlines(text.mid(start, end - start));
    pos = end + 3;
    return node;
}

AstNode::Ptr InlineParser::tryParseStrong(const QString &text, int &pos) {
    QChar const marker = text[pos];
    if (pos + 1 >= text.size() || text[pos+1] != marker) return nullptr;

    int const start = pos + 2;
    auto const closing = QString(2, marker);
    int const end = static_cast<int>(text.indexOf(closing, start));
    if (end == -1) return nullptr;

    auto node = std::make_unique<BoldNode>();
    node->children = parseInlines(text.mid(start, end - start));
    pos = end + 2;
    return node;
}

AstNode::Ptr InlineParser::tryParseEmphasis(const QString &text, int &pos) {
    QChar const marker = text[pos];
    if (pos + 1 >= text.size() || text[pos+1] == marker) return nullptr;

    int const start = pos + 1;
    int const end = static_cast<int>(text.indexOf(marker, start));
    if (end == -1) return nullptr;

    auto node = std::make_unique<ItalicNode>();
    node->children = parseInlines(text.mid(start, end - start));
    pos = end + 1;
    return node;
}

AstNode::Ptr InlineParser::tryParseLink(const QString &text, int &pos) {
    int const start = pos + 1;
    int const end = static_cast<int>(text.indexOf(']', start));
    if (end == -1) return nullptr;

    int const urlStart = end + 1;
    if (urlStart >= text.size() || text[urlStart] != '(') return nullptr;
    int const urlEnd = static_cast<int>(text.indexOf(')', urlStart));
    if (urlEnd == -1) return nullptr;

    auto node = std::make_unique<LinkNode>();
    node->href = text.mid(urlStart + 1, urlEnd - urlStart - 1);
    node->children = parseInlines(text.mid(start, end - start));
    pos = urlEnd + 1;
    return node;
}

AstNode::Ptr InlineParser::tryParseImage(const QString &text, int &pos) {
    int const start = pos + 2;
    int const end = static_cast<int>(text.indexOf(']', start));
    if (end == -1) return nullptr;

    int const urlStart = end + 1;
    if (urlStart >= text.size() || text[urlStart] != '(') return nullptr;
    int const urlEnd = static_cast<int>(text.indexOf(')', urlStart));
    if (urlEnd == -1) return nullptr;

    auto node = std::make_unique<ImageNode>();
    node->src = text.mid(urlStart + 1, urlEnd - urlStart - 1);
    node->alt = text.mid(start, end - start);
    pos = urlEnd + 1;
    return node;
}