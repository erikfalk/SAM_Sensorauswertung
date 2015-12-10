#include <QMessageBox>
#include "addfile.h"
#include "ui_addfile.h"
#include "filebrowser.h"
#include "converter.h"
#include "findpeaks.h"
#include "mainwindow.h"
#include "QStandardPaths"

AddFile::AddFile(QDir filePath, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddFile)
{
    ui->setupUi(this);
    _filePath = filePath;
}

AddFile::~AddFile()
{
    delete ui;
}



//filebrowser for csv file
void AddFile::on_src_toolButton_pressed()
{

    Filebrowser fb;

    //signal and slot for source filepath
    connect(&fb, SIGNAL(FileSelected(QString)),
            this, SLOT(onSrc_FileSelected(QString)));

    connect(this, SIGNAL(setHint(QString&, QString&)),
            &fb, SLOT(onSetHint(QString&, QString&)));

    QString hint = "<h2><center>Please select a csv file.</center></h2>";
    QString filetype = "*.csv";
    emit this->setHint(hint, filetype);

    fb.exec();

}


//check if file has correct ending and put path into textedit
void AddFile::onSrc_FileSelected(const QString &path)
{

    if(path.endsWith(".csv")){
        ui->source_TextEdit->setPlainText(path);
    }
    else{
        QFileInfo fi(path);
        QMessageBox::warning(this, "Warning", fi.fileName() + "\nis not a CSV File!");
    }
}

void AddFile::onDst_FileSelected(const QString &path)
{
    ui->destination_TextEdit->setPlainText(path);
}

void AddFile::on_buttonBox_accepted()
{
    Converter* myConverter = new Converter;

    myConverter->extractSensorData(ui->source_TextEdit->toPlainText());

    //Signals to MainWindow where converted data is
    emit fileConverted(myConverter);



    if(myConverter->writeCzml(_filePath, myConverter->getIncompleteSensorData()) == -1){

        QMessageBox::warning(this, "Error", "Could not write czml File!\n "
                                            "Please check if selected file ist correct.");
    }else{
        QMessageBox::information(this, "Success", "Converting successful.");

    }

}

