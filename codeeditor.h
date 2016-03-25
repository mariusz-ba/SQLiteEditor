/**
 *
 * CodeEditor.h
 *
 * Created by Mariusz Baran on 24.03.2016
 * Copyright (c) 2016 Mariusz Baran. All rights reserved
 *
 * Code editor class that supports simple completion of code.
 */

#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QObject>
#include <QTextEdit>
#include <QCompleter>
#include <QKeyEvent>
#include <QAbstractItemView>
#include <QScrollBar>

#include "codeeditorhighlighter.h"

class CodeEditor : public QTextEdit
{
    Q_OBJECT
public:
    explicit CodeEditor(QWidget* parent = 0);
    ~CodeEditor();

    void setCompleter(QCompleter* completer);
    QCompleter* completer() const;

protected:
    void keyPressEvent(QKeyEvent *e) Q_DECL_OVERRIDE;
    void focusInEvent(QFocusEvent *e) Q_DECL_OVERRIDE;

private slots:
    void insertCompletion(const QString& completion);

private:
    QString textUnderCursor() const;
    QCompleter* c;
    CodeEditorHighlighter* highlighter;
};

#endif // CODEEDITOR_H
