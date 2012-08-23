/*
         addAction(QString name, QString id, QString shortcut, QString iconName, QString slot)
           |
           |     +---------+
           +---->|Action   |_______
           +-----|Manager  |       |
           |     +---------+       +--->QList<ActionStuff *> getActions()
           |
          +-->QAction * getAction(QString id);
*/

#include <QtCore/QDebug>
#include <QtGui/QShortcut>
#include <QtGui/QAction>
#include <QtGui/QWidget>
#include "actionsmanager.h"
#include "actiondata.h"

ActionsManager::ActionsManager(QObject *parent) :
    QObject(parent)
{
    qDebug()<<"new action manager object";
}

bool ActionsManager::addAction(QAction *action, QString id, QWidget *parent, QObject *receiver, std::string slot, QKeySequence shortcut)
{
    bool b = addAction(action, id, parent, shortcut);
    this->connectAction(id, receiver, slot);
    return b;
}

void ActionsManager::setActionIcon(QString id, QString iconName)
{
    if(ActionsList.contains(id)){
        ActionsList[id].action->setIcon(QIcon::fromTheme(iconName));
        ActionsList[id].iconName = iconName;
    }
}

bool ActionsManager::addAction(QAction *action, QString id, QWidget *parent, QKeySequence key)
{
    //check if already have the action added
    if(ActionsList.contains(id)){
        return false;
    }

    /*
      create a new keysequence to use as shortcut. first, if it's posible take the
      one the action owns, or use the one provided by key
      */
    QKeySequence k;
    if(!action->shortcut().isEmpty()){
        //set the action's shortcut as the key
        k = action->shortcut();

        //and then remove that shortcut from the action
        action->setShortcut(QKeySequence());
    }
    else{
        //sets key as k
        k = key;
    }

    //make the action looks like it had a shortcut
    QString name = action->text();
    if(!k.isEmpty()){
        name.append("\t" + k.toString());
    }

    //create a new InternalActData struct to add to ActionsList
    InternalActData IAD;
    IAD.action = action;
    IAD.action->setText(name);
    IAD.shortcut = new QShortcut(k, parent);

    /*configure tooltip in case the action doesn't have one setted
    this is becouse if the action doesn't have one, then it shows the action name
    and as "Name    shortcut", and that's not pretty*/
    QString g = this->removeSString(name).remove('&');
    if(action->toolTip().indexOf(g) != -1){
        action->setToolTip(g);
    }

    //check if the action is enabled or not
    IAD.shortcut->setEnabled(IAD.action->isEnabled());

    ActionsList[id] = IAD;

    return true;
}

void ActionsManager::setEnabled(QString id, bool enabled)
{
    if(ActionsList.contains(id)){
        ActionsList[id].action->setEnabled(enabled);
        ActionsList[id].shortcut->setEnabled(enabled);
    }
    else{
        qDebug()<<"Wrong id:"<<id;
    }
}

QStringList ActionsManager::getIds()
{
    QStringList list;
    QMapIterator<QString, InternalActData> iterator(ActionsList);
    while (iterator.hasNext()) {
        iterator.next();
        list<<QString(iterator.key());
    }
    return list;
}

QList<ActionData *> ActionsManager::getActions()
{
    QList<ActionData *> list;
    QString name, iconName, shortcut, id;

    QMapIterator<QString, InternalActData> iterator(ActionsList);
    while (iterator.hasNext()) {
        iterator.next();

        //id
        id = iterator.key();//key, a.k.a id

        //name
        name = removeSString(iterator.value().action->text()).replace("&", ""); //action name

        //icon name
        iconName = iterator.value().iconName;//icon name
        if(iconName.isEmpty()){
            iconName = "none";
        }

        //shortcut
        shortcut = iterator.value().shortcut->key().toString();//shortcut
        if(shortcut.isEmpty()){
            shortcut = "none";
        }

        //add the object to the list
        list<<new ActionData(name, id, shortcut, iconName);
    }

    return list;
}

QString ActionsManager::removeSString(QString str)
{
    int k = str.indexOf("\t");
    if(k != -1){
        return str.left(k);
    }
    else{
        return str;
    }
}

QAction *ActionsManager::getAction(QString id)
{
    if(ActionsList.contains(id)){
        return ActionsList[id].action;
    }
    else{
        return 0;
    }
}

void ActionsManager::connectAction(QString id, QObject *receiver, std::string slot)
{
    if(!ActionsList.contains(id)){
        qDebug()<<"no action with id:"<<id;
        return;
    }
    else if(!ActionsList[id].action->isCheckable()){
        connect(ActionsList[id].shortcut, SIGNAL(activated()), receiver, slot.data());
        connect(ActionsList[id].action, SIGNAL(triggered()), receiver, slot.data());
    }
    else{
        connect(ActionsList[id].action, SIGNAL(toggled(bool)), receiver, slot.data());
        connect(ActionsList[id].shortcut, SIGNAL(activated()), ActionsList[id].action, SLOT(toggle()));
    }
}

void ActionsManager::showData(QString n, QString id, QString in, QString s, bool e, bool c, bool cd)
{
    //something to show
    qDebug()<<"+-----------------------------------------------+";
    qDebug()<<"|action name:"<<n;
    qDebug()<<"|action id:"<<id;
    qDebug()<<"|action iconName:"<<in;
    qDebug()<<"|action shortcut:"<<s;
    qDebug()<<"|enabled:"<<e;
    qDebug()<<"|checkable:"<<c;
    qDebug()<<"|ckecked:"<<cd;
    qDebug()<<"+-----------------------------------------------+";
}
