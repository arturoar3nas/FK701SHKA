#ifndef MATH_H
#define MATH_H


#include <QDebug>
#include <QString>
#include <QStringList>
#include <stdio.h>

/**
 * @brief The Math class
 * Lo dividiremos en 2 grupos:
 * Grupo 1) Tique/Tique Factura B,C/Tique Nota de Credito B,C
 * Grupo 2) Tique Factura A/ Tique Nota de Credito A
 *
 */
class Math
{
private:
    double __total_iva;
    double __total_pvp;
    double __total_bi;
    double __total_mii;
    double __change;
    double __rest;
    double __subtotal;
    double __monto;
    double __canceled;
    double __bi;

    int __pluNegativo(QStringList qstrlplu);
public:
//---------------------------------
//Constructores
    Math();

//---------------------------------
//Metodos Publicos
    double getPvp();
    double getIva();
    double getSubTotal();
    double getChange();
    double getRest(double dPay);
    double getMonto();
    void   setCanceled(double dcanceled);
    double getCanceled();
    void   setIva(double dIva);
    void   setSubtotal(double dSubtotal);
    void   setPvp(double dPvp);

    void doCalcs(QStringList qstrlitem);
    void cleanMounts();
    void calcGarage(QStringList qstrlitem);
    void Dicount(QStringList qstrlitem);
    void discount_Discharge(int opc, double imount);

    void discountByanulItem(QStringList qstrlitem);

    double getBi();
    void setChange(double change);
};

#endif // MATH_H
