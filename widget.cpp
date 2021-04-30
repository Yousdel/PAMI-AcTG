#include "widget.h"
#include "ui_widget.h"
//librerias mias
#include "operaciones_con_qstrings.h"
//libreria QTXlsx
#include "header/xlsxdocument.h"
//Qt
#include <QFileDialog>
#include <QPicture>
#include <QSystemTrayIcon>

widget::widget(int argc, char **argv, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::widget)
{    
    setMinimumSize(660,480);
    setMaximumSize(660,480);
    mSystemTrayIcon = new QSystemTrayIcon(this);
    mSystemTrayIcon->setIcon(QIcon(":/icono copia.png"));
    mSystemTrayIcon->setToolTip("Actualizador de Tiempos de Gestación PAMIacTG");
    mSystemTrayIcon->setVisible(1);
    ui->setupUi(this);
    if (argc == 2){
        m_arrastro = 1;
        m_filename = argv[1];
        int cantslashs = m_filename.split("\\").length();
        ui->texto->setText("<b> Abriendo y analizando <br/>"+
                            m_filename.section("\\",cantslashs-1)
                           +" ....\n </b>");
        m_timer = startTimer(1000);
    }
}

widget::~widget()
{

    delete ui;
}

void widget::timerEvent(QTimerEvent *event)
{
    (void) event;

    if (m_finalizado)
        killTimer(m_timer);

    if (m_arrastro && !m_entro_a_abrir){
        m_entro_a_abrir = true;
        on_abrirPushButton_clicked();
    }
}


void widget::on_cerrarPushButton_clicked()
{
    qApp->quit();
}

void widget::on_abrirPushButton_clicked()
{

    if (m_arrastro && ! m_filename.contains(".xlsx") ) {
        ui->s_error->setText(
                    "Error: El archivo seleccionado\n no es un archivo de excel");
        return;
    }

    ui->abrirPushButton->setEnabled(0);
    ui->cerrarPushButton->setEnabled(0);

    bool res = abrir_y_acTG();

    ui->abrirPushButton->setEnabled(1);
    ui->cerrarPushButton->setEnabled(1);

    if (res) {
        mSystemTrayIcon->showMessage("Mensaje de tu Actualizador de Edades Gestacionales","Proceso Finalizado exitosamente.");
        mSystemTrayIcon->show();

        if (ui->autoSalir->isChecked())
            on_cerrarPushButton_clicked();
    }
}

bool widget::abrir_y_acTG()
{
    QString nombreArch = m_arrastro? m_filename :
            QFileDialog::getOpenFileName(this, tr("Abrir archivo"),
                                                      "/home",
                                                      tr("Archivos de Excel (*.xlsx *.xls)"));

    if (nombreArch == ""){
        ui->texto->setText("");
        return 0;
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
    bool hay_Error= xlsx.saveAs(nombreArch);
    if (hay_Error) {
        ui->s_error->setText(
                    "<p><span style=\" color:#ff0000;\">Error:  No se pudo salvar plenamente el archivo.<br/>Pero no se pierde ninguna informacion relevante<br/> Desagrupe hojas, ponga filtros a sus datos<br/>e inmovilice sus vistas de nuevo.</span></p>");

    }
    ui->texto->setText("<b>Finalizado<b>");
    m_finalizado = true;
    return 1;
}
