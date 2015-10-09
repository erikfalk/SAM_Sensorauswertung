#ifndef FILEBROWSER_H
#define FILEBROWSER_H

#include <QDialog>
#include <QFileSystemModel>

#include "addfile.h"

namespace Ui {
class Filebrowser;
}

class Filebrowser : public QDialog
{
    Q_OBJECT

public:
    explicit Filebrowser(QWidget *parent = 0);
    ~Filebrowser();

private slots:
    void on_buttonBox_accepted();
    void onSetHint(QString &hint);

signals:
    void FileSelected(const QString &path);

private:
    Ui::Filebrowser *ui;
    QFileSystemModel *filemodel;
};

#endif // FILEBROWSER_H
