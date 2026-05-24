#include "parser/renderers/MarkdownPreviewItem.h"

#include "parser/HtmlExporter.h"
#include <QDesktopServices>
#include <QPainter>
#include <QAbstractTextDocumentLayout>
#include <QPalette>
#include <limits>

MarkdownPreviewItem::MarkdownPreviewItem(QQuickItem *parent)
    : QQuickPaintedItem(parent)
{
    setRenderTarget(QQuickPaintedItem::FramebufferObject);
    setOpaquePainting(true);

    document_ = new MarkdownTextDocument(this);

    connect(document_, &MarkdownTextDocument::resourceLoaded,
            this, [this]() {
                document_->setPageSize(QSizeF());
                updateContentSize();
                update();
            });

    connect(this, &QQuickItem::widthChanged, this, [this]() {
        updateContentSize();
        update();
    });

    connect(this, &QQuickItem::heightChanged, this, [this]() {
        update();
    });

    setAcceptedMouseButtons(Qt::AllButtons);
    setFlag(QQuickItem::ItemHasContents, true);
}

QString MarkdownPreviewItem::markdown() const {
    return markdown_;
}

void MarkdownPreviewItem::setMarkdown(const QString &markdown) {
    if (markdown_ == markdown) return;
    markdown_ = markdown;
    updateDocument();
    emit markdownChanged();
}

qreal MarkdownPreviewItem::contentHeight() const {
    return contentHeight_;
}

void MarkdownPreviewItem::updateDocument() {
    HtmlExporter exporter;
    QString html = exporter.toHtml(markdown_);
    html = "<html><body style=\"color: black; background: white;\">"
           "<style>img { max-width: 100%; height: auto; }</style>"
           + html + "</body></html>";
    document_->setHtml(html);
    updateContentSize();
    update();
}

void MarkdownPreviewItem::updateContentSize() {
    if (width() <= 0) {
        if (!qFuzzyIsNull(contentHeight_)) {
            contentHeight_ = 0;
            emit contentHeightChanged();
        }
        return;
    }

    document_->setPageSize(QSizeF(width(), std::numeric_limits<qreal>::max()));

    if (qreal const newHeight = document_->size().height(); !qFuzzyCompare(newHeight, contentHeight_)) {
        contentHeight_ = newHeight;
        emit contentHeightChanged();
    }
}

void MarkdownPreviewItem::paint(QPainter *painter) {
    if (width() <= 0) return;

    QRectF const fullRect(0, 0, width(), height());
    painter->fillRect(fullRect, Qt::white);

    if (contentHeight_ > 0) {
        QAbstractTextDocumentLayout::PaintContext ctx;
        ctx.palette.setColor(QPalette::Text, Qt::black);
        ctx.clip = painter->clipBoundingRect();
        document_->documentLayout()->draw(painter, ctx);
    }
}

void MarkdownPreviewItem::mousePressEvent(QMouseEvent *event) {
    if (!document_) {
        QQuickPaintedItem::mousePressEvent(event);
        return;
    }

    if (QString const anchor = document_->documentLayout()->anchorAt(event->position()); !anchor.isEmpty()) {
        QDesktopServices::openUrl(QUrl(anchor));
        return;
    }
    QQuickPaintedItem::mousePressEvent(event);
}