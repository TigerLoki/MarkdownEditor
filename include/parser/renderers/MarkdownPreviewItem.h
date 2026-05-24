#pragma once

#include "parser/renderers/MarkdownTextDocument.h"
#include <QQuickPaintedItem>

class MarkdownPreviewItem : public QQuickPaintedItem {
    Q_OBJECT
    Q_PROPERTY(QString markdown READ markdown WRITE setMarkdown NOTIFY markdownChanged)
    Q_PROPERTY(qreal contentHeight READ contentHeight NOTIFY contentHeightChanged)

public:
    explicit MarkdownPreviewItem(QQuickItem *parent = nullptr);

    [[nodiscard]] QString markdown() const;
    void setMarkdown(const QString &markdown);

    [[nodiscard]] qreal contentHeight() const;

    signals:
        void markdownChanged();
    void contentHeightChanged();

protected:
    void paint(QPainter *painter) override;
    void mousePressEvent(QMouseEvent *event) override;

private slots:
    void updateContentSize();

private:
    void updateDocument();

    QString markdown_;
    MarkdownTextDocument* document_{nullptr};
    qreal contentHeight_{0};
};