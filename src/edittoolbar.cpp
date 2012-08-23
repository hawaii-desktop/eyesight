#include <QtCore/QDebug>
#include "edittoolbar.h"
#include "toolbaredit.h"

EditToolBar::EditToolBar(QWidget *parent) :
    QDialog(parent)
{
    ui.setupUi(this);
    ui.okButton->setIcon(QIcon::fromTheme("dialog-ok"));
    ui.applyButton->setIcon(QIcon::fromTheme("dialog-ok-apply"));
    ui.cancelButton->setIcon(QIcon::fromTheme("dialog-cancel"));

    tbe = new ToolBarEdit(this);
    tbe->setMaximumWidth(ui.widget->maximumWidth());
    tbe->setMaximumHeight(ui.widget->maximumHeight());

    connect(tbe, SIGNAL(actionsListChanged()), this, SLOT(actionsChangedSlot()));
    connect(ui.okButton, SIGNAL(clicked()), this, SLOT(okSlot()));
    connect(ui.applyButton, SIGNAL(clicked()), this, SLOT(applySlot()));

    ui.widget = tbe;

    this->setWindowTitle(tr("Configure toolbar"));
}

void EditToolBar::setActionsList(QList<ActionData *> actions, QStringList lActions)
{
    tbe->setActionsList(actions, lActions);
}

void EditToolBar::actionsChangedSlot()
{
    ui.applyButton->setEnabled(true);
    ui.okButton->setEnabled(true);
}

void EditToolBar::applySlot()
{
    qDebug()<<"apply slot";
    emit actionsChanged(tbe->getActionsList());
}

void EditToolBar::okSlot()
{
    qDebug()<<"ok slot";
    emit actionsChanged(tbe->getActionsList());
    this->close();
}

