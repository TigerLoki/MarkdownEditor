#pragma once

#include "parser/renderers/MarkdownTextDocument.h"
#include <QQuickPaintedItem>

#include "ThemeColors.h"

class MarkdownPreviewItem : public QQuickPaintedItem {
    Q_OBJECT
    Q_PROPERTY(QString markdown         READ markdown           WRITE setMarkdown           NOTIFY markdownChanged)
    Q_PROPERTY(qreal contentHeight      READ contentHeight                                  NOTIFY contentHeightChanged)

    Q_PROPERTY(ThemeColors theme        READ theme              WRITE setTheme              NOTIFY themeChanged)

public:
    explicit MarkdownPreviewItem(QQuickItem *parent = nullptr);

    [[nodiscard]] QString markdown() const;
    void setMarkdown(const QString &markdown);

    [[nodiscard]] qreal contentHeight() const;

    [[nodiscard]] ThemeColors theme() const;
    void setTheme(const ThemeColors &theme);

signals:
    void markdownChanged();
    void contentHeightChanged();
    void themeChanged();

protected:
    void paint(QPainter *painter) override;
    void mousePressEvent(QMouseEvent *event) override;

private slots:
    void updateContentSize();
    void performUpdate();

private:
    void scheduleUpdate() const;
    void updateDocument();

    QString markdown_;
    MarkdownTextDocument* document_{nullptr};
    qreal contentHeight_{0};
    QTimer* updateTimer_;
    ThemeColors theme_;
};
