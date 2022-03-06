#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QMainWindow>
#include <QWidget>
#include <QMenu>
#include <QMenuBar>
#include <QPlainTextEdit>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QDir>
#include <QFontDialog>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrinter>

QT_BEGIN_NAMESPACE
namespace Ui { class TextEditor; }
QT_END_NAMESPACE

class TextEditor : public QMainWindow
{
    Q_OBJECT

public:
    TextEditor(QWidget *parent = nullptr);
    ~TextEditor();

private:
    QString openedFilePath;
    QString openedFileName;
    QString initialFileText;

    Ui::TextEditor *ui;
    QPlainTextEdit * TextEdit;

private slots:
    void newFile();
    void OpenFile();
    void saveFile();
    void saveAs();
    void on_actionPrint_triggered();

    void textChanged();


    void changeFont();
};
#endif // TEXTEDITOR_H
