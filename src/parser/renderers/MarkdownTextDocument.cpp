#include "parser/renderers/MarkdownTextDocument.h"

#include <QNetworkRequest>
#include <QNetworkReply>

MarkdownTextDocument::MarkdownTextDocument(QObject *parent)
    : QTextDocument(parent)
{
}

QVariant MarkdownTextDocument::loadResource(int const type, const QUrl &url) {
    if (type == QTextDocument::ImageResource) {
        if (cache_.contains(url))
            return cache_[url];

        if (pending_.contains(url))
            return QImage();

        pending_[url] = true;
        QNetworkReply *reply = network_.get(QNetworkRequest(url));
        reply->setParent(this);
        reply->setProperty("url", url);

        connect(reply, &QNetworkReply::finished,
                this, &MarkdownTextDocument::onReplyFinished);
        return QImage();
    }
    return QTextDocument::loadResource(type, url);
}

void MarkdownTextDocument::onReplyFinished() {
    auto *reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;

    reply->deleteLater();
    QUrl const url = reply->property("url").toUrl();
    pending_.remove(url);

    if (reply->error() == QNetworkReply::NoError) {
        if (QImage image; image.loadFromData(reply->readAll())) {
            cache_[url] = image;
            addResource(QTextDocument::ImageResource, url, image);
            emit resourceLoaded();
        }
    }
}