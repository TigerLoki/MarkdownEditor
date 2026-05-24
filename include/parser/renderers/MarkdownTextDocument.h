#pragma once

#include <QTextDocument>
#include <QNetworkAccessManager>
#include <QHash>
#include <QImage>
#include <QUrl>

class MarkdownTextDocument : public QTextDocument {
    Q_OBJECT

public:
    explicit MarkdownTextDocument(QObject *parent = nullptr);

protected:
    QVariant loadResource(int type, const QUrl &url) override;

    signals:
        void resourceLoaded();

private slots:
    void onReplyFinished();

private:
    QNetworkAccessManager network_;
    QHash<QUrl, QImage>   cache_;
    QHash<QUrl, bool>     pending_;
};