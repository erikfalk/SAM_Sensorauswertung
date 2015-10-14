#include <QDir>
#include <QStringList>
#include <QDebug>

#include "filebrowser.h"
#include "ui_filebrowser.h"

Filebrowser::Filebrowser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Filebrowser)
{
    ui->setupUi(this);

    filemodel = new QFileSystemModel(this);
    filemodel->setFilter(QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot);
    filemodel->setNameFilters(QStringList() << "*.csv" << "*.czml");
    filemodel->setNameFilterDisables(false);

    ui->treeView->setModel(filemodel);
    ui->treeView->setRootIndex(filemodel->setRootPath(QDir::rootPath()));
    ui->treeView->setColumnHidden(2,true);
    ui->treeView->setColumnWidth(0, 200);
}

Filebrowser::~Filebrowser()
{
   delete filemodel;
   delete ui;
}

void Filebrowser::on_buttonBox_accepted()
{
    //check if somthing is selected
    if(ui->treeView->selectionModel()->selectedIndexes().count() != 0){
        //get filepath of the selected file
        QString path = filemodel->fileInfo(ui->treeView->currentIndex()).absoluteFilePath();
        emit this->FileSelected(path);
    }

    this->close();
}

void Filebrowser::onSetHint(QString &hint, QString &filepath)
{
    ui->hint_label->setText(hint);
    if(filepath.endsWith("csv")){
        filemodel->setNameFilters(QStringList() << "*.csv");
        filemodel->setNameFilterDisables(false);
    }
    else{
        filemodel->setNameFilters(QStringList() << "*.czml");
        filemodel->setNameFilterDisables(false);
    }

}


