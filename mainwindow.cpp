#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "operaciones_con_qstrings.h"
#include "xlsxdocument.h"
#include <QFileDialog>
#include <QSystemTrayIcon>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    setMinimumSize(540,480);
    setMaximumSize(640,480);

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    qApp->quit();
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->texto->setText("<b> Abriendo ....\n </b>");
    abrir_y_acTG();
    if (ui->autoSalir->isChecked())
        on_pushButton_clicked();
}

void MainWindow::abrir_y_acTG()
{    
    //qDebug() << " abriendo E:\\MILLO\\BASE DE DATOS EMBARAZADAS.xlsx ....\n";

    QString nombreArch = QFileDialog::getOpenFileName(this, tr("Abrir archivo"),
                                                      "/home",
                                                      tr("Archivos de Excel (*.xlsx *.xls)"));

    if (nombreArch==""){
        ui->texto->setText("");
        return;
    }
    double TG[500]; int cant = 0;
    //inicializacion de TG
    for (int i=0; i<500; ++i)
        TG[i] = 0.0;

    {
        QXlsx::Document xlsx(nombreArch);

        int fila = 4;
        QString celda, nombre;
        while (xlsx.read("B" + QString::number(fila)).toString() != ""){
            nombre = xlsx.read("B" + QString::number(fila)).toString();
            celda = "I" + QString::number(fila);
            //lectura de la fecha ult menstr del archivo y asignacion---------
            QString s = xlsx.read(celda).toString();

            Operaciones_con_QStrings ocs; QDate fecha_mens;
            if (ocs.esCadFechadeSeparadorGuion(s))
                s = ocs.convertirFechaSepGuionAFechaSlash(s);
            QString s2 = ocs.invertirFecha(s);
            if (!ocs.esFechaLaCad(s2,"/"))
            {
                TG[fila-4] = -2;
            }
            else{
                fecha_mens = ocs.Convertir_cadena_a_fecha(s);

                //calcular  Tiempo de Gestacion hasta la fecha en base a la fecha de menstr
                double restg = 42.3;

                double diff_dia, t;
                QDate fecha_hoy = QDate().currentDate();
                if (fecha_mens.daysTo(fecha_hoy) < 0){
                    TG[fila-4] = -3;    //fecha posterior a la fecha de hoy
                    fila++;
                    continue;
                }
                int dias = static_cast<int>(fecha_mens.daysTo(fecha_hoy));

                diff_dia = (dias % 7) * 0.1;
                t = static_cast<int>(dias / 7);
                restg = t + diff_dia;
                if (restg > 42.2)
                    TG[fila-4] = -1;
                //"Para esta fecha de menstruación, ya la gestante no debe constar en el registro <b>(TG > 42.2 semanas)</b>");

                else
                    TG[fila-4] = restg;      //sumo el dia * 0.1
            }
            //----------tomar el resultado obtenido y dejar un solo lugar decimal------
            TG[fila-4] = QString::number(TG[fila-4],'g',3).toDouble();

            //-----------------imprimir---en---pantalla--------------------------------
            /*qDebug () << "B4   "<< nombre
                          << "  fecha ult menstr >s es "<< s << " : s2 es "<<s2 <<
                             "fecha QDATE es "<< fecha_mens
                          << " TG ------> " << TG[fila-4];*/
            //----------escribir---en ---archivo--------------------------------------

            fila++;
        }   //--del while
        cant = fila;
    }
    //escribir para salvar
    //qDebug () << "Todo Leido y TG generados ... Abriendo archivo para escribir los TG calculados y salvar";
    ui->texto->setText("Todo Leido y TG generados ... Abriendo archivo para escribir los TG calculados y salvar"
                       );
    QXlsx::Document xlsx(nombreArch);
    for (int fila = 4; fila < cant; ++fila)
        xlsx.write("E"+ QString::number(fila),
                   (TG[fila-4]>-1)? QVariant(TG[fila-4]):
            (TG[fila-4]==-1)?"Parió":(TG[fila-4]==-3)?"f.posterior":"No fecha menstr.");
    //salvar
    //qDebug () << "salvando ...... espere ";
    bool hay_Error= xlsx.save();
    if (hay_Error) {
        ui->s_error->setText("<p><span style=\" color:#ff0000;\">Error:  No se pudo salvar plenamente el archivo.<br/>Pero no se pierde ninguna informacion relevante<br/> Desagrupe hojas, ponga filtros a sus datos<br/>e inmovilice sus vistas de nuevo.</span></p>");
        //qDebug () << s_error();
    }
    //qDebug () << "Finalizado  .... ";
    ui->texto->setText("<b>Finalizado<b>");

}
