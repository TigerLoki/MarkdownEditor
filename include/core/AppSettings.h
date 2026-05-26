#pragma once

#include <QObject>
#include <QString>
#include <QColor>
#include "parser/renderers/ThemeColors.h"

class AppSettings : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool darkTheme               READ darkTheme              WRITE setDarkTheme          NOTIFY darkThemeChanged)
    Q_PROPERTY(ThemeColors currentTheme     READ currentTheme                                       NOTIFY themeChanged)

    Q_PROPERTY(QColor editorBg              READ editorBg                                           NOTIFY themeChanged)
    Q_PROPERTY(QColor editorText            READ editorText                                         NOTIFY themeChanged)
    Q_PROPERTY(QColor editorSelection       READ editorSelection                                    NOTIFY themeChanged)
    Q_PROPERTY(QColor editorSelectionText   READ editorSelectionText                                NOTIFY themeChanged)
    Q_PROPERTY(QColor editorBorder          READ editorBorder                                       NOTIFY themeChanged)
    Q_PROPERTY(QColor lineNumbersBg         READ lineNumbersBg                                      NOTIFY themeChanged)
    Q_PROPERTY(QColor lineNumbersText       READ lineNumbersText                                    NOTIFY themeChanged)
    Q_PROPERTY(QColor previewBg             READ previewBg                                          NOTIFY themeChanged)
    Q_PROPERTY(QColor previewText           READ previewText                                        NOTIFY themeChanged)
    Q_PROPERTY(QColor previewBorder         READ previewBorder                                      NOTIFY themeChanged)
    Q_PROPERTY(QColor previewFill           READ previewFill                                        NOTIFY themeChanged)

public:
    explicit AppSettings(QObject *parent = nullptr);

    [[nodiscard]] bool darkTheme() const;
    void setDarkTheme(bool dark);

    [[nodiscard]] ThemeColors currentTheme() const;

    [[nodiscard]] QColor editorBg() const;
    [[nodiscard]] QColor editorText() const;
    [[nodiscard]] QColor editorSelection() const;
    [[nodiscard]] QColor editorSelectionText() const;
    [[nodiscard]] QColor editorBorder() const;
    [[nodiscard]] QColor lineNumbersBg() const;
    [[nodiscard]] QColor lineNumbersText() const;
    [[nodiscard]] QColor previewBg() const;
    [[nodiscard]] QColor previewText() const;
    [[nodiscard]] QColor previewBorder() const;
    [[nodiscard]] QColor previewFill() const;

    void applyPalette() const;

signals:
    void darkThemeChanged();
    void themeChanged();

private:
    void load();
    void save() const;
    void applyTheme();

    bool darkThemeFlag_ = false;
    ThemeColors currentTheme_;
    ThemeColors lightThemeColors_;
    ThemeColors darkThemeColors_;
    QString settingsPath_;
};
