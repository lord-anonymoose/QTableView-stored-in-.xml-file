#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMainWindow>
#include "QStandardItemModel"
#include "QStandardItem"
#include <QtXml>
#include <QMessageBox>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QXmlStreamAttribute>

class clients
{
public:
    QString name;
    QString taxID;
    QString phone;
    QString address;
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QStandardItemModel *model = new QStandardItemModel;
       QStandardItem *item;

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_radioButton_clicked();
    void on_radioButton_2_clicked();
    void on_tableView_clicked(const QModelIndex &index);

    void clrEdits () {
        ui->lineEdit->setText("");
        ui->lineEdit_2->setText("");
        ui->lineEdit_3->setText("7");
        ui->lineEdit_4->setText("");
    }

    //Вывод данных из QList в TableView
    void listToTable(QList<clients> list)
    {
        int i = 0;
        while (i < list.size())
        {
            item = new QStandardItem(QString(list.at(i).name));
                model->setItem(i, 0, item);
             item = new QStandardItem(QString(list.at(i).taxID));
                model->setItem(i, 1, item);
             item = new QStandardItem(QString(list.at(i).phone));
                model->setItem(i, 2, item);
             item = new QStandardItem(QString(list.at(i).address));
                model->setItem(i, 3, item);
            ui->tableView->setModel(model);
             ++i;
        }
    }

    //Сохранение данных в файл
    void saveToFile(QList<clients> list)
    {
        QFile file("./myDataFile.xml");
        if (!file.open(QIODevice::WriteOnly))
            {
                QMessageBox::warning(this,
                                     "Ошибка файла",
                                     "Не удалось открыть файл",
                                     QMessageBox::Ok);
                return;
            } else {
        QXmlStreamWriter xmlWriter(&file);
        xmlWriter.setAutoFormatting(true);
        xmlWriter.writeStartElement("Clients");
        int i = 0;
        while (i < list.size())
        {
            xmlWriter.writeStartElement("name");
                xmlWriter.writeAttribute("QString", list.at(i).name);
                    xmlWriter.writeEndElement();
            xmlWriter.writeStartElement("taxID");
                xmlWriter.writeAttribute("QString", list.at(i).taxID);
                    xmlWriter.writeEndElement();
            xmlWriter.writeStartElement("phone");
                xmlWriter.writeAttribute("QString", list.at(i).phone);
                    xmlWriter.writeEndElement();
            xmlWriter.writeStartElement("address");
                xmlWriter.writeAttribute("QString", list.at(i).address);
                    xmlWriter.writeEndElement();
                    i++;
        }
        xmlWriter.writeEndElement();
        xmlWriter.writeEndDocument();
        file.close();
        }
    }

    //Чтение данных из файла
    QList<clients> readFromFile()
    {
        QList<clients> list;
        clients client;
        QFile file("./myDataFile.xml");
        if (!file.open(QFile::ReadOnly | QFile::Text))
        {
            QMessageBox::warning(this,
                                 "Ошибка файла",
                                 "Не удалось открыть файл",
                                 QMessageBox::Ok);
        } else {
            QXmlStreamReader xmlReader;
            xmlReader.setDevice(&file);
            xmlReader.readNext();
            while(!xmlReader.atEnd())
            {
                if (xmlReader.isStartElement())
                {
                    if (xmlReader.name() == "name")
                    {
                        foreach(const QXmlStreamAttribute &attr, xmlReader.attributes())
                        {
                            client.name = attr.value().toString();
                        }
                    }
                    if (xmlReader.name() == "taxID")
                    {
                        foreach(const QXmlStreamAttribute &attr, xmlReader.attributes())
                        {
                            client.taxID = attr.value().toString();
                        }
                    }
                    if (xmlReader.name() == "phone")
                    {
                        foreach(const QXmlStreamAttribute &attr, xmlReader.attributes())
                        {
                            client.phone = attr.value().toString();
                        }
                    }
                    if (xmlReader.name() == "address")
                    {
                        foreach(const QXmlStreamAttribute &attr, xmlReader.attributes())
                        {
                            client.address = attr.value().toString();
                            list.append(client);
                        }
                    }
                }
                xmlReader.readNext();
            }
            file.close();
        }
        return list;
    }


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
