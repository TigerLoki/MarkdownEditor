#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QIcon>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/app/icons/icon.png"));

    QQmlApplicationEngine engine;
    engine.loadFromModule("MarkdownEditor", "Main");

    return app.exec();
}
