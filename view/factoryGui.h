#ifndef FACTORYGUI_H
#define FACTORYGUI_H

#include "view/IGui.h"
#include "view/customer.h"
#include "view/drawer.h"
#include "view/index.h"
#include "view/logusr.h"
#include "view/pointofsale.h"
#include "view/showplu.h"

class FactoryGui
{
public:
    IGui* createCustomer()
    {
        return new Customer;
    }
    IGui* createDrawer()
    {
        return new Drawer;
    }
    IGui* createIndex()
    {
        return new Index;
    }
    IGui* createLogusr()
    {
        return new LogUsr;
    }
    IGui* createPos()
    {
        return new PointOfSale;
    }
    IGui* createShowPlu()
    {
        return new ShowPlu;
    }
};
#endif // FACTORYGUI_H
