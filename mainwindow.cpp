#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlDatabase>
#include <QDebug>
#include <QString>
#include <QSqlError>
#include <QSqlRecord>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QSqlQuery>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //set the pushButtonInfo on the red cross statut and hide every layout but the one that let the user type a server
    ui->pushButtonInfo->setStyleSheet("background-image:url(:/ressource/cross);");
    ui->LayoutBaseReq->hide();
    ui->LayoutResultat->hide();
    ui->LayoutConnection->hide();
    this->adjustSize();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButtonValidateServer_clicked()
{
    QString server = ui->lineEditServer->text();
    maBase = QSqlDatabase::addDatabase("QMYSQL");
    maBase.setHostName(server);
    ui->LayoutServer->hide();
    ui->LayoutConnection->show();
    this->adjustSize();
}

void MainWindow::on_pushButtonValider_clicked()
{

    QString login = ui->lineEditLogin->text();
    QString password = ui->lineEditPassword->text();

    qDebug()<<"valider";
    maBase.setUserName(login);
    maBase.setPassword(password);
    maBase.open();

    if(maBase.open())
    {
        qDebug()<<"connecté";
        ui->LayoutConnection->hide();
        ui->LayoutBaseReq->show();
        this->adjustSize();
        ui->comboBoxDbList->addItem("choisir une base");

        QSqlQuery getBases("show databases;");
        while(getBases.next())
            {
                QString base = getBases.value(0).toString();
                qDebug()<<base;
                ui->comboBoxDbList->addItem(base);
            }
    }else
    {
        qDebug()<<"l'ouverture de la base a échoué !";
    }
}


void MainWindow::on_comboBoxDbList_activated(const QString &arg1)
{
    qDebug()<<"text changed";
    qDebug()<<arg1;
    if (arg1=="choisir une base"){

    }else
    {
        maBase.setDatabaseName(arg1);

        maBase.open();
        this->setWindowTitle(arg1);


        QSqlQuery getTables("show tables;");
        while(getTables.next())
            {
                QString table = getTables.value(0).toString();
                qDebug()<<table;
                ui->listWidgetTables->addItem(table);
            }
    }
}


void MainWindow::on_listWidgetTables_itemClicked(QListWidgetItem *item)
{
    maBase.open();
    ui->listWidgetChamps->clear();

    QSqlQuery getFields("show fields from "+item->text()+";");

    while(getFields.next())
        {
            QString champ = getFields.value(0).toString();
            qDebug()<<champ;
            ui->listWidgetChamps->addItem(champ);
        }

}

void MainWindow::on_textEditReqSQL_textChanged()
{
    ui->labelError->setText("");
    QString text = ui->textEditReqSQL->toPlainText();
    if (!(text.endsWith(";")))
    {
        text += ";";
    }
    qDebug()<<text;
    QSqlQuery requete(text);
    if (requete.isActive())
    {
        ui->LayoutResultat->show();


        ui->pushButtonInfo->setStyleSheet("background-image:url(:/ressource/valid);");
        QSqlRecord rec = requete.record();
        int colMax = rec.count();
        int ligneMax = requete.size();
        QStringList listIntitule;
        for (int i=0; i<colMax; i++)
        {
            listIntitule.push_back(rec.fieldName(i));
        }
        qDebug()<<colMax;
        qDebug()<<ligneMax;
        int ligne = 0;
        ui->tableWidgetResReq->setColumnCount(colMax);
        ui->tableWidgetResReq->setRowCount(ligneMax);

        while(requete.next())
        {

            for (int col=0;col<colMax;col++)
            {
                qDebug()<<col;

                QString result = requete.value(col).toString();
                QTableWidgetItem* item = new QTableWidgetItem(result);
                ui->tableWidgetResReq->setItem(ligne,col,item);
                qDebug()<<result;
            }
            ligne++;
        }
        ui->tableWidgetResReq->setHorizontalHeaderLabels(listIntitule);
        ui->pushButtonSaveHistory->setEnabled(true);

    }else
    {
        ui->LayoutResultat->hide();
        ui->pushButtonInfo->setStyleSheet("background-image:url(:/ressource/cross);");
        ui->pushButtonSaveHistory->setEnabled(false);
    }
}

void MainWindow::on_pushButtonInfo_clicked()
{
    QString etatBouton = ui->pushButtonInfo->styleSheet();
    if (etatBouton == "background-image:url(:/ressource/valid);")
    {
        ui->labelError->setText("the request has been executed");
    }else
    {
        QString text = ui->textEditReqSQL->toPlainText();
        qDebug()<<text;
        QSqlQuery requete(text);
        QString error = requete.lastError().text();
        ui->labelError->setText(error);
    }
}

void MainWindow::on_listWidgetHistory_itemDoubleClicked(QListWidgetItem *item)
{
    QString text = item->text();
    ui->textEditReqSQL->setText(text);
}

void MainWindow::on_pushButtonSaveHistory_clicked()
{
    QString text = ui->textEditReqSQL->toPlainText();
    if (!(text.endsWith(";")))
    {
        text += ";";
    }
    ui->listWidgetHistory->addItem(text);
}

void MainWindow::on_pushButtonSave_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save File"), "/home/fjoly/reqBDD", "*.sql");
    fileName += ".sql";

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return;
    }
    QTextStream out(&file);
    for (int i = 0; i < ui->listWidgetHistory->count(); i++)
    {
        QString itemData = ui->listWidgetHistory->item(i)->text();
        out << itemData << "\n";
    }

}


void MainWindow::on_pushButtonLoad_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open File"), "/home/fjoly/reqBDD", "*.sql");

/*
    QFile file(fileName+".sql");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        process_line(line);
    }*/

    QFile file(fileName);

    if( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
        QMessageBox::warning(this, tr("Error opening!"), tr("Could not open the file"));

    QTextStream stream( &file );

    QString item = "";
    while( !stream.atEnd() )
    {
        QString lineText;
        lineText = stream.readLine(); //Read a line of text
        qDebug()<< lineText;
        if (lineText.endsWith(";"))
        {
            if (!(item==""))
            {
                item += " ";
            }
            item += lineText;
            ui->listWidgetHistory->addItem(item);
            item = "";
            qDebug()<< "oui";
        }else
        {
            if (!(item==""))
            {
                item += " ";
            }
            item += lineText;
            qDebug()<< "non";
        }
    }


    file.close();
}

