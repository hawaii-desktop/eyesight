#ifndef ACTIONSMANAGER_H
#define ACTIONSMANAGER_H

#include <QObject>
#include <QtCore/QStringList>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtGui/QKeySequence>

/*
         addAction(QString name, QString id, QString shortcut, QString iconName, std:string slot, bool enabled)
           |
           |     +---------+
           +---->|Action   |_______
           +-----|Manager  |       |
           |     +---------+       +--->QList<ActionStuff *> getActions()
           |
           +-->QAction * getAction(QString id)
*/

class QAction;
class QWidget;
class QShortcut;
class ActionData;

class ActionsManager : public QObject
{
    /*!
      \brief This class is used to manage all the action in the app.

      Start by creating a new object, then add an action(s), with it's respective
      name, id, icon name and shortcut, and the slot it has to be connected
      Then, if you want to add an action to some widget, just do it by:
      widget->addAction(actManager->getAction(id))
      */

    Q_OBJECT

public:
    /*!
      constructor
      */
    ActionsManager(QObject *parent);

    /*!
      \returns a lsit with the all the actions, in a ActionData struct
      you can use it for the toolbar editor
      */
    QList<ActionData *> getActions();

    /*!
      Adds a new action to the manager
      \param id is the identification for the action, i.e. _open
      if the action already has a shortcut, then \a shortcut is ignored.
      \param shortcut is the action shortcut, i.e. Ctrl+O
      \returns true if succed, otherwise \returns false. If the object already
      had an action with the \a id, then it doesn't create the action
     */
    bool addAction(QAction *action, QString id, QWidget *parent, QKeySequence shortcut = QKeySequence());

    bool addAction(QAction *action,
                   QString id,
                   QWidget *parent,
                   QObject *receiver,
                   std::string slot,
                   QKeySequence shortcut = QKeySequence());

    /*!
      \a returns the action attached to \a id, otherwise \return a null action
      */
    QAction *getAction(QString id);

    /*!
      connects the action asigned to \a id, to the receiver \a receiver to the
      slot \a slot. Note: you get the slot doing SLOT(yourSlot())
      */
    void connectAction(QString id, QObject *receiver, std::string slot);

    /*!
      enable disable the wanted action
      */
    void setEnabled(QString id, bool enabled);

    /*!
      \returns a list with all the id's stored
      */
    QStringList getIds();

    /*!
      set the \a iconName icon from the theme to the action \a id
      */
    void setActionIcon(QString id, QString iconName);

    //signals:

    //public slots:

private:
    /*!
      The name of every action created will have the next format:
      \a name + \t + \a shortcut, i.e. Open\tCtrl+O
      The string \t is used to add the shortcut string in the action
      name to make it look as the action shortcut, but it's not.
      */

    /*!
      \struct this struct contains an action, it's shortcut
      */
    struct InternalActData {
        QAction *action;    ///<action
        QShortcut *shortcut;///<actions's shortcut
        QString iconName;
    };

    /*!
      this QMap object contains a list with the \a actions and it's respective
      \a id's.
      */
    QMap<QString, InternalActData> ActionsList;

    QString removeSString(QString str);

    void showData(QString n, QString id, QString in, QString s, bool e, bool c, bool cd);
};

#endif // ACTIONSMANAGER_H
