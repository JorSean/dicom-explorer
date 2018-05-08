#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <QWheelEvent>

#include "creaddcmfile.h"
#include <QtXml>
#include <QTableWidget>
#include <QHeaderView>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_pReadDcmFile(NULL)
{
    ui->setupUi(this);

    ui->MultiFrame_verticalSlider->setMaximum(0);
    QObject::connect(ui->actionOpen, SIGNAL(triggered(bool)), this, SLOT(on_OpenImg_pushButton_clicked()));

    this->setStyleSheet("background-color:gray;");
}

MainWindow::~MainWindow()
{
    delete ui;

    if (m_pReadDcmFile!= NULL)
    {
        delete m_pReadDcmFile;
        m_pReadDcmFile = NULL;
    }
}

void MainWindow::wheelEvent(QWheelEvent *evt)
{
    if (ui->MultiFrame_verticalSlider->maximum()>1)
    {
        int curPos = ui->MultiFrame_verticalSlider->value();
        if (evt->delta()<0)
        {
            ui->MultiFrame_verticalSlider->setValue(++curPos);
        }
        else
        {
            ui->MultiFrame_verticalSlider->setValue(--curPos);
        }

    }
}

void MainWindow::on_OpenImg_pushButton_clicked()
{
    ui->Image_label->clear();
    ui->MultiFrame_verticalSlider->setVisible(false);
    ui->MultiFrame_verticalSlider->setValue(0);
    this->setWindowTitle("");
    QString fileName = QFileDialog::getOpenFileName(NULL, tr("select a dicom file"), tr("../../dicom-files"));
    if (fileName==NULL)
    {
        QMessageBox::information(NULL, "Find file error", "File name is empty");
    }
    else
    {
        this->setWindowTitle(fileName);

        if(m_pReadDcmFile!=NULL)
        {
            delete m_pReadDcmFile;
            m_pReadDcmFile = NULL;
        }
        m_pReadDcmFile = new CReadDcmFile();
        bool ret = m_pReadDcmFile->ReadFile(fileName.toLatin1().data());
        if (ret)
        {
            unsigned short frameCount = m_pReadDcmFile->GetFrameCount();
            if (frameCount>1)
            {
                ui->MultiFrame_verticalSlider->setVisible(true);
                ui->MultiFrame_verticalSlider->setRange(0, frameCount-1);
            }
            else
            {
                ui->MultiFrame_verticalSlider->setVisible(false);
            }

            QPixmap* pixmap = m_pReadDcmFile->GetPixmap(0);
            ui->Image_label->setPixmap(*pixmap);
            ui->Image_label->setAlignment(Qt::AlignCenter);
            ui->Image_label->show();
        }
        else
        {
            QMessageBox::information(NULL, tr("Open file failed"), tr("Can't open file."));
        }
    }
}

void MainWindow::on_MultiFrame_verticalSlider_valueChanged(int value)
{
    int sliderPos = ui->MultiFrame_verticalSlider->value();

    if (sliderPos<ui->MultiFrame_verticalSlider->maximum())
    {
        QPixmap* pixmap = m_pReadDcmFile->GetPixmap(sliderPos);
        ui->Image_label->setPixmap(*pixmap);
        ui->Image_label->setAlignment(Qt::AlignCenter);
        ui->Image_label->show();
    }
}

void MainWindow::on_actionDicomTags_triggered()
{
    char* strFileName = "XML.xml";
    if (m_pReadDcmFile==NULL)
    {
        QMessageBox::information(this, tr("Info"), tr("No xml file opened!"));
        return;
    }
    bool ret = m_pReadDcmFile->WriteXML(strFileName);
    if (!ret)
    {
        QMessageBox::information(this, tr("Info"), tr("Write XML Error!"));
        return;
    }

    QFile file(strFileName);
    if (file.open(QIODevice::ReadOnly))
    {
        QTableWidget *pInfoTable = new QTableWidget(0,6);
        pInfoTable->setHorizontalHeaderLabels({"Tag", "VR", "VM", "Length", "Description", "Value"});
        QHeaderView *pHeader = pInfoTable->verticalHeader();
        pHeader->setHidden(true);
        pInfoTable->setGeometry(200, 100, 1000, 600);
        pInfoTable->setWindowTitle(tr("DICOM tag information"));
        pInfoTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        pInfoTable->setColumnWidth(0, 150);
        pInfoTable->setColumnWidth(1, 30);
        pInfoTable->setColumnWidth(2, 30);
        pInfoTable->setColumnWidth(3, 50);
        pInfoTable->setColumnWidth(4, 300);
        pInfoTable->setColumnWidth(5, 1200);
        int rowIndex = 0;

        QXmlStreamReader readStream(&file);
        while (!readStream.atEnd() && !readStream.hasError())
        {
            QXmlStreamReader::TokenType token = readStream.readNext();
            if(token == QXmlStreamReader::StartDocument)
            {
                continue;
            }

            if (token == QXmlStreamReader::StartElement && readStream.name().toString()=="element")
            {
                 QString tag = readStream.attributes().value("tag").toString();
                 QString vr = readStream.attributes().value("vr").toString();
                 QString vm = readStream.attributes().value("vm").toString();
                 QString len = readStream.attributes().value("len").toString();
                 QString name = readStream.attributes().value("name").toString();
                 QString text = readStream.readElementText();

                 pInfoTable->insertRow(rowIndex);
                 pInfoTable->setItem(rowIndex,0,new QTableWidgetItem(tag));
                 pInfoTable->setItem(rowIndex,1,new QTableWidgetItem(vr));
                 pInfoTable->setItem(rowIndex,2,new QTableWidgetItem(vm));
                 pInfoTable->setItem(rowIndex,3,new QTableWidgetItem(len));
                 pInfoTable->setItem(rowIndex,4,new QTableWidgetItem(name));
                 pInfoTable->setItem(rowIndex,5,new QTableWidgetItem(text));
                 rowIndex++;
            }
        }

        file.close();

        pInfoTable->show();
    }
    else
    {
        QMessageBox::information(this, tr("ERROR"), tr("Open file failed."));
    }
}


