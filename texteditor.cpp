#include "texteditor.h"
#include "ui_texteditor.h"

TextEditor::TextEditor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TextEditor)
{
    ui->setupUi(this);

    QAction *_new = new QAction("New", this);
    QAction *open = new QAction("Open…", this);
    QAction *save = new QAction("Save", this);
    QAction *saveAs = new QAction("Save As…", this);
    QAction *exit = new QAction("Exit", this);

    connect(exit, &QAction::triggered, this, &QApplication::quit);

    QMenu *file = menuBar()->addMenu("File");
    file->addAction(_new);
    file->addAction(open);
    file->addAction(save);
    file->addAction(saveAs);
    file->addSeparator();
    file->addAction(exit);

    QVBoxLayout *vbox = new QVBoxLayout(this);

    TextEdit = new QPlainTextEdit(this);

    vbox->addWidget(TextEdit);

    QWidget *centralWidget = new QWidget;
    centralWidget->setLayout(vbox);
    setCentralWidget(centralWidget);
}

TextEditor::~TextEditor()
{
    delete ui;
}

