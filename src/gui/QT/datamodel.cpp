#include "datamodel.h"

using namespace utils;
using namespace std;

QDataModel* DataModel::instance;

void QDataModel::loadData()
{
    ProxySettings ps1;
    ps1.loadData("../res/HomeNetwork.ini");
    proxySettings.push_back(ps1);
    
    ProxySettings ps2;
    ps2.loadData("../res/AptuNetwork.ini");
    proxySettings.push_back(ps2);
    
    emit onLoadData();
    
}

void QDataModel::addAppSettings(QString const &)
{
    AppSettings* ap = new AppSettings();
    emit onAddAppSettings(*ap);
}

