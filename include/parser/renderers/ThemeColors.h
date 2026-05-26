#pragma once
#include <QObject>
#include <QColor>

class ThemeColors {
    Q_GADGET
    Q_PROPERTY(QColor editorBg              MEMBER editorBg             CONSTANT)
    Q_PROPERTY(QColor editorText            MEMBER editorText           CONSTANT)
    Q_PROPERTY(QColor editorSelection       MEMBER editorSelection      CONSTANT)
    Q_PROPERTY(QColor editorSelectionText   MEMBER editorSelectionText  CONSTANT)
    Q_PROPERTY(QColor editorBorder          MEMBER editorBorder         CONSTANT)
    Q_PROPERTY(QColor lineNumbersBg         MEMBER lineNumbersBg        CONSTANT)
    Q_PROPERTY(QColor lineNumbersText       MEMBER lineNumbersText      CONSTANT)
    Q_PROPERTY(QColor previewBg             MEMBER previewBg            CONSTANT)
    Q_PROPERTY(QColor previewText           MEMBER previewText          CONSTANT)
    Q_PROPERTY(QColor previewBorder         MEMBER previewBorder        CONSTANT)
    Q_PROPERTY(QColor previewFill           MEMBER previewFill          CONSTANT)

public:
    QColor editorBg;
    QColor editorText;
    QColor editorSelection;
    QColor editorSelectionText;
    QColor editorBorder;
    QColor lineNumbersBg;
    QColor lineNumbersText;
    QColor previewBg;
    QColor previewText;
    QColor previewBorder;
    QColor previewFill;

    bool operator==(const ThemeColors &other) const {
        return editorBg == other.editorBg
            && editorText == other.editorText
            && editorSelection == other.editorSelection
            && editorSelectionText == other.editorSelectionText
            && editorBorder == other.editorBorder
            && lineNumbersBg == other.lineNumbersBg
            && lineNumbersText == other.lineNumbersText
            && previewBg == other.previewBg
            && previewText == other.previewText
            && previewBorder == other.previewBorder
            && previewFill == other.previewFill;
    }
    bool operator!=(const ThemeColors &other) const {
        return !(*this == other);
    }
};
Q_DECLARE_METATYPE(ThemeColors)