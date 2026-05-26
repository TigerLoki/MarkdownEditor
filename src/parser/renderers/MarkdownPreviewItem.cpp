#include "parser/renderers/MarkdownPreviewItem.h"

#include "parser/HtmlExporter.h"
#include <QDesktopServices>
#include <QPainter>
#include <QAbstractTextDocumentLayout>
#include <QPalette>
#include <limits>
#include <QTimer>

MarkdownPreviewItem::MarkdownPreviewItem(QQuickItem *parent)
    : QQuickPaintedItem(parent)
    , fontFamily_("Courier New")
    , fontSize_(16)
{
    setRenderTarget(QQuickPaintedItem::FramebufferObject);

    document_ = new MarkdownTextDocument(this);

    updateTimer_ = new QTimer(this);
    updateTimer_->setSingleShot(true);
    updateTimer_->setInterval(50);
    connect(updateTimer_, &QTimer::timeout, this, &MarkdownPreviewItem::performUpdate);

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
    emit markdownChanged();
    scheduleUpdate();
}

qreal MarkdownPreviewItem::contentHeight() const {
    return contentHeight_;
}

ThemeColors MarkdownPreviewItem::theme() const {
    return theme_;
}

void MarkdownPreviewItem::scheduleUpdate() const {
    updateTimer_->start();
}

void MarkdownPreviewItem::performUpdate() {
    updateDocument();
}

void MarkdownPreviewItem::setTheme(const ThemeColors &theme) {
    if (theme_ == theme) return;
    theme_ = theme;
    emit themeChanged();
    update();
}
QString MarkdownPreviewItem::fontFamily() const {
    return fontFamily_;
}

void MarkdownPreviewItem::setFontFamily(const QString &family) {
    if (fontFamily_ == family) return;
    fontFamily_ = family;
    emit fontFamilyChanged();
    updateDocument();
}

int MarkdownPreviewItem::fontSize() const {
    return fontSize_;
}

void MarkdownPreviewItem::setFontSize(const int size) {
    if (fontSize_ == size) return;
    fontSize_ = size;
    emit fontSizeChanged();
    updateDocument();
}

void MarkdownPreviewItem::updateDocument() {
    HtmlExporter exporter;
    QString html = exporter.toHtml(markdown_);
    html = QString("<html><body style=\"font-family:'%1',monospace;font-size:%2px;\">"
           "<style>img { max-width: 100%%; height: auto; }</style>"
           "%3</body></html>")
           .arg(fontFamily_)
           .arg(fontSize_)
           .arg(html);
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
    painter->setRenderHint(QPainter::Antialiasing);

    QRectF const fullRect(0, 0, width(), height());
    painter->fillRect(fullRect, theme_.previewBg);

    QRectF const docRect(0, 0, width(), qMax(static_cast<qreal>(contentHeight_), height()));
    painter->setBrush(theme_.previewBg);
    painter->setPen(theme_.previewBorder);
    painter->drawRoundedRect(docRect, 4, 4);

    if (contentHeight_ > 0) {
        QAbstractTextDocumentLayout::PaintContext ctx;
        ctx.palette.setColor(QPalette::Text, theme_.previewText);
        ctx.palette.setColor(QPalette::Base, theme_.previewBg);
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