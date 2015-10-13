#include <QMessageBox>

#include "addfile.h"
#include "ui_addfile.h"
#include "filebrowser.h"
#include "converter.h"

AddFile::AddFile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddFile)
{
    ui->setupUi(this);


}

AddFile::~AddFile()
{
    delete ui;
}

void AddFile::on_src_toolButton_pressed()
{

    Filebrowser fb;

    //signal and slot for source filepath
    connect(&fb, SIGNAL(FileSelected(QString)),
            this, SLOT(onSrc_FileSelected(QString)));

    connect(this, SIGNAL(setHint(QString&)),
            &fb, SLOT(onSetHint(QString&)));

    QString hint = "<h2><center>Please select a csv file.</center></h2>";
    emit this->setHint(hint);

    fb.exec();

}

void AddFile::on_dst_toolButton_pressed()
{
    Filebrowser fb;

    //signal and slot for destination filepath
    connect(&fb, SIGNAL(FileSelected(QString)),
            this, SLOT(onDst_FileSelected(QString)));

    //signal and slot for destination hint
    connect(this, SIGNAL(setHint(QString&)),
            &fb, SLOT(onSetHint(QString&)));

    QString hint = "<h2><center>Please select a czml file or folder to save the czml file.</center></h2>";
    emit this->setHint(hint);

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
    QVector<SensorData> data;
    
    getSensorData(ui->source_TextEdit->toPlainText(), data);

    if(writeCzml(ui->destination_TextEdit->toPlainText(), data) == -1)
        QMessageBox::warning(this, "Error", "Could not write czml File!\n Please check if selected file ist correct.");
}