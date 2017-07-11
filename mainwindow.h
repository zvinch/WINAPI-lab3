#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <windows.h>
namespace Ui {
class MainWindow;
}

//struct COPYDATASTRUCT{
//    ULONG_PTR dwData; //32-битовое число для передачи данных принимающему процессу – может служить признаком – какого рода данные Вы отправляете
//    DWORD cbData; //количество байт, на которые указывает lpData
//    PVOID lpData; //указатель на данные. Может быть NULL.
////Замечание: будьте осторожны - это адрес в адресном пространстве процесса-процесса-отправителя (каким бы образом Вы этот блок не создавали – в стеке, глобально, динамически)! => в нем не может быть в свою очередь других адресов
//};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


public slots:
    void on_pushButton_AutonomusChildProcess_clicked();
private slots:

    void on_PushButtonCalc_clicked();


    void on_pushButtonCmd_clicked();

    void on_pushButtonKillCmd_clicked();

    void on_pushButtonKillNotepad_clicked();

    void on_pushButtonKillCalc_clicked();



    void on_pushButtonNotepad_clicked();

    void on_pushButtonSoliter_clicked();

    void on_pushButtonKillSolitere_clicked();

    void on_pushButtonCreateChild_clicked();


    
    void on_sendMesageTOChild_clicked();

    void on_SendTextlineEdit_textChanged(const QString &arg1);

    void on_CreateSgaredMemmory_clicked();

    void on_ReadSharedMemmory_clicked();

    void on_WriteSharedMemmory_clicked();

    void on_pushButton_7_clicked();

    void on_CreateSharedFile_clicked();

    void on_ReadSharedFile_clicked();

    void on_WriteSharedFile_clicked();

    void on_CloseSharedFile_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;

    STARTUPINFO NotepadSi;
    PROCESS_INFORMATION NotepadPi;

    STARTUPINFO CalcSi;
    PROCESS_INFORMATION CalcPi;

    STARTUPINFO CmdSi;
    PROCESS_INFORMATION CmdPi;

    STARTUPINFO SolitSi;
    PROCESS_INFORMATION SolitPi;

    STARTUPINFO ChildSi;
    PROCESS_INFORMATION ChildPi;
    QString timeexecuted(HANDLE h);

    PVOID p_SharedMem = NULL;
    HANDLE hFileMap = NULL;
    PVOID p_SharedFile = NULL;
    HANDLE hFileMap2 = NULL;
    HANDLE hSharedFile = NULL;


};

#endif // MAINWINDOW_H
