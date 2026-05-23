#pragma once

#include "parser/AstNode.h"
#include "parser/BlockNodes.h"
#include "parser/InlineNodes.h"
#include <QString>

class HtmlExporter {
public:
    [[nodiscard]] QString toHtml(const QString &markdown);

private:
    void visit(const AstNode &node);

    void visitDocument(const DocumentNode &node);
    void visitHeading(const HeadingNode &node);
    void visitParagraph(const ParagraphNode &node);
    void visitCodeBlock(const CodeBlockNode &node);
    void visitBlockquote(const BlockquoteNode &node);
    void visitList(const ListNode &node);
    void visitListItem(const ListItemNode &node);
    void visitThematicBreak();

    void visitText(const TextNode &node);
    void visitBold(const BoldNode &node);
    void visitItalic(const ItalicNode &node);
    void visitInlineCode(const InlineCodeNode &node);
    void visitLink(const LinkNode &node);
    void visitImage(const ImageNode &node);
    void visitHardBreak();
    void visitChildren(const AstNode &node);
    void escape(const QString &text);

    QString output_;
};