#ifndef ADDFILE_H
#define ADDFILE_H

#include <QDialog>

namespace Ui {
class AddFile;
}

class AddFile : public QDialog
{
    Q_OBJECT

public:
    explicit AddFile(QWidget *parent = 0);
    ~AddFile();


private slots:

    void on_src_toolButton_pressed();
    void on_dst_toolButton_pressed();
    void onSrc_FileSelected(const QString &path);
    void onDst_FileSelected(const QString &path);
    void on_buttonBox_accepted();
    
signals:
    void setHint(QString &hint, QString &filetype);


private:
    Ui::AddFile *ui;
};

#endif // ADDFILE_H
