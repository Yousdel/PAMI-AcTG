#ifndef OPERACIONES_CON_QSTRINGS_H
#define OPERACIONES_CON_QSTRINGS_H

#include "operaciones_con_qstrings_global.h"
#include <QStringList>
#include <QDate>

class OPERACIONES_CON_QSTRINGSSHARED_EXPORT Operaciones_con_QStrings
{

public:

    Operaciones_con_QStrings(){

    }
    ///Siendo cadena una lista de elementos separados por coma, esta función sirve para
    /// agregar (agregar_eliminar = 1)  o eliminar (agregar_eliminar = 0)
    ///el elemento a/de la cadena siempre que elemento no coincida en
    /// excepcionesEnMayusculasYSinTildes, y no se encuentre ya en la
    ///  cadena en el caso de agregar_eliminar = 1
    void agregar_eliminar_un_elemento_de_texto_a
    (QString &cadena, const QString &elemento,
     const QStringList &excepcionesEnMayusculasYSinTildes,
     const bool &agregar_eliminar=1 )
    {
        QString texto_a_comp_excepciones ,
                texto_c_espacios = elemento,
                texto_mayus_s_tild = elemento;
        QStringList Lista_elem = cadena.split(","); //lista de elementos
        //quitar espacios delante----------------
        QStringList copiaLista_elem;
        foreach (const QString &s , Lista_elem){
            QString s9 = s;
            while (s9.at(0) == ' ')
                s9.remove(0,1);
            copiaLista_elem << s9;
        }
        Lista_elem = copiaLista_elem;

        QStringList Lista_mayus, LexcepcionesEnMayusculasYSinTildes;    //elaborar la lista en mayusculas y sin tildes-y la lista de
        //excepciones igualmente pero sin espacios ni puntos---------
        foreach  (const QString& s , Lista_elem){
            QString ss = s;
            convertir_a_mayusculas_y_sin_tildes(ss);
            Lista_mayus << ss;
        }
        foreach (const QString & s , excepcionesEnMayusculasYSinTildes)
        {
            QString ss = s;
            Quitar_puntos_y_espacios(ss);
            LexcepcionesEnMayusculasYSinTildes << ss;
        }

        convertir_a_mayusculas_y_sin_tildes(texto_mayus_s_tild);//convertir el elemento a mayusc y sin tildes
        texto_a_comp_excepciones = texto_mayus_s_tild;//sera el texto a comparar con la lista de excepciones
        Quitar_puntos_y_espacios(texto_a_comp_excepciones); // quitar puntos y
        //espacios a la cadena para  el siguiente paso si va a agregar

        cambiar_bckslash_x_espacio(texto_c_espacios);  //cambiar los \n por
        //espacios, que este será el texto que se va a agregar

        if (Lista_mayus.contains(texto_mayus_s_tild) == false && agregar_eliminar == false){
            return; //si se desea eliminar una cosa que no esta, salir
        }

        int idx =  Lista_mayus.indexOf(texto_mayus_s_tild);//idx es el indice donde esta
        QString cad = "";

        if (!agregar_eliminar  )   //quiero eliminar------------------
        {
            if ( idx == -1) //no esta no lo puedo remover
            {return;}
            Lista_elem.removeAt(idx);
            int i = 0;
            foreach (const QString& s , Lista_elem){ //elaborar la cadena cad con lo que queda
                if (i != Lista_elem.size()-1)
                    cad += s + ",";
                else
                    cad += s;
                ++i;
            }
            cadena.clear();
            cadena=cad;        //finalmente asignar cad al componente y  visualizarlo

            return;
        }

        //agregar 1er paso - --- analisis de la lista de excepciones------------------
        bool salir = 0;
        for (int i = 0; i < LexcepcionesEnMayusculasYSinTildes.size(); ++i) {
            salir =  salir ||
                    (texto_a_comp_excepciones == LexcepcionesEnMayusculasYSinTildes.at(i));
            if (salir)
                return; //no se usa mas texto_a_comp_excepciones, salir porque es una excepcion
        }
        //para agregar------------------------
        //chequear primero si el texto no se encuentra ya en las cadena, pues no lo agregare en tal caso
        if (idx != -1)
            return;

        //agregarlo en caso de que no este
        cad = cadena;
        if (!cad.isEmpty())
            cad.append(",");           //agregar la coma de separador
        cad.append( texto_c_espacios);
        cadena.clear();
        cadena.append(cad);               //añadir a las cadena
    }

    ///Extraer el nombre y la extension de la ruta filename
    const QString &Extraer_nombre_y_extension(const QString &Complete_filename)
    {
        subcad = Complete_filename ;

        int idx = Complete_filename.indexOf('/');

        while (idx != -1 && !Complete_filename.isEmpty()) {
            subcad.remove(0, idx+1);
            idx = subcad.indexOf('/');
        }
        const QString *const ptr = &subcad;
        return (*ptr);
    }
    ///Quitar los puntos y espacios de cad
    void Quitar_puntos_y_espacios(QString &cad)
    {   // quitar puntos y espacios a la cadena
        cad.remove(' ');
        cad.remove('.');
    }
    ///cambiar todos los saltos de linea de cad por espacios
    void cambiar_bckslash_x_espacio(QString &cad)
    {       //cambiarle los \n por espacios a la cadena

        cad.replace('\n',' ');
    }
    ///convertir cad a mayúsculas y sin tildes
    void convertir_a_mayusculas_y_sin_tildes(QString &cad)
    {           //quitarle las tildes y convertir a mayusculas la cadena

        cad.replace("á", "a");
        cad.replace("é","e");
        cad.replace("í","i");
        cad.replace("ó", "o");
        cad.replace("ú", "u");

        cad.replace("Á", "A");
        cad.replace("É", "E");
        cad.replace("Í", "I");
        cad.replace("Ó", "O");
        cad.replace("Ú", "U");

        cad = cad.toUpper();        //llevar el resultado a mayusculas
    }

    //fechas-------------------------
    /// Devuelve true si la cadena cad es fecha con separador '-', sino false.
    bool esCadFechadeSeparadorGuion(const QString &cad)
    {
        return esFechaLaCad(cad,"-");
    }
    /// Devuelve true si la cadena cad es fecha con separador '/', sino false.
    bool esCadFechadeSeparadorSlash(const QString &cad)
    {
        return esFechaLaCad(cad, "/");
    }

    ///Devuelve la fecha DD/MM/AA como AA/MM/DD y viceversa
    const QString &invertirFecha(const QString &s, const QString &sep = "/")
    {

        if (esFechaLaCad(s,sep))
        {
            s1 = "";
            const QString *const ptr= &s1;
            return *ptr;
        }
        QStringList Li, L = s.split(sep);
        foreach (const QString &sss, L){
            Li.push_front(sss);
        }
        ss = Li.join(sep);

        const QString * const ptr = &ss;
        return (*ptr);
    }
    /// Devuelve la fecha s separada por '-' como una fecha separada por '/'
    const QString &convertirFechaSepGuionAFechaSlash(const QString &s,
                                                     const bool &leer_a_la_inversa = 1)
    {
        QStringList L;
        if (esFechaLaCad(s,"-") == false){
            s1 = "";
            const QString *const ptr= &s1;
            return *ptr;
        }
        L = s.split("-");
        if (!leer_a_la_inversa){
            int i=0;
            foreach (const QString&sss , L){
                ss.append(sss);
                i++;
                if (i<3)
                    ss.append("/");
            }
        }
        else {
            QStringList LL;
            foreach (const QString&sss , L){
                LL.push_front(sss);
            }
            int i = 0;
            foreach (const QString&sss , LL){
                ss.append(sss);
                i++;
                if (i<3)
                    ss.append("/");
            }
        }
        const QString *const ptr= &ss;
        return (*ptr);
    }

    ///Convertir la fecha cad separada con separador sep como QDate
    const QDate &Convertir_cadena_a_fecha(const QString &cad,
                                          const QString &sep="/", const bool &leer_a_la_inversa=0) {
        QStringList L = cad.split(sep);
        int arg[3], i = 0;

        foreach (QString s , L)
            arg[i++] = s.toInt();

        if (!leer_a_la_inversa)
        {
            fecha.setDate(arg[2],arg[1],arg[0]);
            const QDate *const ptr = &fecha;
            return *ptr;
        }
        else {
            fecha.setDate(arg[0], arg[1], arg[2]);
            const QDate *const ptr = &fecha;
            return *ptr;
        }
    }
    ///Tomar la fecha date y devolverla en forma de cadena en formato DDMMAA con separador '/'
    const QString &Convertir_fecha_short_a_cadenaDDMMAA(const QDate &date,
                                                        const QString &sep="/") {
        res ="";
        int year, mes, dia;
        QDate d = date;
        d.getDate(&year, &mes, &dia);
        if (year == 0 || mes == 0 ||dia == 0){
            const QString *const ptr = &res;
            return *ptr; //retorno cadena vacia si la fecha no es valida
        }
        if (dia <10)
            res.append("0");
        res.append(QString::number(dia));
        res.append(sep);
        if (mes <10)
            res.append("0");
        res.append(QString::number(mes));
        res.append(sep);
        res.append(QString::number(year));
        const QString *const ptr = &res;
        return *ptr;
    }

    ///funcion auxiliar para las algunas fciones esCadFechadeSeparadorGuion y esCadFechadeSeparadorSlash
    /// La fecha s debe suministrarse en forma de AA/MM/DD si sep es '/'
    bool esFechaLaCad(const QString &s, const QString &sep)
    {
        QStringList L = s.split(sep);
        if (L.count() == 3){
            int Lint[3], i = 0;
            foreach (const QString &ss , L)
            {
                bool ok;
                int x = ss.toInt(&ok);
                if (x == 0 ||  !ok)
                    return 0;
                Lint[i]  = x;
                ++i;
            }
            return QDate::isValid(Lint[0], Lint[1], Lint[2]);
        }
        return  0;
    }
private:
    QString s, s1, ss, subcad, res;
    QDate fecha;
};

#endif // OPERACIONES_CON_QSTRINGS_H
