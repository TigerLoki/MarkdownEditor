#include "parser/BlockParser.h"

#include <QList>
#include "parser/InlineNodes.h"
#include "parser/InlineParser.h"

BlockParser::BlockParser(const std::vector<Token> &tokens) : tokens_(tokens) {
}

AstNode::Ptr BlockParser::parse() {
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

Token const &BlockParser::current() const {
    return tokens_[pos_];
}

Token const& BlockParser::peek(int const offset) const {
    return tokens_[pos_ + offset];
}

Token BlockParser::advance() {
    if (!isAtEnd()) return tokens_[pos_++];
    return tokens_[pos_];
}
bool BlockParser::isAtEnd() const {
    return pos_ == tokens_.size() || current().type == TokenType::EndOfFile;
}

bool BlockParser::check(TokenType const type) const {
    return current().type == type;
}

AstNode::Children BlockParser::parseInlines(const QString &text) {
    return InlineParser(text).parse();
}

AstNode::Ptr BlockParser::parseBlock() {
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

AstNode::Ptr BlockParser::parseHeading() {
    auto const token = advance();

    auto node = std::make_unique<HeadingNode>();
    node->level = std::get<HeadingData>(token.data).level;
    node->children = parseInlines(token.content.value_or(""));
    return node;
}

AstNode::Ptr BlockParser::parseParagraph() {
    auto node = std::make_unique<ParagraphNode>();

    while (!isAtEnd() && check(TokenType::Paragraph)) {
        auto const token = advance();
        QString const line = token.content.value_or("");

        bool const hardBreak = line.endsWith("  ");
        QString const content = hardBreak ? line.chopped(2) : line;
        for (auto inlines = parseInlines(content); auto &child : inlines) {
            node->children.push_back(std::move(child));
        }

        if (!isAtEnd() && check(TokenType::Paragraph)) {
            if (hardBreak) {
                node->children.push_back(std::make_unique<HardBreakNode>());
            } else {
                auto space = std::make_unique<TextNode>();
                space->text = " ";
                node->children.push_back(std::move(space));
            }
        }
    }

    return node;
}

AstNode::Ptr BlockParser::parseCodeBlock() {
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

AstNode::Ptr BlockParser::parseBlockquote() {
    auto const token = advance();

    auto node = std::make_unique<BlockquoteNode>();
    node->children = parseInlines(token.content.value_or(""));
    return node;
}

AstNode::Ptr BlockParser::parseList() {
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

AstNode::Ptr BlockParser::parseThematicBreak() {
    advance();
    return std::make_unique<ThematicBreakNode>();
}