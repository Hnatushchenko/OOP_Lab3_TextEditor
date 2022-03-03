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

    TextEdit = new QPlainTextEdit(this);
    TextEdit->setFont(TextEditFont);

    QAction *_new = new QAction("New", this);
    QAction *open = new QAction("Open…", this);
    QAction *save = new QAction("Save", this);
    QAction *saveAs = new QAction("Save As…", this);
    QAction *exit = new QAction("Exit", this);

    QAction *undo = new QAction("Undo", this);
    QAction *redu = new QAction("Redu", this);
    QAction *cut = new QAction("Cut", this);
    QAction *copy = new QAction("Copy", this);
    QAction *paste = new QAction("Paste", this);
    QAction *selectAll = new QAction("Select all", this);

    QAction *font = new QAction("Font…", this);

    _new->setShortcut(tr("CTRL+N"));
    open->setShortcut(tr("CTRL+O"));
    save->setShortcut(tr("CTRL+S"));

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

    QMenu *format = menuBar()->addMenu("Format");
    format->addAction(font);

    QVBoxLayout *vbox = new QVBoxLayout(this);

    connect(_new, &QAction::triggered, this, &TextEditor::newFile);
    connect(open, &QAction::triggered, this, &TextEditor::OpenFile);
    connect(save, &QAction::triggered, this, &TextEditor::saveFile);
    connect(saveAs, &QAction::triggered, this, &TextEditor::saveAs);
    connect(exit, &QAction::triggered, this, &QApplication::quit);
    connect(TextEdit, &QPlainTextEdit::textChanged, this, &TextEditor::textChanged);

    connect(undo, &QAction::triggered, TextEdit, &QPlainTextEdit::undo);
    connect(redu, &QAction::triggered, TextEdit, &QPlainTextEdit::redo);
    connect(cut, &QAction::triggered, TextEdit, &QPlainTextEdit::cut);
    connect(copy, &QAction::triggered, TextEdit, &QPlainTextEdit::copy);
    connect(paste, &QAction::triggered, TextEdit, &QPlainTextEdit::paste);
    connect(selectAll, &QAction::triggered, TextEdit, &QPlainTextEdit::selectAll);

    connect(font, &QAction::triggered, this, &TextEditor::changeFont);

    vbox->addWidget(TextEdit);
    QWidget *centralWidget = new QWidget;
    centralWidget->setLayout(vbox);
    setCentralWidget(centralWidget);
}

void TextEditor::OpenFile()
{
    openedFilePath = QFileDialog::getOpenFileName(this, "Opening", QDir::homePath(), "Text files (*.txt)");

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
        openedFilePath = QFileDialog::getSaveFileName(this, "Save File", QDir::homePath(), "Text files (*.txt)");
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
    openedFilePath = QFileDialog::getSaveFileName(this, "Save File", QDir::homePath(), "Text files (*.txt)");

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

void TextEditor::changeFont()
{
    TextEdit->setFont(QFontDialog::getFont(0, TextEdit->font()));
}

TextEditor::~TextEditor()
{
    delete ui;
}

