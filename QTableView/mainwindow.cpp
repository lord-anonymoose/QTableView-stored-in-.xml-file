#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QString>
#include <QList>
#include <QtXml>
#include <QFileDialog>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QFile>
#include <QXmlStreamAttribute>
#include <QFileDialog>


QFile file("myDataFile.xml");
bool isEditing = false; //Таблица редактируется
bool ErrorFound = false; //Найдена ошибка
int currentRow; //Редактируемая ячейка
QList<clients> clientsList; //Контейнер для денег

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lineEdit_3->setInputMask("7""9999999999");
    ui->lineEdit_2->setReadOnly(true);
       QStringList horizontalHeader;
       horizontalHeader.append("ФИО/название организации");
       horizontalHeader.append("ИНН");
       horizontalHeader.append("Телефон");
       horizontalHeader.append("Адрес");
       model->setHorizontalHeaderLabels(horizontalHeader);
           ui->tableView->resizeRowsToContents();
           ui->tableView->resizeColumnsToContents();
          clientsList = readFromFile();
          listToTable(clientsList);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Клик по таблице
void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    currentRow = index.row();
    ui->lineEdit->setText(clientsList.at(currentRow).name);
    ui->lineEdit_2->setText(clientsList.at(currentRow).taxID);
    ui->lineEdit_3->setText(clientsList.at(currentRow).phone);
    ui->lineEdit_4->setText(clientsList.at(currentRow).address);
    isEditing = true;
    update();
}

//Кнопка сохранить
void MainWindow::on_pushButton_clicked()
{
    ErrorFound = false;
    if (ui->lineEdit->text() == "") {QMessageBox::warning(this, tr("Ошибка"),tr ("Пустое поле: 'ФИО/Наименование организации'")); ErrorFound = true;};
    if (ui->lineEdit_2->text() == "") {QMessageBox::warning(this, tr("Ошибка"),tr ("Пустое поле: 'ИНН'")); ErrorFound = true;};
    if (ui->lineEdit_3->text() == "7") {QMessageBox::warning(this, tr("Ошибка"),tr ("Не заполнено поле: 'Номер телефона'")); ErrorFound = true;};
    if (ui->lineEdit_4->text() == "") {QMessageBox::warning(this, tr("Ошибка"),tr ("Пустое поле: 'Адрес'")); ErrorFound = true;};
    if (ErrorFound == false)
    {
        if (isEditing == false)
        {
            clients tempClient;
            tempClient.name = ui->lineEdit->text();
            tempClient.taxID = ui->lineEdit_2->text();
            tempClient.phone = ui->lineEdit_3->text();
            tempClient.address = ui->lineEdit_4->text();
            clientsList.append(tempClient);
            listToTable((clientsList));
            clrEdits();
        }

        if (isEditing == true)
        {
            clientsList[currentRow].name = ui->lineEdit->text();
            clientsList[currentRow].taxID = ui->lineEdit_2->text();
            clientsList[currentRow].phone = ui->lineEdit_3->text();
            clientsList[currentRow].address = ui->lineEdit_4->text();
            item = new QStandardItem(QString(clientsList[currentRow].name));
                model->setItem(currentRow, 0, item);
            item = new QStandardItem(QString(clientsList[currentRow].taxID));
                model->setItem(currentRow, 1, item);
            item = new QStandardItem(QString(clientsList[currentRow].phone ));
                model->setItem(currentRow, 2, item);
            item = new QStandardItem(QString(clientsList[currentRow].address));
                model->setItem(currentRow, 3, item);
            clrEdits();
            isEditing = false;
        }
    }
    saveToFile(clientsList);
}

//Кнопка удалить
void MainWindow::on_pushButton_2_clicked()
{
    if (isEditing == true)
    {
        clrEdits();
        clientsList.removeAt(currentRow);
        listToTable((clientsList));
        model->removeRow(model->rowCount()-1);
        update();
        isEditing = false;
        saveToFile(clientsList);

    }
}

//Кнопка добавить
void MainWindow::on_pushButton_3_clicked()
{
    isEditing = false;
    update();
}

//Если мы вносим физ. лицо
void MainWindow::on_radioButton_clicked()
{
    ui->lineEdit_2->setReadOnly(false);
    ui->lineEdit_2->setInputMask("999999999999");
}

//Если мы вносим юр. лицо
void MainWindow::on_radioButton_2_clicked()
{
    ui->lineEdit_2->setReadOnly(false);
    ui->lineEdit_2->setInputMask("9999999999");
}



