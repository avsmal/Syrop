#include "dialogaddapp.h"
#include "ui_dialogaddapp.h"
#include "datamodel.h"
#include "proxysettings.h"
#include "fstream"

using namespace utils;
using namespace std;

DialogAddApp::DialogAddApp(MainWindow* mainWindow, utils::ProxySettings& proxySettings) 
    : QDialog(mainWindow)
    , mainWindow(mainWindow)
    , proxySettings(proxySettings)
    , ui(new Ui::DialogAddApp)
{
    ui->setupUi(this);
    
    utils::plugins_t::const_iterator it = DataModel::getInstance()->getApps().begin();
    utils::plugins_t::const_iterator end = DataModel::getInstance()->getApps().end();
    
    while(it != end) {
        if(!proxySettings.exists(it->first))
            ui->comboBox->addItem(QString(it->first.data()));
        it++;
    }
    
    connect(ui->buttonBox, SIGNAL(accepted()),
            this, SLOT(onChoose()));   
}


void DialogAddApp::onChoose()
{
    std::string app = ui->comboBox->currentText().toStdString();                                
    proxySettings.add(app, utils::attributes());
    mainWindow->onAddPlugin(ui->comboBox->currentText());
}

DialogAddApp::~DialogAddApp()
{
    delete ui;
}
