#include "proxysettings.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogaddnetwork.h"
#include "applicationsettingstab.h"
#include "dialogabout.h"
#include "dialogeditnetwork.h"
#include "dialogaddapp.h"
#include "genutils.h"

#include <qmessagebox.h>

#include <algorithm>
#include <vector>

using namespace utils;
using namespace std;

/*** Init ***/
MainWindow::MainWindow(QWidget *parent) 
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , isCurrentNetworkEdited(false)
{
    // Init data model
    DataModel::createInstance(this);
    connect(DataModel::getInstance(), SIGNAL(onLoadData()),
            this, SLOT(onLoad()));
    DataModel::getInstance()->loadData();
}

void MainWindow::onLoad()
{
    // design settings
    ui->setupUi(this);
    ui->horizontalLayoutHolder->setMargin(10);
    ui->centralwidgetHolder->setLayout(ui->horizontalLayoutHolder);
    // end design settings
    
    // front end connections
    connect(ui->pushButtonNetworkAdd, SIGNAL(clicked()), 
            this, SLOT(addNetwork()));
    connect(ui->actionAddNetwork, SIGNAL(triggered(bool)), 
            this, SLOT(addNetwork()));
    connect(ui->actionAbout, SIGNAL(triggered(bool)),
            this, SLOT(showAbout()));
    connect(ui->pushButtonNetworkRemove, SIGNAL(clicked()),
            this, SLOT(removeCurrentNetwork()));
    connect(ui->pushButtonNetworkSettings, SIGNAL(clicked()),
            this, SLOT(editCurrentNetworkSettings()));
    connect(ui->pushButtonSave, SIGNAL(clicked()),
            this, SLOT(updateCurrentNetwork()));
    connect(ui->pushButtonAddApp, SIGNAL(clicked()),
            this, SLOT(addApplication()));
    connect(ui->pushButtonRemoveApp, SIGNAL(clicked()),
            this, SLOT(removeApplication()));
    connect(ui->tabWidget, SIGNAL(currentChanged(int)),
            this, SLOT(onTabChange(int)));
    // end front end connection
    
    // data model connections
    disconnect(DataModel::getInstance(), SIGNAL(onLoadData()),
            this, SLOT(onLoad()));
    connect(DataModel::getInstance(), SIGNAL(onAddNetwork(QString)),
            this, SLOT(onAddNetwork(QString)));
    connect(DataModel::getInstance(), SIGNAL(onRemoveNetwork(QString)),
            this, SLOT(onRemoveNetwok(QString)));
    // end data model connection
    
    
    bindData();
    
    // select first network
    if(DataModel::getInstance()->getProxies().size() > 0) {
        ui->listWidgetNetworks->setCurrentRow(0);
    }
    
}

void MainWindow::closeEvent(QCloseEvent *){
    checkToSaveCurrentNetwork();
}

void MainWindow::bindData()
{
    QDataModel::proxyList::iterator it = 
            DataModel::getInstance()->getProxies().begin();
    
    while(it != DataModel::getInstance()->getProxies().end()){
        ui->listWidgetNetworks->addItem(QString(it->first.data()));
        it++;
    }
     
    connect(ui->listWidgetNetworks, SIGNAL(currentTextChanged(QString)),
            this, SLOT(changeCurrentNetwork(QString)));
    
}

/*** Data model reactions ***/
void MainWindow::onAddNetwork(QString const & title)
{    
    ui->listWidgetNetworks->addItem(title);
}

void MainWindow::onCurrentNetworkEdited()
{
    isCurrentNetworkEdited = true;
    this->ui->pushButtonSave->setEnabled(true);
}

void MainWindow::onRemoveNetwok(const QString &title)
{
     delete ui->listWidgetNetworks->findItems(title, 
                                              Qt::MatchFixedString).first();
}

void MainWindow::onUpdateNetwork(const QString&){}

void MainWindow::onAddPlugin(const QString & app){
    
    this->onCurrentNetworkEdited();
    
    ui->tabWidget->addTab(
                new ApplicationSettingsTab(
                    this,
                    ui->tabWidget, 
                    (*currentProxySettings)[app.toStdString()],
                    DataModel::getInstance()->loadPluginSettings(app)),
                app);
    checkAddAppPosibility();
}

/*** Front end slots ***/

void MainWindow::addNetwork()
{
    (new DialogAddNetwork(this))->show();
}

void MainWindow::restoreCurrentNetwork()
{
    DataModel::getInstance()->restoreNetwork(
                currentNetworkName.toStdString());
}

void MainWindow::updateCurrentNetwork()
{
    for(int i = 0; i < ui->tabWidget->count(); i++) {
        ApplicationSettingsTab* tab = static_cast<ApplicationSettingsTab*>(
                    ui->tabWidget->widget(i));
        tab->saveChanges();
    }
    
    DataModel::getInstance()->updateNetwork(currentNetworkName);
    isCurrentNetworkEdited = false;
    ui->pushButtonSave->setEnabled(false);
}

void MainWindow::removeCurrentNetwork()
{
    QMessageBox mb(this);
    
    mb.setWindowTitle("Removing approval");
    mb.setText("Are you sure you want to \nremove network \"" 
               + currentNetworkName + "\"?");
    mb.addButton(QMessageBox::Yes);
    mb.addButton(QMessageBox::No);
    
    if(mb.exec() != QMessageBox::Yes)
        return;
    
    isCurrentNetworkEdited = false;
    DataModel::getInstance()->removeNetwork(currentNetworkName);
    
    
}

void MainWindow::addApplication()
{
    DialogAddApp d(this, *currentProxySettings);
    d.exec();
}

void MainWindow::editCurrentNetworkSettings()
{
    DialogEditNetwork d(this, currentNetworkName);
    d.exec();
}

void MainWindow::removeApplication() 
{
    int i = ui->tabWidget->currentIndex();
    string s = ui->tabWidget->tabText(i).toStdString();
    currentProxySettings->remove(s);
    ui->tabWidget->removeTab(i);
    onCurrentNetworkEdited();
    ui->pushButtonAddApp->setEnabled(true);
}

void MainWindow::onTabChange(int)
{
    ui->pushButtonRemoveApp->setEnabled(ui->tabWidget->count() > 0);
}

void MainWindow::checkAddAppPosibility()
{
    if((DataModel::getInstance()->getApps().size()) 
            == ui->tabWidget->count()) {
        ui->pushButtonAddApp->setEnabled(false);
    }
}

void MainWindow::checkToSaveCurrentNetwork(){
    
    if(isCurrentNetworkEdited) {
        QMessageBox mb(this);
        
        mb.setWindowTitle("Saving approval");
        mb.setText("\"" + currentNetworkName + "\" has been edited.\n"
                        + "Do you want to save changes?");
        mb.addButton(QMessageBox::Yes);
        mb.addButton(QMessageBox::No);
        
        if(mb.exec() == QMessageBox::Yes)
            updateCurrentNetwork();
        else
            restoreCurrentNetwork();
        
    }
    
}

void MainWindow::changeCurrentNetwork(QString const & title)
{
    
    if(title.isEmpty()) {
        currentNetworkName = "";
        ui->pushButtonNetworkRemove->setEnabled(false);
        ui->pushButtonNetworkSettings->setEnabled(false);
        ui->pushButtonRemoveApp->setEnabled(false);
        ui->pushButtonAddApp->setEnabled(false);
        ui->pushButtonSave->setEnabled(false);
        ui->tabWidget->clear();
        return;
    }
    
    checkToSaveCurrentNetwork();
                
    currentProxySettings = &DataModel::getInstance()->getProxies()
                            .find(title.toStdString())->second;
    
    ui->tabWidget->clear();
    ProxySettings::iterator it = currentProxySettings->begin();
    
    while(it != currentProxySettings->end()) {
        QString qname (it->first.data());
        ui->tabWidget->addTab(
                    new ApplicationSettingsTab(
                        this,
                        ui->tabWidget, 
                        it->second,
                        DataModel::getInstance()->loadPluginSettings(qname)), 
                    qname);
        it++;
    }
        
    ui->pushButtonNetworkRemove->setEnabled(true);
    ui->pushButtonNetworkSettings->setEnabled(true);
    ui->pushButtonAddApp->setEnabled(true);
    ui->pushButtonSave->setEnabled(false);
    isCurrentNetworkEdited = false;
    
    currentNetworkName = ui->listWidgetNetworks->currentItem()->text();
    checkAddAppPosibility();
    
}

void MainWindow::showAbout()
{
    DialogAbout d;
    d.exec();
}

/*** Other ***/
MainWindow::~MainWindow()
{
    delete ui;
}

