#include "core/AppSettings.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QApplication>
#include <QJsonObject>
#include <QPalette>
#include <QStandardPaths>

static ThemeColors loadThemeFromJson(const QJsonObject &obj) {
    ThemeColors theme;
    theme.editorBg          = QColor(obj.value("editorBg").toString("#ffffff"));
    theme.editorText        = QColor(obj.value("editorText").toString("#111111"));
    theme.editorSelection   = QColor(obj.value("editorSelection").toString("#cce5ff"));
    theme.editorSelectionText = QColor(obj.value("editorSelectionText").toString("#000000"));
    theme.editorBorder      = QColor(obj.value("editorBorder").toString("#dddddd"));
    theme.lineNumbersBg     = QColor(obj.value("lineNumbersBg").toString("#f5f5f5"));
    theme.lineNumbersText   = QColor(obj.value("lineNumbersText").toString("#666666"));
    theme.previewBg         = QColor(obj.value("previewBg").toString("#ffffff"));
    theme.previewText       = QColor(obj.value("previewText").toString("#111111"));
    theme.previewBorder     = QColor(obj.value("previewBorder").toString("#dddddd"));
    theme.previewFill       = QColor(obj.value("previewFill").toString("#f0f0f0"));
    return theme;
}

static void mergeJsonObjects(QJsonObject &target, const QJsonObject &source) {
    for (auto it = source.begin(); it != source.end(); ++it) {
        if (const QString key = it.key(); !target.contains(key)) {
            target[key] = it.value();
        } else if (it.value().isObject() && target[key].isObject()) {
            QJsonObject targetObj = target[key].toObject();
            QJsonObject sourceObj = it.value().toObject();
            mergeJsonObjects(targetObj, sourceObj);
            target[key] = targetObj;
        }
    }
}

AppSettings::AppSettings(QObject *parent) : QObject(parent) {
    settingsPath_ = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)
                    + "/settings.json";

    if (!QFile::exists(settingsPath_)) {
        if (!QDir().mkpath(QFileInfo(settingsPath_).absolutePath())) {
            return;
        }

        QFile::copy(":/app/config/settings.json", settingsPath_);
    }

    load();
    applyTheme();
}

bool AppSettings::darkTheme() const {
    return darkThemeFlag_;
}

void AppSettings::setDarkTheme(bool const dark) {
    if (darkThemeFlag_ == dark) return;
    darkThemeFlag_ = dark;
    applyTheme();
    save();
    emit darkThemeChanged();
    emit themeChanged();
}

ThemeColors AppSettings::currentTheme() const {
    return currentTheme_;
}

QColor AppSettings::editorBg() const { return currentTheme_.editorBg; }
QColor AppSettings::editorText() const { return currentTheme_.editorText; }
QColor AppSettings::editorSelection() const { return currentTheme_.editorSelection; }
QColor AppSettings::editorSelectionText() const { return currentTheme_.editorSelectionText; }
QColor AppSettings::editorBorder() const { return currentTheme_.editorBorder; }
QColor AppSettings::lineNumbersBg() const { return currentTheme_.lineNumbersBg; }
QColor AppSettings::lineNumbersText() const { return currentTheme_.lineNumbersText; }
QColor AppSettings::previewBg() const { return currentTheme_.previewBg; }
QColor AppSettings::previewText() const { return currentTheme_.previewText; }
QColor AppSettings::previewBorder() const { return currentTheme_.previewBorder; }
QColor AppSettings::previewFill() const { return currentTheme_.previewFill; }

void AppSettings::applyPalette() const {
    QPalette pal;

    // Основные цвета
    pal.setColor(QPalette::Window, currentTheme_.editorBg);
    pal.setColor(QPalette::WindowText, currentTheme_.editorText);
    pal.setColor(QPalette::Base, currentTheme_.editorBg);
    pal.setColor(QPalette::Text, currentTheme_.editorText);
    pal.setColor(QPalette::Button, currentTheme_.editorBg);
    pal.setColor(QPalette::ButtonText, currentTheme_.editorText);
    pal.setColor(QPalette::Highlight, currentTheme_.editorSelection);
    pal.setColor(QPalette::HighlightedText, currentTheme_.editorSelectionText);
}

void AppSettings::load() {
    QFile file(settingsPath_);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Cannot open settings file:" << settingsPath_;
        return;
    }

    QJsonDocument const doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    if (!doc.isObject()) return;
    QJsonObject obj = doc.object();

    QFile templateFile(":/app/config/settings.json");
    bool merged = false;
    if (templateFile.open(QIODevice::ReadOnly)) {
        QJsonDocument const templateDoc = QJsonDocument::fromJson(templateFile.readAll());
        templateFile.close();
        if (templateDoc.isObject()) {
            QJsonObject const templateObj = templateDoc.object();
            mergeJsonObjects(obj, templateObj);
            merged = true;
        }
    }

    if (merged) {
        if (QFile outFile(settingsPath_); outFile.open(QIODevice::WriteOnly)) {
            outFile.write(QJsonDocument(obj).toJson());
            outFile.close();
        }
    }

    darkThemeFlag_ = obj.value("darkTheme").toBool(false);
    QJsonObject const themeObj = obj.value("theme").toObject();
    lightThemeColors_ = loadThemeFromJson(themeObj.value("light").toObject());
    darkThemeColors_  = loadThemeFromJson(themeObj.value("dark").toObject());
}

void AppSettings::save() const {
    if (!QDir().mkpath(QFileInfo(settingsPath_).absolutePath())) {
        return;
    }

    QFile file(settingsPath_);
    if (!file.open(QIODevice::WriteOnly)) return;

    QJsonObject obj;
    obj["darkTheme"] = darkThemeFlag_;
    file.write(QJsonDocument(obj).toJson());
    file.close();
}

void AppSettings::applyTheme() {
    currentTheme_ = darkThemeFlag_ ? darkThemeColors_ : lightThemeColors_;
    applyPalette();
    emit themeChanged();
}