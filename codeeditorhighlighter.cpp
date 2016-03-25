#include "codeeditorhighlighter.h"
#include <QDebug>

CodeEditorHighlighter::CodeEditorHighlighter(QTextDocument* parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    /**
     * In constructor all formats are set up.
     * All formating rules are stored in highlightingRules QVector<HighlightingRule>
     */

    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);

    QStringList keywordPatterns;
    keywordPatterns << "\\bselect\\b" << "\\bfrom\\b" << "\\bwhere\\b" << "\\border\\b"
                    << "\\bby\\b" << "\\bas\\b" << "\\bcount\\b" << "\\bhaving\\b" << "\\bgroup\\b"
                    << "\\binsert\\b" << "\\binto\\b" << "\\bvalues\\b" << "\\bcreate\\b" << "\\btable\\b"
                    << "\\bupdate\\b" << "\\bset\\b" << "\\band\\b" << "\\bor\\b";

    foreach(const QString& pattern, keywordPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    QStringList types;
    types << "\\binteger\\b" << "\\btext\\b" << "\\breal\\b" << "\\bbool\\b" << "\\bblob\\b"
          << "\\bprimary\\b" << "\\bkey\\b" << "\\bnot\\b" << "\\bnull\\b" << "\\bautoincrement\\b";

    typeFormat.setForeground(Qt::blue);
    typeFormat.setFontItalic(true);

    foreach(const QString& type, types) {
        rule.pattern = QRegExp(type);
        rule.format = typeFormat;
        highlightingRules.append(rule);
    }
}

/**
 * The highlightBlock(QString&) function highlight the given block of text.
 * This function is called when necessary by the rich text engine.
 */
void CodeEditorHighlighter::highlightBlock(const QString &text)
{
    foreach(const HighlightingRule& rule, highlightingRules) {
        QRegExp expression(rule.pattern);
        /**
         * Enabling minimal matching for expression.
         * If minimal is false, matching is greedy(maximal) which is the default.
         */
        expression.setMinimal(true);
        int index = expression.indexIn(text.toLower());
        while(index >= 0) {
            /**
             * Searching all strings matching expression.
             * Then setting format for each occurence.
             */
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
}
