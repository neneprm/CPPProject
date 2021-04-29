#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "payer.h"
#include "listaddingdialog.h"
#include <QMainWindow>
#include <QLabel>
#include <QGridLayout>
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_nameAdd_button_clicked();

    void on_listAdd_button_clicked();

    void on_clearAllName_button_clicked();

private:
    int numPayer{0};
    Ui::MainWindow *ui;
    ListAddingDialog *listAddingDialog;
    QLabel *listNameInput_label;
    QGridLayout *payerGridLayout;
    QVector<Payer*>payerList;

    void clearLayout(QLayout *layout);

};
#endif // MAINWINDOW_H
