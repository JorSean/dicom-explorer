#ifndef CREADDCMFILE_H
#define CREADDCMFILE_H

#include <QPixmap>
#include <string>
using namespace std;

class DcmFileFormat;

struct stAnnotationInfo
{
    stAnnotationInfo()
        : strPatientName("")
        , strPatientID("")
        , strPatientSex("")
        , strStudyID("")
        , strSeriesNum("")
        , strStudyDate("")
        , strStudyTime("")
        , nFrameCount(-1)
        , nFrameIndex(-1)
        , nWindowCenter(-1)
        , nWindowWidth(-1)
    {}

    string strPatientName;
    string strPatientID;
    string strPatientSex;
    string strStudyID;
    string strSeriesNum;
    string strStudyDate;
    string strStudyTime;

    int nFrameCount;
    int nFrameIndex;
    int nWindowCenter;
    int nWindowWidth;
};
class CReadDcmFile
{
public:
    CReadDcmFile();
    ~CReadDcmFile();

    bool ReadFile(const char* strFileName);
    ushort GetFrameCount();
    QPixmap* GetPixmap(ushort nFrame);
    bool WriteXML(const char* strFileName);
    stAnnotationInfo* GetAnnotationInfo() {return m_pAnnotationInfo;}

private:
    bool ReadAnnotationInfo();

private:
    DcmFileFormat* m_pDcmFile;
    ushort m_nMultiFrameCount;
    QPixmap* m_pQPixmap;
    stAnnotationInfo *m_pAnnotationInfo;
};

#endif // CREADDCMFILE_H
