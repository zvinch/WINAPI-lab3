#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <QString>
#include <QTextCodec>
#include <QDateTime>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    ZeroMemory( &CalcSi, sizeof(CalcSi) );
    CalcSi.cb = sizeof(CalcSi);

    ZeroMemory( &NotepadSi, sizeof(NotepadSi) );
    NotepadSi.cb = sizeof(NotepadSi);

    ZeroMemory( &CmdSi, sizeof(CmdSi) );
    CmdSi.cb = sizeof(CmdSi);

    ZeroMemory( &SolitSi, sizeof(SolitSi) );
    SolitSi.cb = sizeof(SolitSi);

    ZeroMemory( &ChildSi, sizeof(ChildSi) );
    ChildSi.cb = sizeof(ChildSi);


}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::timeexecuted(HANDLE h){



    FILETIME  creationTime;
    FILETIME  exitTime;
    FILETIME  kernelTime;
    FILETIME  userTime;

    ZeroMemory( &creationTime, sizeof(creationTime) );
    ZeroMemory( &exitTime, sizeof(exitTime) );
    ZeroMemory( &kernelTime, sizeof(kernelTime) );
    ZeroMemory( &userTime, sizeof(userTime) );

    GetProcessTimes(h,
            &creationTime,
            &exitTime,
            &kernelTime,
            &userTime);

    SYSTEMTIME ct;
    SYSTEMTIME et;
    ZeroMemory( &ct, sizeof(ct) );
    ZeroMemory( &et, sizeof(et) );
    FileTimeToSystemTime(&creationTime,&ct);
    FileTimeToSystemTime(&exitTime, &et);
    QString str;
    unsigned int hours =  et.wHour - ct.wHour;
    unsigned int minutes = et.wMinute - ct.wMinute;
    unsigned int seconds =  et.wSecond - ct.wSecond;
    unsigned int  ms = et.wMilliseconds - ct.wMilliseconds ;
    str = QString::number(hours)+":"+QString::number(minutes)+":"+QString::number(minutes)+":"+QString::number(seconds)+":"+QString::number(ms);
    return str;

}

void MainWindow::on_pushButtonCmd_clicked()
{
    TCHAR name[] = L"cmd.exe";
  bool result = CreateProcess(
              NULL ,
               name,
                  NULL,
                  NULL,
                  FALSE,
                    NORMAL_PRIORITY_CLASS,
                  NULL,
                  NULL,
                  &CmdSi,
                  &CmdPi
                );
    if(result){
       ui->console->append("Cmd created");


       while(true)
           {
               DWORD res = MsgWaitForMultipleObjects(1,//количество ожидаемых объектов
                   &CmdPi.hProcess,//указатель на массив «ожидаемых» дескрипторов
                   FALSE, //TRUE – ждать освобождения всех объектов + событие
                       //FALSE - когда любой из ожидаемых объектов освободился или событие
                   INFINITE,//интервал ожидания в мс
                   QS_ALLEVENTS);//тип события - QS_KEY | QS_MOUSE

               if(res==WAIT_OBJECT_0)
                   break;// дождались (у нас всего один объект).
               //Если false, то событие – обрабатываем его
               MSG m;
               while(PeekMessage(&m,0,0,0,PM_REMOVE))//пока в очереди есть сообщения – удаляем из очереди и обрабатываем, иначе (если сообщений нет) PeekMessage неизрасходованный квант «передаст» другому потоку
               {
                   DispatchMessage(&m);//”обрабатываем» сообщение
               }
           }
           // Закрывайте описатели, как только необходимость в них отпадает!!!

           DWORD exitcode = 0;
           GetExitCodeProcess(CmdPi.hProcess, &exitcode);
           ui->console->append("Cmd exitcode "+QString::number(exitcode,16));
           ui->console->append(timeexecuted(CmdPi.hProcess));
            CloseHandle( CmdPi.hProcess );


    } else{
       ui->console->append("Notepad creation failed");
    }
}

void MainWindow::on_pushButtonKillCmd_clicked()
{
    DWORD exitcode = 0;
    GetExitCodeProcess(CmdPi.hProcess, &exitcode);
    ui->console->append("Cmd exitcode "+QString::number(exitcode,16));

    if(exitcode == STILL_ACTIVE){
        UINT uExitCode = 0;
        TerminateProcess(CmdPi.hProcess, uExitCode);
        ui->console->append("Cmd uExitCode "+QString::number(uExitCode,10));

    }
    ui->console->append("Cmd execution time "+timeexecuted(CmdPi.hProcess));


}

void MainWindow::on_pushButtonKillNotepad_clicked()
{

}

void MainWindow::on_pushButtonKillCalc_clicked()
{
    DWORD exitcode = 0;
    GetExitCodeProcess(CalcPi.hProcess, &exitcode);
    ui->console->append("Calc exitcode "+QString::number(exitcode,16));
    ui->console->append("Calc execution time "+timeexecuted(CalcPi.hProcess));
    if(exitcode == STILL_ACTIVE){
        UINT uExitCode = 0;
        TerminateProcess(CalcPi.hProcess, uExitCode);
        ui->console->append("Calc execution time "+timeexecuted(CalcPi.hProcess));
        ui->console->append("Calc uExitCode "+QString::number(uExitCode,10));
    }
}

void MainWindow::on_PushButtonCalc_clicked()
{
    TCHAR name[] = L"calc.exe";
  bool result = CreateProcess(
              NULL ,
               name,
                  NULL,
                  NULL,
                  FALSE,
                    NORMAL_PRIORITY_CLASS,
                  NULL,
                  NULL,
                  &CalcSi,
                  &CalcPi
                );
    if(result){
       ui->console->append("Calc created");


       DWORD exitcode = 0;
       GetExitCodeProcess(CalcPi.hProcess, &exitcode);
       ui->console->append("Calc exitcode "+QString::number(exitcode,16));
    } else{
       ui->console->append("Calc creation failed");
    }
}

void MainWindow::on_pushButtonNotepad_clicked()
{

    TCHAR name[] = L"notepad.exe";
  bool result = CreateProcess(
              NULL ,
               name,
                  NULL,
                  NULL,
                  FALSE,
                    NORMAL_PRIORITY_CLASS,
                  NULL,
                  NULL,
                  &NotepadSi,
                  &NotepadPi
                );
    if(result){
       ui->console->append("Notepad created");
       WaitForSingleObject(NotepadPi.hProcess, 3000);
       DWORD exitcode = 0;
       GetExitCodeProcess(NotepadPi.hProcess, &exitcode);
       ui->console->append("Notepad exitcode "+QString::number(exitcode,16));
       if(exitcode == STILL_ACTIVE){
           UINT uExitCode = 0;
           TerminateProcess(NotepadPi.hProcess, uExitCode);
           ui->console->append("Notepad uExitCode "+QString::number(uExitCode,10));
       }
      ui->console->append("Cmd execution time "+timeexecuted(NotepadPi.hProcess));

    } else{
       ui->console->append("Notepad creation failed");
    }
}

void MainWindow::on_pushButtonSoliter_clicked()
{

    SolitSi.cb = sizeof(SolitSi);

    SolitSi.dwFlags = STARTF_USESHOWWINDOW;
    SolitSi.wShowWindow = SW_SHOWMAXIMIZED;

    TCHAR name[] = L"notepad.exe";
  bool result = CreateProcess(
              NULL ,
               name,
                  NULL,
                  NULL,
                  FALSE,
                    NORMAL_PRIORITY_CLASS,
                  NULL,
                  NULL,
                  &SolitSi,
                  &SolitPi
                );
    if(result){
       ui->console->append("notepad created");

       WaitForSingleObject(SolitPi.hProcess, 10000);
       DWORD exitcode = 0;
       GetExitCodeProcess(SolitPi.hProcess, &exitcode);
       ui->console->append("notepad exitcode "+QString::number(exitcode,16));
    } else{
       ui->console->append("notepad creation failed");
    }
}

void MainWindow::on_pushButtonKillSolitere_clicked()
{
    DWORD exitcode = 0;
    GetExitCodeProcess(SolitPi.hProcess, &exitcode);
    ui->console->append("Solit exitcode "+QString::number(exitcode,16));
    if(exitcode == STILL_ACTIVE){
        UINT uExitCode = 0;
        TerminateProcess(SolitPi.hProcess, uExitCode);
        ui->console->append("Solit uExitCode "+QString::number(uExitCode,10));
    }

}

void MainWindow::on_pushButtonCreateChild_clicked()
{
    SECURITY_ATTRIBUTES sa;
    ZeroMemory( &sa, sizeof(sa) );
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = 0;
    sa.bInheritHandle = TRUE;
    TCHAR fname[] = L"file.txt";
    HANDLE fHandle = CreateFile(fname,
                                GENERIC_READ | GENERIC_WRITE,
                                FILE_SHARE_READ | FILE_SHARE_WRITE,
                                &sa,
                                CREATE_ALWAYS,
                                FILE_ATTRIBUTE_NORMAL,
                                0);
    if(fHandle != INVALID_HANDLE_VALUE){
        ChildSi.dwFlags = STARTF_USESTDHANDLES;
        ChildSi.hStdOutput =fHandle;





        TCHAR name[] = L"C://Users//mzvin//Projects//MPP//lab1_Child//build-Lab1_Child-Desktop_Qt_5_8_0_MinGW_32bit-Debug//debug//Lab1_Child.exe";
        bool result = CreateProcess(
                    NULL,
                    name ,
                    NULL,
                    NULL,
                    TRUE,
                    NORMAL_PRIORITY_CLASS,
                    NULL,
                    NULL,
                    &ChildSi,
                    &ChildPi
                    );

        CloseHandle(ChildPi.hThread);
        CloseHandle(ChildPi.hProcess);
        CloseHandle(fHandle);



//        if(result){
//            ui->console->append("Child created");

//            WaitForSingleObject(ChildPi.hProcess, 40000);
//            DWORD exitcode = 0;
//            GetExitCodeProcess(ChildPi.hProcess, &exitcode);
//            CloseHandle(fHandle);

//            ui->console->append("Child exitcode "+QString::number(exitcode,16));
//        } else{
//            ui->console->append("Child creation failed");
//        }
    }
}

void MainWindow::on_pushButton_AutonomusChildProcess_clicked()
{
    TCHAR name[] = L"calc.exe";
  bool result = CreateProcess(
              NULL ,
               name,
                  NULL,
                  NULL,
                  FALSE,
                    NORMAL_PRIORITY_CLASS,
                  NULL,
                  NULL,
                  &CalcSi,
                  &CalcPi
                );
    if(result){
       ui->console->append("Calc created");


       DWORD exitcode = 0;
       GetExitCodeProcess(CalcPi.hProcess, &exitcode);
       ui->console->append("Calc exitcode "+QString::number(exitcode,16));
    } else{
       ui->console->append("Calc creation failed");
    }
}

void MainWindow::on_sendMesageTOChild_clicked()
{
    HWND h = FindWindow( 0, L"ChildLab1" );
    if(h != 0)
    {   static UINT MyMessage=::RegisterWindowMessage(L"MESSAGELAB1");
        if(MyMessage==0)
        {

            return;
        }
        SendMessage(h,MyMessage,0,1337);

    }

}

void MainWindow::on_SendTextlineEdit_textChanged(const QString &arg1)
{
    HWND h = FindWindow( 0, L"ChildLab1" );
    if ( h != 0)
    {
        COPYDATASTRUCT data;
        data.cbData = arg1.toUtf8().size() +1; //сколько передаем
        data.dwData = 33;                      // тэг сообщения
        data.lpData = arg1.toUtf8().data();    //указатель на данные
        SendMessage(h, WM_COPYDATA, 0, (LPARAM)&data);
    }
}

void MainWindow::on_CreateSgaredMemmory_clicked()
{

    if (hFileMap==NULL)
    {
        hFileMap = CreateFileMapping
            (INVALID_HANDLE_VALUE,

            0,
            PAGE_READWRITE,
            0,
            100,
            L"ShareMemory");

        p_SharedMem = static_cast<char*>(
          MapViewOfFile
            (hFileMap,

            FILE_MAP_READ|FILE_MAP_WRITE,
            0,
            0,
            0));

    }

}

void MainWindow::on_ReadSharedMemmory_clicked()
{
    if (hFileMap != NULL)
    {
        char* pp = static_cast<char*>(p_SharedMem);
        QString str = QString::fromUtf8(pp);
        ui->lineEditContentsOfSharedMemmory->setText(str);
    }


}

void MainWindow::on_WriteSharedMemmory_clicked()
{
    if (hFileMap != NULL)
    {
        char* pp = static_cast<char*>(p_SharedMem);
        QString str = ui->AddToSharedMemmoryEdit->text();
        char* tmp = str.toUtf8().data();

        MoveMemory((void*)pp,(void*)tmp, str.length());
        //strcpy(pp, tmp);
        ui->AddToSharedMemmoryEdit-> setText("");
    }

}

void MainWindow::on_pushButton_7_clicked()
{
    if (hFileMap != NULL)
    {
        //очистка дескрипторов
        UnmapViewOfFile(p_SharedMem);
        CloseHandle(hFileMap);
        hFileMap=NULL;
        p_SharedMem=NULL;

    }

}

void MainWindow::on_CreateSharedFile_clicked()
{
    if (hFileMap2==NULL)
    {
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = 0;
    sa.bInheritHandle = true;
    hSharedFile = CreateFile(
                L"C:\\shared_file.txt",
                GENERIC_READ | GENERIC_WRITE,
                FILE_SHARE_READ | FILE_SHARE_WRITE,
                &sa,
                CREATE_ALWAYS,
                FILE_ATTRIBUTE_NORMAL,
                0);

        hFileMap2 = CreateFileMapping
            (hSharedFile,  //дескриптор файла
            0,                      //LPSECURITY_ATTRIBUTES
            PAGE_READWRITE,         //страницы предназначены как для чтения, так и для записи
            0,
            100,                    //размер отображения
            L"SharedFile");        //имя

        p_SharedFile = static_cast<char*>(
          MapViewOfFile           // отображаем файл на адресное пространство
            (hFileMap2,          //дескриптор объекта «проекция файла»,
                                //полученный предыдущим CreateFileMapping() или OpenFileMapping()
            FILE_MAP_READ|FILE_MAP_WRITE,     //чтение+запись
            0,                  //dwFileOffsetHigh - смещение для view
            0,                  //dwFileOffsetLow
            0));                //dwNumberOfBytesToMap, 0 - представление с указанного смещения и до конца файла

        }
}

void MainWindow::on_ReadSharedFile_clicked()
{
    if (hFileMap2 != NULL)
    {
        char* pp = static_cast<char*>(p_SharedFile);
        QString str = QString::fromUtf8(pp);
        ui->lineEdit_FromSharedFile->setText(str);
    }

}

void MainWindow::on_WriteSharedFile_clicked()
{
    if (hFileMap2 != NULL)
    {
        char* pp = static_cast<char*>(p_SharedFile);

        QString str = ui->lineEdit_ToSharedFile->text();
        char* tmp = str.toUtf8().data();
        //strcpy(pp, tmp);
        MoveMemory((void*)pp,(void*)tmp, str.length());

        //ui->lineEdit_ToSharedFile-> setText("");
    }

}

void MainWindow::on_CloseSharedFile_clicked()
{
    if (hFileMap2 != NULL)
    {

        UnmapViewOfFile(p_SharedFile);
        CloseHandle(hFileMap2);
        CloseHandle(hSharedFile);
        hFileMap2=NULL;
        p_SharedFile=NULL;
        hSharedFile=NULL;

    }

}

void MainWindow::on_pushButton_2_clicked()
{
    TCHAR name[] = L"notepad.exe";
  bool result = CreateProcess(
              NULL ,
               name,
                  NULL,
                  NULL,
                  FALSE,
                    NORMAL_PRIORITY_CLASS,
                  NULL,
                  NULL,
                  &SolitSi,
                  &SolitPi
                );


TCHAR name2[] = L"cmd.exe";
bool result2 = CreateProcess(
          NULL ,
           name2,
              NULL,
              NULL,
              FALSE,
                NORMAL_PRIORITY_CLASS,
              NULL,
              NULL,
              &CmdSi,
              &CmdPi
            );


HANDLE handles[2];
 handles[0] = CmdPi.hProcess;
 handles[1] = SolitPi.hProcess;
QString str;

while(true)
    {
        DWORD res = MsgWaitForMultipleObjects(2,//количество ожидаемых объектов
            handles,//указатель на массив «ожидаемых» дескрипторов
            FALSE, //TRUE – ждать освобождения всех объектов + событие
                //FALSE - когда любой из ожидаемых объектов освободился или событие
            INFINITE,//интервал ожидания в мс
            QS_ALLEVENTS);//тип события - QS_KEY | QS_MOUSE

    //res – если значение лежит в диапазоне от WAIT_OBJECT_0 до (WAIT_OBJECT_0+количество ожидаемых объектов -1), то освободился один из ожидаемых объектов,
//если возвращаемое значение больше или равно (WAIT_OBJECT_0+количество ожидаемых объектов -1), то поступило сообщение.
        if(res < WAIT_OBJECT_0+2){
            break;// дождались (у нас всего один объект).
        }
        //Если false, то событие – обрабатываем его
        MSG m;
        while(PeekMessage(&m,0,0,0,PM_REMOVE))//пока в очереди есть сообщения – удаляем из очереди и обрабатываем, иначе (если сообщений нет) PeekMessage неизрасходованный квант «передаст» другому потоку
        {
            DispatchMessage(&m);//”обрабатываем» сообщение
        }
    }


//bool flag = true;
//while(flag)
//    {

//    for(int i = 0; i < 2; i++)
//    {
//        DWORD exitCode;
//        GetExitCodeProcess(handles[i], &exitCode);
//        if(exitCode != STILL_ACTIVE){
//            if(i == 0){
//                str += "CMD ";
//            } else if(i ==1){
//                str += "Notepad ";
//            }
//                str +=  QString(" завершен выходом")+QString("\n");
//                flag = false;
//                break;
//        }
//    }

//}

    for(int i = 0; i < 2; i++)
    {
        DWORD exitCode;
        GetExitCodeProcess(handles[i], &exitCode);
        if(exitCode == STILL_ACTIVE){
                TerminateProcess(handles[i], i);
                if(i == 0){
                    str += "CMD ";
                } else if(i ==1){
                    str += "Notepad ";
                }
                str += QString(" завершен насильно")+QString("\n");
            }  else {
            if(i == 0){
                str += "CMD ";
            } else if(i ==1){
                str += "Notepad ";
            }
            str += QString(" Завершен штатно")+QString("\n");
        }

    }

    QMessageBox::information(this, "Info", str);

}
