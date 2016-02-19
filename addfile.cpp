#include "addfile.h"


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
    ui->source_TextEdit->setPlainText(path);
}


void AddFile::on_buttonBox_accepted()
{
    QVector<SensorData> sensordata;
    CsvReader fileReader;
    Cleaner cleaner;
    CzmlConverter czmlConverter;

    fileReader.read(ui->source_TextEdit->toPlainText(), sensordata);

    if(!sensordata.empty()){
        //cleaner.grubbsTest(sensordata);
        czmlConverter.convertToFile(_filePath, sensordata);
        QMessageBox::information(this, "Info", "Converting sucessful!");

    } else {
        QMessageBox::warning(this, "Warning", "Nothing to convert!" );
    }
}

