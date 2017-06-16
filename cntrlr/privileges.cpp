#include "privileges.h"

Privileges::Privileges()
{

}
/**
 * @brief Privileges::privileges_Null
 * @param c
 * @return
 * Mozos
 */
bool Privileges::privileges_Null(Cashier *c)
{
    if(c->getNlevel() >= 0){
        return true;
    }
    return false;
}

/**
 * @brief Privileges::privileges_Low
 * @param c
 * @return
 * Solo puede realizar:
 * ventas
 * registro de clientes
 */
bool Privileges::privileges_Low(Cashier *c)
{
    if(c->getNlevel() >= 1){
        return true;
    }
    return false;
}

/**
 * @brief Privileges::privileges_Midle
 * @param c
 * @return
 * Solo puede realizar:
 * ventas
 * registro de clientes
 * anulaciones
 */
bool Privileges::privileges_Midle(Cashier *c)
{
    if(c->getNlevel() >= 2){
        return true;
    }
    return false;
}

/**
 * @brief Privileges::privileges_High
 * @param c
 * @return
 * Puede realizar:
 * ventas
 * registro de clientes
 * anulaciones
 * NDC
 * configuraciones
 * reportes
 */
bool Privileges::privileges_High(Cashier *c)
{
    if(c->getNlevel() >= 3){
        return true;
    }
    return false;
}

/**
 * @brief Privileges::privileges_SU
 * @param c
 * @return
 * Acceso a todo el sistema
 */
bool Privileges::privileges_SU(Cashier *c)
{
    if(c->getNlevel() == 4){
        return true;
    }
    return false;
}

