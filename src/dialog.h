#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QProcess>
#include <QSystemTrayIcon>
#include <QAction>
#include <QMenu>
#include <QIcon>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();


private slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);

protected:
    void closeEvent(QCloseEvent *);
    void hideEvent(QHideEvent *) ;

private:
    Ui::Dialog *ui;

private:
    QProcess m_process;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    QAction *minAction;
    QAction *maxAction;
    QAction *quitAction;

private:
    void proxyAuto(bool checked);
    void proxyManual(bool checked);
    void startProxy(bool start);

};
#endif // DIALOG_H
