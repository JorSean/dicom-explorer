#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class QTableWidget;
class CReadDcmFile;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void wheelEvent(QWheelEvent* evt);

protected:
    virtual void closeEvent(QCloseEvent *event);

private slots:
    void on_OpenImg_pushButton_clicked();
    void on_MultiFrame_verticalSlider_valueChanged(int value);

    void on_actionDicomTags_triggered();

private:
    void ShowAnnotations();
    void ClearAnnotations();


private:
    Ui::MainWindow *ui;

    CReadDcmFile* m_pReadDcmFile;
    QTableWidget *m_pTagsInfoTable;
};

#endif // MAINWINDOW_H
