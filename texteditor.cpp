#include "texteditor.h"
#include "ui_texteditor.h"

TextEditor::TextEditor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TextEditor)
{
    ui->setupUi(this);

    openedFileName = "new";
    TextEdit = new QPlainTextEdit(this);

    QAction *_new = new QAction("New", this);
    QAction *open = new QAction("Open…", this);
    QAction *save = new QAction("Save", this);
    QAction *saveAs = new QAction("Save As…", this);
    QAction *exit = new QAction("Exit", this);

    _new->setShortcut(tr("CTRL+N"));
    open->setShortcut(tr("CTRL+O"));
    save->setShortcut(tr("CTRL+S"));

    QMenu *file = menuBar()->addMenu("File");
    file->addAction(_new);
    file->addAction(open);
    file->addAction(save);
    file->addAction(saveAs);
    file->addSeparator();
    file->addAction(exit);

    QVBoxLayout *vbox = new QVBoxLayout(this);


    connect(_new, &QAction::triggered, this, &TextEditor::newFile);
    connect(open, &QAction::triggered, this, &TextEditor::OpenFile);
    connect(save, &QAction::triggered, this, &TextEditor::saveFile);
    connect(exit, &QAction::triggered, this, &QApplication::quit);
    connect(TextEdit, &QPlainTextEdit::textChanged, this, &TextEditor::textChanged);

    vbox->addWidget(TextEdit);
    QWidget *centralWidget = new QWidget;
    centralWidget->setLayout(vbox);
    setCentralWidget(centralWidget);
}

void TextEditor::OpenFile()
{
    openedFilePath = QFileDialog::getOpenFileName(this, "Opening", QDir::homePath());

    QFile file(openedFilePath);
    if(!file.open(QFile::ReadOnly))
    {
        //QMessageBox::warning(this, "Warning", "Can't open the file: " + openedFileName);
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
        openedFilePath = QFileDialog::getSaveFileName(this, "Save File", QDir::homePath());
    }

    QFile file(openedFilePath);
    if(!file.open(QFile::WriteOnly))
    {
        //QMessageBox::warning(this, "Warning", "Can't open the file: " + openedFilePath);
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

}

TextEditor::~TextEditor()
{
    delete ui;
}

