#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    setWindowTitle("About SQLite Editor");
    setWindowIcon(QIcon(":/img/icon-database.png"));

    QPixmap pixmap(":/img/icon-database.png");
    ui->imageLabel->setMinimumSize(96, 96);
    ui->imageLabel->setPixmap(pixmap.scaled(QSize(ui->imageLabel->width(), ui->imageLabel->height()), Qt::KeepAspectRatio));

    setFixedSize(size());

    connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(accept()));
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
