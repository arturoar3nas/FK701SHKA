#include "math.h"

/**
 * @brief Math::Math
 * Calculo de bi,iva,mii,descuento y recargo
 * Precison:
 * Cantidad: 5 enteros, 4 decimales
 * Valor Unitario: 7 enteros, 4 decimales
 * PVP, BI: 7 enteros, 2 decimales
 * MIIF: 7 enteros, 2 decimales
 * TAVA, TAVB: 0 enteros, 8 decimales
 * ALICUOTA IVA: 2 enteros, 2 decimales
 * TASA: 0 enteros, 4 decimales
 * FACTOR: 2 enteros, 2 decimales
 */
Math::Math()
{
    __total_iva = 0;
    __total_pvp = 0;
    __total_bi = 0;
    __total_mii = 0;
    __change = 0;
    __rest = 0;
    __subtotal = 0;
    __monto = 0;
    __bi = 0;
}

//------------------------------------------
//getters
double Math::getCanceled()
{
    return __canceled;
}
double Math::getBi()
{
    return __bi;
}
double Math::getMonto()
{
    return __monto;
}
double Math::getPvp()
{
    return __total_pvp;
}
double Math::getIva()
{
    return __total_iva;
}
double Math::getSubTotal()
{
    __subtotal = __total_pvp - __total_iva;
    return __subtotal;
}
double Math::getChange()
{
    return __change;
}
double Math::getRest(double dPay)
{
    __rest += dPay;

    return __rest;
}

//-------------------------------------------------
//Setters
void Math::setCanceled(double dcanceled)
{    
    __canceled += dcanceled;
}

void Math::setPvp(double dPvp)
{
    __total_pvp = dPvp;
}

void Math::setIva(double dIva)
{
    __total_iva = dIva;
}

void Math::setSubtotal(double dSubtotal)
{
    __subtotal = dSubtotal;
}

void Math::setChange(double change)
{
    __change = (-1)*__total_pvp;
}

//----------------------------------------------------
// Public

/**
 * @brief Math::doCalcs
 * @param qstrlitem
 */
void Math::doCalcs(QStringList qstrlitem)
{
    double dpvp = 0,
           dbi  = 0,
           diva = 0,
           dquanty = 0;
    int ret = 0;

    if(qstrlitem[6].contains(".") == true){/*Si contiene . lo quitamos*/
        qstrlitem[6].replace(".","");
        if(qstrlitem[6].length() == 2){
            qstrlitem[6].append("0");
        }
    }

    /*Transformamos del qstringlist a double los valores*/
    dbi = qstrlitem.at(6).toDouble();
    dquanty = qstrlitem.at(5).toDouble();
    diva = qstrlitem.at(7).toDouble();

    ret = __pluNegativo(qstrlitem);/*Plu Negativo*/
    qDebug() << "plu negativo: " << ret;
    if(ret < 0){/*Calculamos los montos*/
        dpvp = dquanty * dbi;
        dpvp = dpvp/100;
        __total_pvp -= dpvp;
        __total_iva -= (dpvp * diva)/10000;//deberia ser entre 100
        __total_bi -= dbi;

        if(__total_pvp < 0){
            __total_pvp = 0;
            qDebug("[ERROR][MATH][DOCALCS] el total de venta no puede ser negativo");
            /*deberia de retornar negativo*/
            return;
        }
    }
    else
    {
        dpvp = dquanty * dbi;
        dpvp = dpvp/100;
        __total_pvp += dpvp;
        __total_iva += (dpvp * diva)/10000;//deberia ser entre 100
        __total_bi += dbi;
    }
    __monto = dpvp;
    __bi = dbi/100;

    return;
}

/**
 * @brief Math::pluNegativo
 * @param qstrlplu
 * @return
 * Si el PLU es negativo se resta del total no se suma
 */


int Math::__pluNegativo(QStringList qstrlplu)
{
    int ret = 0;

   /*Si el plu es negativo retorna negativo*/
   if(qstrlplu.at(13).compare("Yes")==0)
   {
       return -1;
   }
   return ret;
}

/**
 * @brief Math::cleanMounts
 */
void Math::cleanMounts()
{
    __total_iva = 0;
    __total_pvp = 0;
    __total_bi = 0;
    __total_mii = 0;
    __change = 0;
    __rest = 0;
    __subtotal = 0;
    __canceled = 0;
}


/**
 * @brief Math::calcGarage
 * @param qstrlitem
 */
void Math::calcGarage(QStringList qstrlitem)
{
    /*la categoria esta ligada a la unidad*/
    /*vemos en la base de datos que unidad corresponde a esa categoria*/
    /*llevamos  la diferencia a una fraccion dada(la fraccion es la duracion de la unidad)*/

    double dpvp = 0,
           dbi  = 0,
           diva = 0,
           dquanty = 0;

    /*Transformamos del qstringlist a double los valores*/
    dbi = qstrlitem.at(6).toDouble();
    dquanty = qstrlitem.at(5).toDouble();
    diva = qstrlitem.at(7).toDouble();

    /*Calculamos los montos*/
    dpvp = dquanty * dbi;
    __total_pvp += dpvp;
    __total_iva += (dpvp * diva)/10000;
    __total_bi += dbi;
    return;
}

/**
 * @brief Math::discount_Discharge
 * Calcular descuentos y recargos
 */
void Math::discount_Discharge(int opc, double imount)
{
    switch (opc)
    {
    case 0://descuento
        __total_pvp -= imount;
        break;
    case 1://recargo
        __total_pvp += imount;
        break;
    default:
        break;
    }
}

/**
 * @brief Math::discountByanulItem
 * @param qstrlitem
 */
void Math::discountByanulItem(QStringList qstrlitem)
{
    double dpvp = 0,
           dbi  = 0,
           diva = 0,
           dquanty = 0;


    if(qstrlitem[6].contains(".")==true)/*Si contiene . lo quitamos*/
    {
        qstrlitem[6].replace(".","");
    }

    dbi = qstrlitem.at(6).toDouble();
    dbi = dbi / 100;
    qDebug("[DEBUG][MATH][discountByanulItem] qstrlitem[6] %s", qstrlitem[6].toLatin1().constData());
    dquanty = qstrlitem.at(5).toDouble();
    qDebug("[DEBUG][MATH][discountByanulItem] qstrlitem[5] %s", qstrlitem[5].toLatin1().constData());
    diva = qstrlitem.at(7).toDouble();
    qDebug("[DEBUG][MATH][discountByanulItem] qstrlitem[7] %s", qstrlitem[7].toLatin1().constData());

    dpvp = dquanty * dbi;
    __total_pvp -= dpvp;
    __total_iva -= (dpvp * diva)/10000;
    __total_bi -= dbi;

}

/**
 * @brief Math::Dicount
 * @param qstrlitem
 */
void Math::Dicount(QStringList qstrlitem)
{
    double dpvp = 0,
           dbi  = 0,
           diva = 0,
           dquanty = 0;

    /*Transformamos del qstringlist a double los valores*/
    dbi = qstrlitem.at(6).toDouble();
    dquanty = qstrlitem.at(5).toDouble();
    diva = qstrlitem.at(7).toDouble();

    /*Calculamos los montos*/
    dpvp = dquanty * dbi;
    __total_pvp -= dpvp;
    __total_iva -= (dpvp * diva)/100;
    __total_bi -= dbi;

    return;
}
