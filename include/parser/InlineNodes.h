#pragma once

#include "parser/AstNode.h"
#include <QString>

struct TextNode : AstNode {
    [[nodiscard]] NodeType type() const override { return NodeType::Text; }
    QString text;
};

struct BoldNode : AstNode {
    [[nodiscard]] NodeType type() const override { return NodeType::Bold; }
};

struct ItalicNode : AstNode {
    [[nodiscard]] NodeType type() const override { return NodeType::Italic; }
};

struct InlineCodeNode : AstNode {
    [[nodiscard]] NodeType type() const override { return NodeType::InlineCode; }
    QString code;
};

struct LinkNode : AstNode {
    [[nodiscard]] NodeType type() const override { return NodeType::Link; }
    QString href;
};

struct ImageNode : AstNode {
    [[nodiscard]] NodeType type() const override { return NodeType::Image; }
    QString src;
    QString alt;
};

struct HardBreakNode : AstNode {
    [[nodiscard]] NodeType type() const override { return NodeType::HardBreak; }
};