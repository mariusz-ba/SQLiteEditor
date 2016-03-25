/**
 *
 * CodeEditorHighlighter.h
 *
 * Created by Mariusz Baran on 24.03.2016
 * Copyright (c) 2016 Mariusz Baran. All rights reserved
 *
 * Syntax highlighter for SQL language.
 */

#ifndef CODEEDITORHIGHLIGHTER_H
#define CODEEDITORHIGHLIGHTER_H

#include <QObject>
#include <QSyntaxHighlighter>

class CodeEditorHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    CodeEditorHighlighter(QTextDocument* parent = 0);

protected:
    void highlightBlock(const QString &text) Q_DECL_OVERRIDE;

private:
    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };

    QVector<HighlightingRule> highlightingRules;

    QRegExp commentStartExpression;
    QRegExp commentEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat typeFormat;
};

#endif // CODEEDITORHIGHLIGHTER_H
