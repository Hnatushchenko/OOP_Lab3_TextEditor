#include "texteditor.h"
#include "ui_texteditor.h"

TextEditor::TextEditor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TextEditor)
{
    ui->setupUi(this);

    openedFileName = "new";

    QFont TextEditFont("Lucida Console, Regular");
    TextEditFont.setPixelSize(19);

    TextEdit = new QTextEdit(this);
    TextEdit->setFont(TextEditFont);

    QAction *_new = new QAction("New", this);
    QAction *open = new QAction("Open…", this);
    QAction *save = new QAction("Save", this);
    QAction *saveAs = new QAction("Save As…", this);
    QAction *print = new QAction("Print…", this);
    QAction *exit = new QAction("Exit", this);

    QAction *undo = new QAction("Undo", this);
    QAction *redu = new QAction("Redu", this);
    QAction *cut = new QAction("Cut", this);
    QAction *copy = new QAction("Copy", this);
    QAction *paste = new QAction("Paste", this);
    QAction *selectAll = new QAction("Select all", this);
    QAction *find = new QAction("Find", this);
    QAction *findAndReplace = new QAction("Find and replace", this);


    QAction *font = new QAction("Font…", this);

    _new->setShortcut(tr("CTRL+N"));
    open->setShortcut(tr("CTRL+O"));
    save->setShortcut(tr("CTRL+S"));
    print->setShortcut(tr("CTRL+P"));

    undo->setShortcut(tr("CTRL+Z"));
    redu->setShortcut(tr("CTRL+Y"));
    cut->setShortcut(tr("CTRL+X"));
    copy->setShortcut(tr("CTRL+C"));
    paste->setShortcut(tr("CTRL+V"));
    selectAll->setShortcut(tr("CTRL+A"));

    QMenu *file = menuBar()->addMenu("File");
    file->addAction(_new);
    file->addAction(open);
    file->addAction(save);
    file->addAction(saveAs);
    file->addSeparator();
    file->addAction(print);
    file->addSeparator();
    file->addAction(exit);


    QMenu *edit = menuBar()->addMenu("Edit");
    edit->addAction(undo);
    edit->addAction(redu);
    edit->addSeparator();
    edit->addAction(cut);
    edit->addAction(copy);
    edit->addAction(paste);
    edit->addSeparator();
    edit->addAction(selectAll);
    edit->addAction(find);
    edit->addAction(findAndReplace);

    QMenu *format = menuBar()->addMenu("Format");
    format->addAction(font);

    QVBoxLayout *vbox = new QVBoxLayout(this);

    connect(_new, &QAction::triggered, this, &TextEditor::newFile);
    connect(open, &QAction::triggered, this, &TextEditor::OpenFile);
    connect(save, &QAction::triggered, this, &TextEditor::saveFile);
    connect(saveAs, &QAction::triggered, this, &TextEditor::saveAs);
    connect(print, &QAction::triggered, this, &TextEditor::printFile);
    connect(exit, &QAction::triggered, this, &QApplication::quit);

    connect(TextEdit, &QTextEdit::textChanged, this, &TextEditor::textChanged);

    connect(undo, &QAction::triggered, TextEdit, &QTextEdit::undo);
    connect(redu, &QAction::triggered, TextEdit, &QTextEdit::redo);
    connect(cut, &QAction::triggered, TextEdit, &QTextEdit::cut);
    connect(copy, &QAction::triggered, TextEdit, &QTextEdit::copy);
    connect(paste, &QAction::triggered, TextEdit, &QTextEdit::paste);
    connect(selectAll, &QAction::triggered, TextEdit, &QTextEdit::selectAll);
    connect(find, &QAction::triggered, this, &TextEditor::find);
    connect(findAndReplace, &QAction::triggered, this, &TextEditor::findAndReplace);

    connect(font, &QAction::triggered, this, &TextEditor::changeFont);

    connect(TextEdit, &QTextEdit::cursorPositionChanged, this, [this]()
    {
        QTextCharFormat fmt;
        fmt.setBackground(Qt::white);

        QTextCursor cursor(TextEdit->document());
        cursor.setPosition(0, QTextCursor::MoveAnchor);
        cursor.setPosition(TextEdit->toPlainText().length(), QTextCursor::KeepAnchor);
        cursor.setCharFormat(fmt);
    });

    vbox->addWidget(TextEdit);
    QWidget *centralWidget = new QWidget;
    centralWidget->setLayout(vbox);
    setCentralWidget(centralWidget);
}

void TextEditor::OpenFile()
{
    openedFilePath = QFileDialog::getOpenFileName(this, "Opening", QDir::homePath(), "Text Documents (*.txt);;All Files (*.*)");

    QFile file(openedFilePath);
    if(!file.open(QFile::ReadOnly))
    {
        return;
    }

    QTextStream inStream(&file);
    initialFileText = inStream.readAll();

    TextEdit->setPlainText(initialFileText);

    openedFileName = QFileInfo(openedFilePath).fileName();
    this->setWindowTitle(openedFileName + ": Text Editor");

    file.close();
}

void TextEditor::saveFile()
{
    if(openedFilePath.isEmpty())
    {
        openedFilePath = QFileDialog::getSaveFileName(this, "Save File", QDir::homePath(), "Text Documents (*.txt);;All Files (*.*)");
    }

    QFile file(openedFilePath);

    if(!file.open(QFile::WriteOnly))
    {
        return;
    }

    openedFileName = QFileInfo(openedFilePath).fileName();
    QTextStream outStream(&file);

    initialFileText = TextEdit->toPlainText();
    outStream << initialFileText;
    this->setWindowTitle(openedFileName + ": Text Editor");

    file.close();
}

void TextEditor::saveAs()
{
    openedFilePath = QFileDialog::getSaveFileName(this, "Save File", QDir::homePath(), "Text Documents (*.txt);;All Files (*.*)");

    QFile file(openedFilePath);
    if(!file.open(QFile::WriteOnly))
    {
        return;
    }

    openedFileName = QFileInfo(openedFilePath).fileName();
    QTextStream outStream(&file);

    initialFileText = TextEdit->toPlainText();
    outStream << initialFileText;
    this->setWindowTitle(openedFileName + ": Text Editor");

    file.close();
}

void TextEditor::printFile()
{
    QPrinter printer;
    QPrintDialog pDialog(&printer, this);

    if(pDialog.exec() == QDialog::Rejected) return;

    TextEdit->print(&printer);
}

void TextEditor::textChanged()
{
    if(TextEdit->toPlainText() != initialFileText)
    {
        this->setWindowTitle("*" + openedFileName + ": Text Editor");
    }
    else
    {
        this->setWindowTitle(openedFileName + ": Text Editor");
    }

}

void TextEditor::newFile()
{
    if(TextEdit->toPlainText() != initialFileText)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Text Editor");
        msgBox.setText("The document has been modified.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();

        switch(ret)
        {
        case QMessageBox::Cancel:
            return;

        case QMessageBox::Save:
            if(openedFilePath.isEmpty())
            {
                openedFilePath = QFileDialog::getSaveFileName(this, "Save File", QDir::homePath(), "Text files (*.txt)");
            }

            QFile file(openedFilePath);

            if(!file.open(QFile::WriteOnly))
            {
                return;
            }

            QTextStream outStream(&file);
            outStream << TextEdit->toPlainText();

            file.close();
            break;
        }
    }

    openedFilePath = "";
    openedFileName = "new";
    TextEdit->setPlainText("");
    initialFileText = "";

    this->setWindowTitle(openedFileName + ": Text Editor");
}

void TextEditor::findAndReplace()
{
    QWidget *findAndReplaceDialogWindow = new QWidget;
    findAndReplaceDialogWindow->setWindowTitle("Find and replace");
    findAndReplaceDialogWindow->setMinimumWidth(330);

    QVBoxLayout *vbox = new QVBoxLayout(findAndReplaceDialogWindow);
    QFormLayout *form = new QFormLayout(findAndReplaceDialogWindow);
    form->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);

    QLineEdit *findEdit = new QLineEdit(findAndReplaceDialogWindow);
    QLineEdit *replaceEdit = new QLineEdit(findAndReplaceDialogWindow);
    form->addRow("Find:", findEdit);
    form->addRow("Replace with:", replaceEdit);

    QPushButton *button = new QPushButton(findAndReplaceDialogWindow);
    button->setText("Replace all");

    connect(button, &QPushButton::clicked, this, [=](){
        QString textBefore = TextEdit->toPlainText();
        TextEdit->setPlainText(textBefore.replace(findEdit->text(), replaceEdit->text()));
          delete form;
          delete vbox;
          delete findEdit;
          delete replaceEdit;
          delete button;
          delete findAndReplaceDialogWindow;
    });

    vbox->addLayout(form);
    vbox->addWidget(button);

    findAndReplaceDialogWindow->show();
}

void TextEditor::find()
{
    QWidget *findDialogWindow = new QWidget;
    findDialogWindow->setWindowTitle("Find");
    findDialogWindow->setMinimumWidth(260);

    QVBoxLayout *vbox = new QVBoxLayout(findDialogWindow);
    QFormLayout *form = new QFormLayout(findDialogWindow);
    form->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);

    QLineEdit *findEdit = new QLineEdit(findDialogWindow);

    form->addRow("Find:", findEdit);


    QPushButton *button = new QPushButton(findDialogWindow);
    button->setText("Find");

    connect(button, &QPushButton::clicked, this,
                   [this, findEdit]() {
                        QTextCharFormat fmt;
                        fmt.setBackground(Qt::white);

                        QTextCursor cursor(TextEdit->document());
                        cursor.setPosition(0, QTextCursor::MoveAnchor);
                        cursor.setPosition(TextEdit->toPlainText().length(), QTextCursor::KeepAnchor);
                        cursor.setCharFormat(fmt);

                        int end = 0;
                        QString text = this->TextEdit->toPlainText();
                        for(int i = 0; i < text.count(findEdit->text()); ++i)
                        {
                            int begin = text.indexOf(findEdit->text(), end);
                            if (begin != -1)
                            {
                                end = begin + findEdit->text().size();

                                QTextCharFormat fmt;
                                fmt.setBackground(QColor::fromRgb(0, 120, 215));

                                QTextCursor cursor(TextEdit->document());
                                cursor.setPosition(begin, QTextCursor::MoveAnchor);
                                cursor.setPosition(end, QTextCursor::KeepAnchor);
                                cursor.setCharFormat(fmt);
                            }
                        }
                   });

    vbox->addLayout(form);
    vbox->addWidget(button);

    findDialogWindow->show();
}

void TextEditor::changeFont()
{
    TextEdit->setFont(QFontDialog::getFont(0, TextEdit->font()));
}

TextEditor::~TextEditor()
{
    delete ui;
}




