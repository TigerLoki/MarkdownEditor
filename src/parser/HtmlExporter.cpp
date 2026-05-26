#include "parser/HtmlExporter.h"

#include "parser/BlockParser.h"
#include "parser/Lexer.h"

static QString escapeHtml(const QString &str, bool const forAttribute = false) {
    QString out;
    out.reserve(str.size() * 2);
    for (QChar c : str) {
        switch (c.unicode()) {
            case '&':  out += "&amp;";  break;
            case '<':  out += "&lt;";   break;
            case '>':  out += "&gt;";   break;
            case '"':  out += forAttribute ? "&quot;" : "\""; break;
            case '\'': out += forAttribute ? "&#39;"  : "'";  break;
            default:   out += c;        break;
        }
    }
    return out;
}

QString HtmlExporter::toHtml(const QString &markdown) {
    output_.clear();

    Lexer lexer(markdown);
    BlockParser parser(lexer.tokenize());
    auto const ast = parser.parse();

    visit(*ast);

    return output_;
}

void HtmlExporter::visit(const AstNode &node) {
    switch (node.type()) {
        case NodeType::Document:
            visitDocument(dynamic_cast<const DocumentNode&>(node)); break;
        case NodeType::Heading:
            visitHeading(dynamic_cast<const HeadingNode&>(node)); break;
        case NodeType::Paragraph:
            visitParagraph(dynamic_cast<const ParagraphNode&>(node)); break;
        case NodeType::CodeBlock:
            visitCodeBlock(dynamic_cast<const CodeBlockNode&>(node)); break;
        case NodeType::Blockquote:
            visitBlockquote(dynamic_cast<const BlockquoteNode&>(node)); break;
        case NodeType::List:
            visitList(dynamic_cast<const ListNode&>(node)); break;
        case NodeType::ListItem:
            visitListItem(dynamic_cast<const ListItemNode&>(node)); break;
        case NodeType::ThematicBreak:
            visitThematicBreak(); break;
        case NodeType::Text:
            visitText(dynamic_cast<const TextNode&>(node)); break;
        case NodeType::Bold:
            visitBold(dynamic_cast<const BoldNode&>(node)); break;
        case NodeType::Italic:
            visitItalic(dynamic_cast<const ItalicNode&>(node)); break;
        case NodeType::BoldItalic:
            visitBoldItalic(dynamic_cast<const BoldItalicNode&>(node)); break;
        case NodeType::InlineCode:
            visitInlineCode(dynamic_cast<const InlineCodeNode&>(node)); break;
        case NodeType::Link:
            visitLink(dynamic_cast<const LinkNode&>(node)); break;
        case NodeType::Image:
            visitImage(dynamic_cast<const ImageNode&>(node)); break;
        case NodeType::HardBreak:
            visitHardBreak(); break;
        default: break;
    }
}

void HtmlExporter::visitChildren(const AstNode &node) {
    for (const auto &child : node.children) {
        visit(*child);
    }
}

void HtmlExporter::visitDocument(const DocumentNode &node) {
    visitChildren(node);
}

void HtmlExporter::visitHeading(const HeadingNode &node) {
    QString const tag = QString("h%1").arg(node.level);
    output_ += "<" + tag + ">";
    visitChildren(node);
    output_ += "</" + tag + ">\n";
}

void HtmlExporter::visitParagraph(const ParagraphNode &node) {
    output_ += "<p>";
    visitChildren(node);
    output_ += "</p>\n";
}

void HtmlExporter::visitCodeBlock(const CodeBlockNode &node) {
    output_ += "<pre><code";
    if (!node.lang.isEmpty()) {
        output_ += " class=\"language-" + escapeHtml(node.lang, true) + "\"";
    }
    output_ += ">";
    output_ += escapeHtml(node.code, false);
    output_ += "</code></pre>\n";
}

void HtmlExporter::visitBlockquote(const BlockquoteNode &node) {
    output_ += "<blockquote>";
    visitChildren(node);
    output_ += "</blockquote>\n";
}

void HtmlExporter::visitList(const ListNode &node) {
    QString const tag = node.ordered ? "ol" : "ul";
    output_ += "<" + tag;
    if (node.ordered && node.start != 1) {
        output_ += " start=\"" + QString::number(node.start) + "\"";
    }
    output_ += ">\n";
    visitChildren(node);
    output_ += "</" + tag + ">\n";
}

void HtmlExporter::visitListItem(const ListItemNode &node) {
    output_ += "<li>";
    visitChildren(node);
    output_ += "</li>\n";
}

void HtmlExporter::visitThematicBreak() {
    output_ += "<hr />\n";
}

void HtmlExporter::visitText(const TextNode &node) {
    QString escaped = escapeHtml(node.text, false);
    escaped.replace("\n", "<br>");
    output_ += escaped;
}

void HtmlExporter::visitBold(const BoldNode &node) {
    output_ += "<strong>";
    visitChildren(node);
    output_ += "</strong>";
}

void HtmlExporter::visitItalic(const ItalicNode &node) {
    output_ += "<em>";
    visitChildren(node);
    output_ += "</em>";
}

void HtmlExporter::visitBoldItalic(const BoldItalicNode &node) {
    output_ += "<strong><em>";
    visitChildren(node);
    output_ += "</em></strong>";
}

void HtmlExporter::visitInlineCode(const InlineCodeNode &node) {
    output_ += "<code>";
    output_ += escapeHtml(node.code, false);
    output_ += "</code>";
}

void HtmlExporter::visitLink(const LinkNode &node) {
    output_ += "<a href=\"" + escapeHtml(node.href, true) + "\">";
    visitChildren(node);
    output_ += "</a>";
}

void HtmlExporter::visitImage(const ImageNode &node) {
    output_ += "<img src=\"" + escapeHtml(node.src, true) +
               "\" alt=\"" + escapeHtml(node.alt, true) + "\">";
}

void HtmlExporter::visitHardBreak() {
    output_ += "<br>\n";
}