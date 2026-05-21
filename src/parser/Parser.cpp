#include "parser/Parser.h"

#include <QList>

Parser::Parser(const std::vector<Token> &tokens) : tokens_(tokens) {
}

AstNode::Ptr Parser::parse() {
    auto document = std::make_unique<DocumentNode>();

    while (!isAtEnd()) {
        if (check(TokenType::BlankLine)) {
            advance();
            continue;
        }

        if (auto block = parseBlock()) {
            document->children.push_back(std::move(block));
        }
    }

    return document;
}

Token const& Parser::current() const {
    return tokens_[pos_];
}

Token const& Parser::peek(int const offset) const {
    return tokens_[pos_ + offset];
}

Token Parser::advance() {
    if (!isAtEnd()) return tokens_[pos_++];
    return tokens_[pos_];
}
bool Parser::isAtEnd() const {
    return pos_ == tokens_.size() || current().type == TokenType::EndOfFile;
}

bool Parser::check(TokenType const type) const {
    return current().type == type;
}

AstNode::Children Parser::parseInlines(const QString &text) {
    AstNode::Children result;
    // auto textNode = std::make_unique<TextNode>();
    // textNode->text = text;
    // result.push_back(std::move(textNode));
    return result;
}

AstNode::Ptr Parser::parseBlock() {
    switch (current().type) {
        case TokenType::Heading:          return parseHeading();
        case TokenType::Paragraph:        return parseParagraph();
        case TokenType::FencedCodeBlockStart: return parseCodeBlock();
        case TokenType::Blockquote:       return parseBlockquote();
        case TokenType::UnorderedListItem:
        case TokenType::OrderedListItem:  return parseList();
        case TokenType::ThematicBreak:    return parseThematicBreak();
        default:
            advance();
            return nullptr;
    }
}

AstNode::Ptr Parser::parseHeading() {
    auto const token = advance();

    auto node = std::make_unique<HeadingNode>();
    node->level = std::get<HeadingData>(token.data).level;
    node->children = parseInlines(token.content.value_or(""));
    return node;
}

AstNode::Ptr Parser::parseParagraph() {
    auto const token = advance();

    auto node = std::make_unique<ParagraphNode>();
    node->children = parseInlines(token.content.value_or(""));
    return node;
}

AstNode::Ptr Parser::parseCodeBlock() {
    auto const token = advance();

    auto node = std::make_unique<CodeBlockNode>();
    node->lang = std::get<CodeBlockData>(token.data).lang;

    QStringList lines;
    while (!isAtEnd() && !check(TokenType::FencedCodeBlockEnd)) {
        auto const contentToken = advance();
        lines.push_back(contentToken.content.value_or(""));
    }

    if (check(TokenType::FencedCodeBlockEnd)) {
        advance();
    }

    node->code = lines.join('\n');
    return node;
}

AstNode::Ptr Parser::parseBlockquote() {
    auto const token = advance();

    auto node = std::make_unique<BlockquoteNode>();
    node->children = parseInlines(token.content.value_or(""));
    return node;
}

AstNode::Ptr Parser::parseList() {
    auto node = std::make_unique<ListNode>();
    node->ordered = check(TokenType::OrderedListItem);

    TokenType const itemType = node->ordered
                         ? TokenType::OrderedListItem
                         : TokenType::UnorderedListItem;

    while (!isAtEnd() && check(itemType)) {
        auto token = advance();
        auto item  = std::make_unique<ListItemNode>();
        item->children = parseInlines(token.content.value_or(""));
        node->children.push_back(std::move(item));
    }

    return node;
}

AstNode::Ptr Parser::parseThematicBreak() {
    advance();
    return std::make_unique<ThematicBreakNode>();
}