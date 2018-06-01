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
    m_pReadDcmFile(NULL),
    m_pTagsInfoTable(NULL)
{
    ui->setupUi(this);

    InitToolBar();

    ui->MultiFrame_verticalSlider->setMaximum(0);
    QObject::connect(ui->actionOpen, SIGNAL(triggered(bool)), this, SLOT(on_OpenImg_pushButton_clicked()));
    QObject::connect(ui->actionClose, SIGNAL(triggered(bool)), this, SLOT(on_Close_pushButton_clicked()));
    QObject::connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(on_About_pushButton_clicked()));

    this->setStyleSheet("background-color:gray;");
}

MainWindow::~MainWindow()
{
    if (m_pReadDcmFile!= NULL)
    {
        delete m_pReadDcmFile;
        m_pReadDcmFile = NULL;
    }

    if (m_pTagsInfoTable != NULL)
    {
        m_pTagsInfoTable->close();
        delete m_pTagsInfoTable;
        m_pTagsInfoTable = NULL;
    }

    ClearAnnotations();

    delete ui;
}

void MainWindow::SetFileName(const char* fileName)
{
    m_strFileName = fileName;

    if(m_strFileName.isNull() || m_strFileName.isEmpty())
    {
        return;
    }

    OpenImg();
}

void MainWindow::InitToolBar()
{
    ui->mainToolBar->addAction(ui->actionOpen);
    ui->mainToolBar->addAction(ui->actionClose);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(ui->actionDicomTags);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(ui->actionAbout);
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
void MainWindow::closeEvent(QCloseEvent *)
{
    if (m_pTagsInfoTable != NULL)
    {
        m_pTagsInfoTable->close();
        delete m_pTagsInfoTable;
        m_pTagsInfoTable = NULL;
    }
}
void MainWindow::on_OpenImg_pushButton_clicked()
{
    CloseImg();

    m_strFileName = QFileDialog::getOpenFileName(NULL, tr("select a dicom file"), tr("../../dicom-files"));
    if (m_strFileName==NULL)
    {
        QMessageBox::information(NULL, "Find file error", "File name is empty");
    }
    else
    {
        OpenImg();
    }
}

void MainWindow::OpenImg()
{
    this->setWindowTitle(m_strFileName);

    m_pReadDcmFile = new CReadDcmFile();
    bool ret = m_pReadDcmFile->ReadFile(m_strFileName.toLatin1().data());
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

        ShowAnnotations();
    }
    else
    {
        m_strFileName.clear();
        QMessageBox::information(NULL, tr("Open file failed"), tr("Can't open file."));
        delete m_pReadDcmFile;
        m_pReadDcmFile = NULL;
    }

}
void MainWindow::on_Close_pushButton_clicked()
{
    CloseImg();
}

void MainWindow::on_MultiFrame_verticalSlider_valueChanged(int value)
{
    int sliderPos = ui->MultiFrame_verticalSlider->value();

    if (sliderPos<=ui->MultiFrame_verticalSlider->maximum() && m_pReadDcmFile!=NULL)
    {
        QPixmap* pixmap = m_pReadDcmFile->GetPixmap(sliderPos);
        ui->Image_label->setPixmap(*pixmap);
        ui->Image_label->setAlignment(Qt::AlignCenter);
        ui->Image_label->show();
        ShowAnnotations();
    }
}

void MainWindow::on_actionDicomTags_triggered()
{
    const char* strFileName = "XML.xml";
    if (m_pReadDcmFile==NULL)
    {
        QMessageBox::information(this, tr("Info"), tr("No DCM file opened!"));
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
        if (m_pTagsInfoTable != NULL)
        {
            m_pTagsInfoTable->close();
            delete m_pTagsInfoTable;
            m_pTagsInfoTable = NULL;
        }
        m_pTagsInfoTable = new QTableWidget(0,6);
        m_pTagsInfoTable->setHorizontalHeaderLabels({"Tag", "VR", "VM", "Length", "Description", "Value"});
        QHeaderView *pHeader = m_pTagsInfoTable->verticalHeader();
        pHeader->setHidden(true);
        m_pTagsInfoTable->setGeometry(200, 100, 1000, 600);
        m_pTagsInfoTable->setWindowTitle(tr("DICOM tag information"));
        m_pTagsInfoTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        m_pTagsInfoTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        m_pTagsInfoTable->setColumnWidth(0, 150);
        m_pTagsInfoTable->setColumnWidth(1, 30);
        m_pTagsInfoTable->setColumnWidth(2, 30);
        m_pTagsInfoTable->setColumnWidth(3, 50);
        m_pTagsInfoTable->setColumnWidth(4, 300);
        m_pTagsInfoTable->setColumnWidth(5, 1200);
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

                 m_pTagsInfoTable->insertRow(rowIndex);
                 m_pTagsInfoTable->setItem(rowIndex,0,new QTableWidgetItem(tag));
                 m_pTagsInfoTable->setItem(rowIndex,1,new QTableWidgetItem(vr));
                 m_pTagsInfoTable->setItem(rowIndex,2,new QTableWidgetItem(vm));
                 m_pTagsInfoTable->setItem(rowIndex,3,new QTableWidgetItem(len));
                 m_pTagsInfoTable->setItem(rowIndex,4,new QTableWidgetItem(name));
                 m_pTagsInfoTable->setItem(rowIndex,5,new QTableWidgetItem(text));
                 rowIndex++;
            }
        }

        file.close();

        m_pTagsInfoTable->show();
    }
    else
    {
        QMessageBox::information(this, tr("ERROR"), tr("Open file failed."));
    }
}

void MainWindow::ShowAnnotations()
{
    ClearAnnotations();
    if (m_pReadDcmFile==NULL)
    {
        return;
    }
    stAnnotationInfo* pInfo = m_pReadDcmFile->GetAnnotationInfo();
    if (pInfo==NULL)
    {
        return;
    }
    int labelHeight(30), labelWidth(500);
    int labelMargin(25);//控件与边界的间距
    QFont ft;
    ft.setPixelSize(25);
    ft.setBold(false);
    ft.setFamily("仿宋");
    QPalette yellowPa;
    yellowPa.setColor(QPalette::WindowText, QColor::fromRgb(200,200,30));

    QLabel* pNameLabel = new QLabel(pInfo->strPatientName.c_str(), ui->Image_label);
    pNameLabel->setAlignment(Qt::AlignRight);
    pNameLabel->setFont(ft);
    pNameLabel->setPalette(yellowPa);
    pNameLabel->setStyleSheet("background:transparent");
    pNameLabel->setGeometry(ui->Image_label->geometry().right()-labelWidth-labelMargin, labelMargin, labelWidth, labelHeight);
    pNameLabel->show();

    QLabel* pPIDLabel = new QLabel(pInfo->strPatientID.c_str(), ui->Image_label);
    pPIDLabel->setAlignment(Qt::AlignRight);
    pPIDLabel->setFont(ft);
    pPIDLabel->setPalette(yellowPa);
    pPIDLabel->setStyleSheet("background:transparent");
    pPIDLabel->setGeometry(pNameLabel->geometry().left(), pNameLabel->geometry().bottom(), labelWidth, labelHeight);
    pPIDLabel->show();

    QLabel* pSexLabel = new QLabel(pInfo->strPatientSex.c_str(), ui->Image_label);
    pSexLabel->setAlignment(Qt::AlignRight);
    pSexLabel->setFont(ft);
    pSexLabel->setPalette(yellowPa);
    pSexLabel->setStyleSheet("background:transparent");
    pSexLabel->setGeometry(pPIDLabel->geometry().left(), pPIDLabel->geometry().bottom(), labelWidth, labelHeight);
    pSexLabel->show();

    QLabel* pStudyIDLabel = new QLabel(pInfo->strStudyID.c_str(), ui->Image_label);
    pStudyIDLabel->setAlignment(Qt::AlignRight);
    pStudyIDLabel->setFont(ft);
    pStudyIDLabel->setPalette(yellowPa);
    pStudyIDLabel->setStyleSheet("background:transparent");
    pStudyIDLabel->setGeometry(pSexLabel->geometry().left(), pSexLabel->geometry().bottom(), labelWidth, labelHeight);
    pStudyIDLabel->show();

    QLabel* pStudyDateTimeLabel = new QLabel((pInfo->strStudyDate + " " + pInfo->strStudyTime).c_str(), ui->Image_label);
    pStudyDateTimeLabel->setAlignment(Qt::AlignRight);
    pStudyDateTimeLabel->setFont(ft);
    pStudyDateTimeLabel->setPalette(yellowPa);
    pStudyDateTimeLabel->setStyleSheet("background:transparent");
    pStudyDateTimeLabel->setGeometry(ui->Image_label->geometry().right()-labelWidth-labelMargin, ui->Image_label->height() -labelHeight-labelMargin, labelWidth, labelHeight);
    pStudyDateTimeLabel->show();

    QString strFrameIndex = tr("Frame: ") + QString::number( ui->MultiFrame_verticalSlider->value()+1, 10) + tr("/")
            + QString::number( m_pReadDcmFile->GetFrameCount(), 10);
    QLabel* pFrameIndexLabel = new QLabel(strFrameIndex, ui->Image_label);
    pFrameIndexLabel->setAlignment(Qt::AlignLeft);
    pFrameIndexLabel->setFont(ft);
    pFrameIndexLabel->setPalette(yellowPa);
    pFrameIndexLabel->setStyleSheet("background:transparent");
    pFrameIndexLabel->setGeometry(labelMargin, labelMargin, labelWidth, labelHeight);
    pFrameIndexLabel->show();

    QLabel* pSeriesLabel = new QLabel( tr("Series: ") + pInfo->strSeriesNum.c_str(), ui->Image_label);
    pSeriesLabel->setAlignment(Qt::AlignLeft);
    pSeriesLabel->setFont(ft);
    pSeriesLabel->setPalette(yellowPa);
    pSeriesLabel->setStyleSheet("background:transparent");
    pSeriesLabel->setGeometry(labelMargin, pFrameIndexLabel->geometry().bottom(), labelWidth, labelHeight);
    pSeriesLabel->show();

    QString strWindow = tr("W:") + QString::number(pInfo->nWindowWidth, 10) + tr(" L:") + QString::number(pInfo->nWindowCenter, 10);
    QLabel* pWindowLabel = new QLabel(strWindow, ui->Image_label);
    pWindowLabel->setAlignment(Qt::AlignLeft);
    pWindowLabel->setFont(ft);
    pWindowLabel->setPalette(yellowPa);
    pWindowLabel->setStyleSheet("background:transparent");
    pWindowLabel->setGeometry(labelMargin, ui->Image_label->geometry().bottom()-labelHeight-labelMargin, labelWidth, labelHeight);
    pWindowLabel->show();
}

void MainWindow::ClearAnnotations()
{
    QObjectList pLabelList= ui->Image_label->children();
    foreach (QObject* pLabel, pLabelList)
    {
        if (pLabel!=NULL)
        {
            reinterpret_cast<QLabel*>(pLabel)->clear();
            delete pLabel;
        }
    }
    pLabelList.clear();
}

void MainWindow::CloseImg()
{
    ui->Image_label->clear();
    ClearAnnotations();
    if (m_pTagsInfoTable != NULL)
    {
        m_pTagsInfoTable->close();
        delete m_pTagsInfoTable;
        m_pTagsInfoTable = NULL;
    }
    if(m_pReadDcmFile!=NULL)
    {
        delete m_pReadDcmFile;
        m_pReadDcmFile = NULL;
    }
    ui->MultiFrame_verticalSlider->setMaximum(0);
    ui->MultiFrame_verticalSlider->setVisible(false);
    ui->MultiFrame_verticalSlider->setValue(0);
    this->setWindowTitle("");
    m_strFileName.clear();
}

void MainWindow::on_About_pushButton_clicked()
{
    QFont ft;
    ft.setPixelSize(15);
    ft.setBold(false);
    ft.setFamily("仿宋");

    QDialog* pAboutWgt = new QDialog(this);
    pAboutWgt->setFixedSize(360, 200);
    pAboutWgt->setWindowTitle(tr("About"));

    QVBoxLayout* pLayout = new QVBoxLayout();
    pAboutWgt->setLayout(pLayout);
    QLabel* pLable = new QLabel();
    pLable->setFont(ft);
    pLable->setText(("Version: 1.0.0\n\n"
                     "QQ: 547609650 \n"
                     "e-mail: qiaoxin686@163.com\n"
                     "欢迎DICOM开发的同仁们沟通交流.\n\n"
                     "个人原创,未经许可严禁用于一切商业行为."));
    pLayout->addWidget(pLable);

    pAboutWgt->exec();
}
