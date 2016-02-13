#ifndef ADDFILE_H
#define ADDFILE_H

#include <QDialog>
#include <QDir>
#include <QString>

#include "czmlconverter.h"

namespace Ui {
class AddFile;
}

class AddFile : public QDialog
{
    Q_OBJECT

public:
    explicit AddFile(QDir filePath, QWidget *parent = 0);
    ~AddFile();


private slots:

    void on_src_toolButton_pressed();
    void onSrc_FileSelected(const QString &path);
    void on_buttonBox_accepted();
    
signals:
    void setHint(QString &hint, QString &filetype);
    void fileConverted(Converter *convertedData);

private:
    Ui::AddFile *ui;
    QDir _filePath;
};

#endif // ADDFILE_H
