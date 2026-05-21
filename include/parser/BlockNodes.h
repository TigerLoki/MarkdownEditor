#pragma once

#include "parser/AstNode.h"
#include <QString>

struct DocumentNode : AstNode {
    [[nodiscard]] NodeType type() const override { return NodeType::Document; }
};

struct HeadingNode : AstNode {
    [[nodiscard]] NodeType type() const override { return NodeType::Heading; }
    int level{1};
};

struct ParagraphNode : AstNode {
    [[nodiscard]] NodeType type() const override { return NodeType::Paragraph; }
};

struct CodeBlockNode : AstNode {
    [[nodiscard]] NodeType type() const override { return NodeType::CodeBlock; }
    QString lang;
    QString code;
};

struct ThematicBreakNode : AstNode {
    [[nodiscard]] NodeType type() const override { return NodeType::ThematicBreak; }
};

struct BlockquoteNode : AstNode {
    [[nodiscard]] NodeType type() const override { return NodeType::Blockquote; }
};

struct ListNode : AstNode {
    [[nodiscard]] NodeType type() const override { return NodeType::List; }
    bool ordered{false};
    int  start{1};
};

struct ListItemNode : AstNode {
    [[nodiscard]] NodeType type() const override { return NodeType::ListItem; }
};