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

QString AppSettings::fontFamily() const {
    return fontFamily_;
}

void AppSettings::setFontFamily(const QString &family) {
    if (fontFamily_ == family) return;
    fontFamily_ = family;
    save();
    emit fontFamilyChanged();
}

int AppSettings::fontSize() const {
    return fontSize_;
}

void AppSettings::setFontSize(int size) {
    size = qBound(12, size, 32);
    if (fontSize_ == size) return;
    fontSize_ = size;
    save();
    emit fontSizeChanged();
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

void AppSettings::increaseFontSize() {
    int newSize = fontSize_ + 2;
    if (newSize > 32) newSize = 32;
    setFontSize(newSize);
}

void AppSettings::decreaseFontSize() {
    int newSize = fontSize_ - 2;
    if (newSize < 12) newSize = 12;
    setFontSize(newSize);
}

void AppSettings::load() {
    QFile templateFile(":/app/config/settings.json");
    if (!templateFile.open(QIODevice::ReadOnly)) {
        return;
    }
    QJsonDocument const templateDoc = QJsonDocument::fromJson(templateFile.readAll());
    templateFile.close();
    if (!templateDoc.isObject()) return;
    QJsonObject const templateObj = templateDoc.object();

    QJsonObject userObj;
    if (QFile file(settingsPath_); file.open(QIODevice::ReadOnly)) {
        QJsonDocument const userDoc = QJsonDocument::fromJson(file.readAll());
        file.close();
        if (userDoc.isObject()) {
            userObj = userDoc.object();
        }
    }

    mergeJsonObjects(userObj, templateObj);

    if (QFile outFile(settingsPath_); outFile.open(QIODevice::WriteOnly)) {
        outFile.write(QJsonDocument(userObj).toJson());
        outFile.close();
    }

    darkThemeFlag_ = userObj.value("darkTheme").toBool(false);
    fontFamily_ = userObj.value("fontFamily").toString("Courier New");
    fontSize_ = userObj.value("fontSize").toInt(16);

    QJsonObject const themeObj = userObj.value("theme").toObject();
    lightThemeColors_ = loadThemeFromJson(themeObj.value("light").toObject());
    darkThemeColors_  = loadThemeFromJson(themeObj.value("dark").toObject());
}

void AppSettings::save() {
    if (!QDir().mkpath(QFileInfo(settingsPath_).absolutePath())) {
        emit errorOccurred(tr("Cannot create directory: %1").arg(settingsPath_));
        return;
    }

    QJsonObject obj;
    QFile file(settingsPath_);
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument const doc = QJsonDocument::fromJson(file.readAll());
        file.close();
        if (doc.isObject()) {
            obj = doc.object();
        }
    }

    obj["darkTheme"] = darkThemeFlag_;
    obj["fontFamily"] = fontFamily_;
    obj["fontSize"] = fontSize_;

    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(obj).toJson());
        file.close();
    }
}

void AppSettings::applyTheme() {
    currentTheme_ = darkThemeFlag_ ? darkThemeColors_ : lightThemeColors_;
    applyPalette();
    emit themeChanged();
}