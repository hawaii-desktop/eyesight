#ifndef EDITTOOLBAR_H
#define EDITTOOLBAR_H

#include <QDialog>
#include <QList>
#include "actiondata.h"
#include "ui_edittoolbar.h"

class ToolBarEdit;

class EditToolBar : public QDialog
{
    Q_OBJECT
public:
    EditToolBar(QWidget *parent = 0);
    void setActionsList(QList<ActionData *>, QStringList);

signals:
    void actionsChanged(QStringList);

public slots:

private:
    Ui_Dialog ui;
    ToolBarEdit *tbe;

private slots:
    void applySlot();
    void okSlot();
    void actionsChangedSlot();

};

#endif // EDITTOOLBAR_H
