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
    void OpenFile();
    void saveFile();
    void textChanged();
};
#endif // TEXTEDITOR_H
