#ifndef IGUI_H
#define IGUI_H
#include <QString>

class IGui
{
public:
    IGui(){}
    virtual void updateWidgets(void *sender)=0;
    virtual void writeOnWidget(QString qstr)=0;
    virtual void backSpaceLineEdit()=0;
    virtual int  getFromWidgets(QString &qstr)=0;
    virtual int  getFromWidgets(QStringList &qstrl)=0;
    virtual int  focusUp()=0;
    virtual int  focusDown()=0;
    virtual void clear()=0;
    virtual void release()=0;
    virtual void showWidget()=0;
    virtual void hideWidget()=0;
    virtual bool isVisibleWidget()=0;
    virtual int  isFocus()=0;
    virtual int  modifyLayout(int nOption)=0;
};
#endif // IGUI_H
