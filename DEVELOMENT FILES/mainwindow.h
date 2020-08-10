#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include "mysquer.h"
#include <QVector>
#include <vector>
#include <iostream>
#include <string>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    MySquer *squer;

private slots:
    void on_pushButton_clicked();
    void on_pushButton_add_clicked();
    void on_lineEdit_memSize_editingFinished();
    void on_pushButton_Addhole_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_dealloc_process_clicked();
    void on_pushButton_addPocess_clicked();
    void on_comboBox_process_currentTextChanged(const QString &arg1);
    void on_treeWidget_doubleClicked(const QModelIndex &index);
};
#endif // MAINWINDOW_H
