#include "dialog.h"
#include "ui_dialog.h"
#include "switchcontrol.h"

#include <QApplication>
#include <QCloseEvent>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog),
    trayIcon(nullptr),
    trayIconMenu(nullptr),
    minAction(nullptr),
    maxAction(nullptr),
    quitAction(nullptr)
{
    ui->setupUi(this);

    trayIcon = new QSystemTrayIcon(this);
    connect(trayIcon, &QSystemTrayIcon::activated, this, &Dialog::iconActivated);

    minAction = new QAction(tr("Hide"), this);
    connect(minAction, &QAction::triggered, this, &Dialog::hide);

    maxAction = new QAction(tr("Show"), this);
    connect(maxAction, &QAction::triggered, this, &Dialog::showNormal);

    quitAction = new QAction(tr("Quit"), this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minAction);
    trayIconMenu->addAction(maxAction);
    trayIconMenu->addSeparator();

    trayIconMenu->addAction(quitAction);

    QIcon icon = QIcon(":/res/freenet.png");
    trayIcon->setIcon(icon);
    setWindowIcon(icon);

    trayIcon->setToolTip("Free Net");
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->show();

    connect(ui->radioButtonAuto, &QRadioButton::clicked, this, &Dialog::proxyAuto);
    connect(ui->radioButtonManual, &QRadioButton::clicked, this, &Dialog::proxyManual);
    connect(&m_process, &QProcess::readyReadStandardOutput, this, [=](){
        ui->labelInfo->setText(m_process.readAll());
        ui->radioButtonManual->click();
    });

    SwitchControl *pSwitchControl = new SwitchControl(ui->labelSwitch);


    // 设置状态、样式
    pSwitchControl->setToggle(true);
    pSwitchControl->setCheckedColor(QColor(0, 160, 230));


    // 连接信号槽
    connect(pSwitchControl, &SwitchControl::toggled, this, &Dialog::startProxy);

    startProxy(true);
}

Dialog::~Dialog()
{
    startProxy(false);
    delete ui;
}

void Dialog::startProxy(bool start)
{
    if(start){
        QString freeNet = QString("%1/%2").arg(qApp->applicationDirPath()).arg("freenet");
        m_process.start(freeNet);
    }
    else{
        m_process.kill();
        m_process.close();
        ui->radioButtonAuto->click();
        QProcess::execute("gsettings set org.gnome.system.proxy mode 'auto'");
    }
}

/*
 *
 * gsettings set org.gnome.system.proxy mode 'manual'
gsettings set org.gnome.system.proxy.https host '127.0.0.1'
gsettings set org.gnome.system.proxy.https port 9666
gsettings set org.gnome.system.proxy.ftp host '127.0.0.1'
gsettings set org.gnome.system.proxy.ftp port 9666
gsettings set org.gnome.system.proxy mode 'auto'
 */
void Dialog::proxyAuto(bool checked)
{
    if(checked){
        QStringList args;
        args << "set" << "org.gnome.system.proxy" << "mode" << "'auto'";
        QProcess::execute("gsettings", args);
    }
}

void Dialog::proxyManual(bool checked)
{
    if(checked){
        QProcess::execute("gsettings set org.gnome.system.proxy mode 'manual'");
        QProcess::execute("gsettings set org.gnome.system.proxy.https host '127.0.0.1'");
        QProcess::execute("gsettings set org.gnome.system.proxy.https port 9666");
        QProcess::execute("gsettings set org.gnome.system.proxy.http host '127.0.0.1'");
        QProcess::execute("gsettings set org.gnome.system.proxy.http port 9666");
        QProcess::execute("gsettings set org.gnome.system.proxy.ftp host '127.0.0.1'");
        QProcess::execute("gsettings set org.gnome.system.proxy.ftp port 9666");
    }
}


void Dialog::closeEvent(QCloseEvent *e)
{
    hide();
    e->ignore();
}

void Dialog::hideEvent(QHideEvent *e)
{
    hide();
    e->ignore();
}


void Dialog::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason) {
    case QSystemTrayIcon::DoubleClick:
        break;
    case QSystemTrayIcon::Trigger:{
        if(isHidden()){
            showMaximized();
        }
        else{
            hide();
        }
        break;
    }
    default:
        break;
    }
}

