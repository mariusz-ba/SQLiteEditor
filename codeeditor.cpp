#include "codeeditor.h"
#include <QDebug>

CodeEditor::CodeEditor(QWidget* parent)
    :QTextEdit(parent)
{
    //Setting up the syntax highlighter for code editor.
    highlighter = new CodeEditorHighlighter(document());
}

CodeEditor::~CodeEditor()
{
}

void CodeEditor::setCompleter(QCompleter *completer)
{
    /**
     * When disconnecting c signals program finishes unexpectedly
     * if(c)
     *     disconnect(c, SIGNAL(activated(QString)), this, SLOT(insertCompletion(QString)));
     */

    c = completer;

    if(!c)
        return;

    c->setWidget(this);
    c->setCompletionMode(QCompleter::PopupCompletion);
    c->setCaseSensitivity(Qt::CaseInsensitive);
    connect(c, SIGNAL(activated(QString)), this, SLOT(insertCompletion(QString)));
}

QCompleter* CodeEditor::completer() const
{
    return c;
}

/**
 * insertCopletion() takes the completion that user accepted and
 * insert it at the current text cursor position.
 * param @completion string& - completion accepted by the user
 */
void CodeEditor::insertCompletion(const QString &completion)
{
    if(c->widget() != this)
        return;

    QTextCursor tc = QTextCursor(document());

    tc.setPosition(this->textCursor().position());
    tc.select(QTextCursor::WordUnderCursor);
    tc.removeSelectedText();
    tc.insertText(completion);

    setTextCursor(tc);
}

QString CodeEditor::textUnderCursor() const
{
    QTextCursor tc = textCursor();
    tc.select(QTextCursor::WordUnderCursor);
    return tc.selectedText();
}

void CodeEditor::focusInEvent(QFocusEvent *e)
{
    if(c)
        c->setWidget(this);
    QTextEdit::focusInEvent(e);
}

void CodeEditor::keyPressEvent(QKeyEvent *e)
{
    if(c && c->popup()->isVisible()) {
        switch(e->key()) {
        case Qt::Key_Enter:
        case Qt::Key_Return:
        case Qt::Key_Escape:
        case Qt::Key_Tab:
        case Qt::Key_Backtab:
            e->ignore();
            return;
        default:
            break;
        }
    }

    bool isShortcut = ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_Space);
    if(!c || !isShortcut)
        QTextEdit::keyPressEvent(e);

    const bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
    if(!c || (ctrlOrShift && e->text().isEmpty()))
        return;

    static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-=");
    bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
    QString completionPrefix = textUnderCursor();

    if(!isShortcut && (hasModifier || e->text().isEmpty() || completionPrefix.length() < 1
                       || eow.contains(e->text().right(1)))) {
        c->popup()->hide();
        return;
    }

    if(completionPrefix != c->completionPrefix()) {
        c->setCompletionPrefix(completionPrefix);
        c->popup()->setCurrentIndex(c->completionModel()->index(0, 0));
    }

    QRect cr = cursorRect();
    cr.setWidth(c->popup()->sizeHintForColumn(0) + c->popup()->verticalScrollBar()->sizeHint().width());
    c->complete(cr);
}


