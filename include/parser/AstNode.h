#pragma once

#include <vector>
#include <memory>

enum class NodeType {
    // Block
    Document,
    Heading,
    Paragraph,
    CodeBlock,
    ThematicBreak,
    Blockquote,
    List,
    ListItem,

    // Inline
    Text,
    Bold,
    Italic,
    BoldItalic,
    InlineCode,
    Link,
    Image,
    HardBreak,
    SoftBreak,
};

struct AstNode {
    using Ptr      = std::unique_ptr<AstNode>;
    using Children = std::vector<Ptr>;

    virtual ~AstNode() = default;
    [[nodiscard]] virtual NodeType type() const = 0;

    Children children;
};