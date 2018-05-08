#ifndef CREADDCMFILE_H
#define CREADDCMFILE_H

#include <QPixmap>

class DcmFileFormat;

class CReadDcmFile
{
public:
    CReadDcmFile();
    ~CReadDcmFile();

    bool ReadFile(const char* strFileName);
    ushort GetFrameCount();
    QPixmap* GetPixmap(ushort nFrame);
    bool WriteXML(const char* strFileName);

private:
    DcmFileFormat* m_pDcmFile;
    ushort m_nMultiFrameCount;
    QPixmap* m_pQPixmap;
};

#endif // CREADDCMFILE_H
