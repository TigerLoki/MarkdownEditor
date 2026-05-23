#pragma once

#include "parser/Token.h"
#include "parser/AstNode.h"
#include "parser/BlockNodes.h"
#include <vector>

class BlockParser {
public:
    explicit BlockParser(const std::vector<Token> &tokens);

    [[nodiscard]] AstNode::Ptr parse();

private:
    [[nodiscard]] Token const &current() const;
    [[nodiscard]] Token const &peek(int offset = 1) const;
    Token advance();
    [[nodiscard]] bool isAtEnd() const;
    [[nodiscard]] bool check(TokenType type) const;

    AstNode::Ptr parseBlock();
    AstNode::Ptr parseHeading();
    AstNode::Ptr parseParagraph();
    AstNode::Ptr parseCodeBlock();
    AstNode::Ptr parseBlockquote();
    AstNode::Ptr parseList();
    AstNode::Ptr parseThematicBreak();

    static AstNode::Children parseInlines(const QString &text);

    std::vector<Token> tokens_;
    size_t pos_{0};
};