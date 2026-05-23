#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QIcon>
#include <QQmlContext>

#include "core/Document.h"
#include "viewmodels/EditorViewModel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/app/icons/icon.png"));

    EditorViewModel editorVM;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("editorVM", &editorVM);
    engine.loadFromModule("MarkdownEditor", "Main");

    return app.exec();
}

