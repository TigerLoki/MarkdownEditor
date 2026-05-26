#pragma once

#include "parser/renderers/MarkdownTextDocument.h"
#include <QQuickPaintedItem>

#include "ThemeColors.h"

class MarkdownPreviewItem : public QQuickPaintedItem {
    Q_OBJECT
    Q_PROPERTY(QString markdown         READ markdown           WRITE setMarkdown           NOTIFY markdownChanged)
    Q_PROPERTY(qreal contentHeight      READ contentHeight                                  NOTIFY contentHeightChanged)

    Q_PROPERTY(ThemeColors theme        READ theme              WRITE setTheme              NOTIFY themeChanged)
    Q_PROPERTY(QString fontFamily       READ fontFamily         WRITE setFontFamily         NOTIFY fontFamilyChanged)
    Q_PROPERTY(int fontSize             READ fontSize           WRITE setFontSize           NOTIFY fontSizeChanged)

public:
    explicit MarkdownPreviewItem(QQuickItem *parent = nullptr);

    [[nodiscard]] QString markdown() const;
    void setMarkdown(const QString &markdown);

    [[nodiscard]] qreal contentHeight() const;

    [[nodiscard]] ThemeColors theme() const;
    void setTheme(const ThemeColors &theme);

    [[nodiscard]] QString fontFamily() const;
    void setFontFamily(const QString &family);
    [[nodiscard]] int fontSize() const;
    void setFontSize(int size);

signals:
    void markdownChanged();
    void contentHeightChanged();
    void themeChanged();
    void fontFamilyChanged();
    void fontSizeChanged();

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
    QString fontFamily_;
    int fontSize_;
};
