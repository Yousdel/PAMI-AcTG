#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "xlsxdocument.h"

namespace Ui {
class widget;
}

class QSystemTrayIcon;

class widget : public QWidget
{
    Q_OBJECT

public:
    explicit widget(int argc, char **argv, QWidget *parent = nullptr);
    ~widget();

protected:
    void timerEvent(QTimerEvent *event) override;

private slots:
    void on_cerrarPushButton_clicked();

    void on_abrirPushButton_clicked();

private:
    Ui::widget *ui;

    QSystemTrayIcon *mSystemTrayIcon;
    QString m_filename;
    bool m_arrastro = 0, m_finalizado = false, m_entro_a_abrir = false;
    int m_timer;

    inline bool abrir_y_acTG() ;
};

#endif // WIDGET_H
