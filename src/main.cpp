#include <QQmlApplicationEngine>
#include <QIcon>
#include <QQmlContext>
#include <QQuickStyle>

#include "core/AppSettings.h"
#include "parser/renderers/MarkdownPreviewItem.h"
#include "viewmodels/EditorViewModel.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/app/icons/icon.png"));

    QQuickStyle::setStyle("Fusion");
    qRegisterMetaType<ThemeColors>("ThemeColors");
    qmlRegisterUncreatableType<ThemeColors>("MarkdownEditor", 1, 0, "themeColors", "...");

    qmlRegisterType<MarkdownPreviewItem>("MarkdownEditor", 1, 0, "MarkdownPreview");

    AppSettings settings;
    EditorViewModel editorVM;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("appSettings", &settings);
    engine.rootContext()->setContextProperty("editorVM", &editorVM);
    engine.loadFromModule("MarkdownEditor", "Main");

    return app.exec();
}
