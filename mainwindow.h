#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "xlsxdocument.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QSystemTrayIcon;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;

    QSystemTrayIcon *mSystemTrayIcon;
    void abrir_y_acTG() ;
    bool falta_encabezado_en_xlsx(
            const
            QXlsx::Worksheet &wSERIE //hoja SERIE del excel
            , QString &s) const;
};
#endif // MAINWINDOW_H
