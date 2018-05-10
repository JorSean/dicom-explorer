#include "creaddcmfile.h"


#include "dcdeftag.h"
#include "dcdatset.h"
#include "dcelem.h"
#include "dcfilefo.h"
#include "dcuid.h"
#include "dcrledrg.h"
#include "dcmetinf.h"
#include "djdecode.h"
#include "dcjson.h"
#include <QtXml>

CReadDcmFile::CReadDcmFile()
    : m_nMultiFrameCount(1)
    , m_pQPixmap(NULL)
    , m_pDcmFile(NULL)
    , m_pAnnotationInfo(NULL)
{
}

CReadDcmFile::~CReadDcmFile()
{
    if (m_pQPixmap!=NULL)
    {
        delete [] m_pQPixmap;
        m_pQPixmap = NULL;
    }
    if(m_pDcmFile!=NULL)
    {
        delete m_pDcmFile;
        m_pDcmFile = NULL;
    }
    if (m_pAnnotationInfo!=NULL)
    {
        delete m_pAnnotationInfo;
        m_pAnnotationInfo = NULL;
    }
}

bool CReadDcmFile::ReadFile(const char* strFileName)
{
    if (m_pDcmFile!=NULL)
    {
        delete m_pDcmFile;
    }
    if (m_pAnnotationInfo!=NULL)
    {
        delete m_pAnnotationInfo;
        m_pAnnotationInfo = NULL;
    }
    m_pDcmFile = new DcmFileFormat();
    OFCondition result = m_pDcmFile->loadFile(strFileName);
    if (result.bad())
    {
        delete m_pDcmFile;
        m_pDcmFile = NULL;
        return false;
    }

    DcmDataset* dataset = m_pDcmFile->getDataset();
    if (dataset == NULL)
    {
        delete m_pDcmFile;
        m_pDcmFile = NULL;
        return false;
    }

    DcmMetaInfo* meta = m_pDcmFile->getMetaInfo();

    DcmElement *element = NULL;

    OFString transferSyntaxUID;
    result = meta->findAndGetElement(DCM_TransferSyntaxUID, element);
    if (result.bad() || element == NULL)
    {
//        assert(false);
//        return false;
    }
    else
    {
        element->getOFString(transferSyntaxUID, 0);
    }

    if (transferSyntaxUID.compare(UID_RLELosslessTransferSyntax)==0)
    {
        DcmRLEDecoderRegistration::registerCodecs();

        result = dataset->chooseRepresentation(EXS_LittleEndianExplicit, NULL);
        DcmRLEDecoderRegistration::cleanup();
        if (result.bad())
        {
            delete m_pDcmFile;
            m_pDcmFile = NULL;
            return false;
        }
    }
    else if ( transferSyntaxUID.compare(UID_JPEGProcess14SV1TransferSyntax)==0
             || transferSyntaxUID.compare(UID_JPEGProcess1TransferSyntax)==0 )
    {
        DJDecoderRegistration::registerCodecs(EDC_lossyOnly);
        result = dataset->chooseRepresentation(EXS_LittleEndianExplicit, NULL);
        DJDecoderRegistration::cleanup();
        if (result.bad())
        {
            delete m_pDcmFile;
            m_pDcmFile = NULL;
            return false;
        }
    }

    //sample per pixel
    result = dataset->findAndGetElement(DCM_SamplesPerPixel, element);
    if (result.bad() || element == NULL)
    {
        assert(false);
        delete m_pDcmFile;
        m_pDcmFile = NULL;
        return false;
    }
    Uint16 samplesPerPixel(0);
    element->getUint16(samplesPerPixel);

    result = dataset->findAndGetElement(DCM_PhotometricInterpretation, element);
    if (result.bad() || element==NULL)
    {
        assert(false);
        delete m_pDcmFile;
        m_pDcmFile = NULL;
        return false;
    }
    OFString photometricInterpretation;
    element->getOFString(photometricInterpretation,0);

    result = dataset->findAndGetElement(DCM_Columns, element);
    if (result.bad() || element == NULL)
    {
        assert(false);
        delete m_pDcmFile;
        m_pDcmFile = NULL;
        return false;
    }
    Uint16 nColumns(0);
    element->getUint16(nColumns);

    result = dataset->findAndGetElement(DCM_Rows, element);
    if (result.bad() || element == NULL)
    {
        assert(false);
        delete m_pDcmFile;
        m_pDcmFile = NULL;
        return false;
    }
    Uint16 nRows(0);
    element->getUint16(nRows);

    result = dataset->findAndGetElement(DCM_BitsAllocated, element);
    if (result.bad() || element == NULL)
    {
        assert(false);
        delete m_pDcmFile;
        m_pDcmFile = NULL;
        return false;
    }
    Uint16 nBitsAllocated(0);
    element->getUint16(nBitsAllocated);

    result = dataset->findAndGetElement(DCM_BitsStored, element);
    if (result.bad() || element == NULL)
    {
        assert(false);
        delete m_pDcmFile;
        m_pDcmFile = NULL;
        return false;
    }
    Uint16 nBitsStored(0);
    element->getUint16(nBitsStored);

    result = dataset->findAndGetElement(DCM_HighBit, element);
    if (result.bad() || element == NULL)
    {
        assert(false);
        delete m_pDcmFile;
        m_pDcmFile = NULL;
        return false;
    }
    Uint16 nHighBit(0);
    element->getUint16(nHighBit);


    result = dataset->findAndGetElement(DCM_PixelRepresentation, element);
    if (result.good() && element != NULL)
    {
        Uint16 nPixelRepresentation(0);
        element->getUint16(nPixelRepresentation);
    }

    Uint16 planarConfiguration(0);
    result = dataset->findAndGetElement(DCM_PlanarConfiguration, element);
    if (result.good() && element != NULL)
    {
        element->getUint16(planarConfiguration);
    }

    result = dataset->findAndGetElement(DCM_PixelAspectRatio, element);
    if (result.good() && element != NULL)
    {
        Uint16 nPixelAspectRatio(0);
        element->getUint16(nPixelAspectRatio);
    }

    Sint16 smallestImagePixelValue(0x7FFF);
    result = dataset->findAndGetElement(DCM_SmallestImagePixelValue, element);
    if (result.good() && element != NULL)
    {
        element->getSint16(smallestImagePixelValue);
    }

    Sint16 largestImagePixelValue(0);
    result = dataset->findAndGetElement(DCM_LargestImagePixelValue, element);
    if (result.good() && element != NULL)
    {
        element->getSint16(largestImagePixelValue);
    }

    Uint16 pixelRepresentation(0);
    result = dataset->findAndGetElement(DCM_PixelRepresentation, element);
    if (result.good() && element != NULL)
    {
        element->getUint16(pixelRepresentation);
    }

    unsigned short* pPaletteColorLUTDescrip = NULL;
    result = dataset->findAndGetElement(DCM_RedPaletteColorLookupTableDescriptor, element);
    if (result.good() && element != NULL)
    {
        element->getUint16Array(pPaletteColorLUTDescrip);
    }
    //Green and Blue equal to red;

    unsigned short *pPaletteColorLUT_Red(NULL), *pPaletteColorLUT_Green(NULL), *pPaletteColorLUT_Blue(NULL);
    result = dataset->findAndGetElement(DCM_RedPaletteColorLookupTableData, element);
    if (result.good() && element != NULL)
    {
        element->getUint16Array(pPaletteColorLUT_Red);
    }
    result = dataset->findAndGetElement(DCM_GreenPaletteColorLookupTableData, element);
    if (result.good() && element != NULL)
    {
        element->getUint16Array(pPaletteColorLUT_Green);
    }
    result = dataset->findAndGetElement(DCM_BluePaletteColorLookupTableData, element);
    if (result.good() && element != NULL)
    {
        element->getUint16Array(pPaletteColorLUT_Blue);
    }


    //Modality LUT
    bool isRescaleIntercept(true);
    DcmSequenceOfItems *sequence = NULL;
    result = dataset->findAndGetSequence(DCM_ModalityLUTSequence, sequence);
    if (result.good() && element != NULL)
    {
        isRescaleIntercept = false;
    }

    float rescaleIntercept(0.0);
    result = dataset->findAndGetElement(DCM_RescaleIntercept, element);
    if (result.good() && element != NULL)
    {
        OFString strValue("");
        element->getOFString(strValue, 0);
        rescaleIntercept = atof(strValue.c_str());
    }
    float rescaleSlope(1.0);
    result = dataset->findAndGetElement(DCM_RescaleSlope, element);
    if (result.good() && element != NULL)
    {
        OFString strValue("");
        element->getOFString(strValue, 0);
        rescaleSlope = atof(strValue.c_str());
    }
    OFString rescaleType("");
    result = dataset->findAndGetElement(DCM_RescaleType, element);
    if (result.good() && element != NULL)
    {
        element->getOFString(rescaleType, 0);
    }
    //End Modality LUT

    //VOI LUT
    bool isVOILUTSeq(false);
    result = dataset->findAndGetSequence(DCM_VOILUTSequence, sequence);
    if (result.good() && sequence != NULL)
    {
        isVOILUTSeq = true;
    }

    float windowCenter(0.0);
    result = dataset->findAndGetElement(DCM_WindowCenter, element);
    if (result.good() && element != NULL)
    {
        isVOILUTSeq = false;
        OFString strValue("");
        element->getOFString(strValue, 0);
        windowCenter = atof(strValue.c_str());
    }
    float windowWidth(0.0);
    result = dataset->findAndGetElement(DCM_WindowWidth, element);
    if (result.good() && element != NULL)
    {
        OFString strValue("");
        element->getOFString(strValue, 0);
        windowWidth = atof(strValue.c_str());
    }
    //End VOI LUT

    //Multi-frame Module. PS3.3 2017e, C.7.6.6, P470.
    result = dataset->findAndGetElement(DCM_NumberOfFrames, element);
    if (result.good() && element != NULL)
    {
        OFString strValue("");
        element->getOFString(strValue, 0);
        m_nMultiFrameCount = atoi(strValue.c_str());
        assert(m_nMultiFrameCount>0);
    }

    DcmTagKey frameIncrementPointer;
    result = dataset->findAndGetElement(DCM_FrameIncrementPointer, element);
    if (result.good() && element != NULL)
    {
        Uint16* tag = NULL;
        element->getUint16Array(tag);
        frameIncrementPointer.setGroup(tag[0]);
        frameIncrementPointer.setElement(tag[1]);
    }
    //End Multi-frame Module.


    result = dataset->findAndGetElement(DCM_PixelData, element);
    if (result.bad() || element == NULL)
    {
        assert(false);
        delete m_pDcmFile;
        m_pDcmFile = NULL;
        return false;
    }

    Uint8* pPixSrcBuf;
    int pixLen(0);
    if (nBitsAllocated==8)
    {
        result = element->getUint8Array(pPixSrcBuf);
        pixLen = element->getLength();//num of bit
    }
    else if (nBitsAllocated==16)
    {
        Uint16* pSrcTemp(0);
        result = element->getUint16Array(pSrcTemp);
        pixLen = element->getLength();//num of byte
        pPixSrcBuf = (Uint8*)pSrcTemp;
    }
    else
    {
        assert(false);
        delete m_pDcmFile;
        m_pDcmFile = NULL;
        return false;
    }

    if (pixLen != m_nMultiFrameCount * nColumns * nRows * samplesPerPixel * nBitsAllocated/8)
    {
        assert(false);
        delete m_pDcmFile;
        m_pDcmFile = NULL;
        return false;
    }

    if( m_pQPixmap!=NULL )
    {
        delete [] m_pQPixmap;
        m_pQPixmap = NULL;
    }
    m_pQPixmap = new QPixmap[m_nMultiFrameCount];

    for (int frame=0; frame<m_nMultiFrameCount; frame++)
    {
        int* pSrcIntValue = new int[nRows*nColumns];//Save int value of src
        memset(pSrcIntValue, 0, nRows*nColumns*sizeof(int));

        //get int value for per pixel; and Find min and max from all samples.
        for (int r=0; r<nRows; r++)
        {
            for (int c=0; c<nColumns; c++)
            {
                assert(nBitsAllocated<32 && nBitsStored<32);
                int& srcValue = pSrcIntValue[r*nColumns+c];
                if (samplesPerPixel==1)
                {
                    Uint8* valueAddr = pPixSrcBuf + frame*nColumns*nRows*samplesPerPixel*nBitsAllocated/8
                            + (r*nColumns+c)*samplesPerPixel*nBitsAllocated/8;
                    //
                    if (pixelRepresentation==0)
                    {
                        memcpy(&srcValue, valueAddr, samplesPerPixel*nBitsAllocated/8);
                    }
                    else if (pixelRepresentation==1)
                    {
                        if (samplesPerPixel==1)
                        {
                            if (nBitsAllocated==8)
                            {
                                Sint8 sValue = (*(Sint8*)valueAddr) & (~(0xFF<<nBitsStored));
                                srcValue = sValue;
                            }
                            else if (nBitsAllocated == 16)
                            {
                                short sValue = (*((short*)valueAddr)) & (~(0xFFFF<<nBitsStored));
                                srcValue = sValue;
                            }
                            else
                            {
                                memcpy(&srcValue, valueAddr, samplesPerPixel*nBitsAllocated/8);
                                srcValue = srcValue & (~(0xFFFFFFFFFFFFFFFF<<nBitsStored));
                            }
                        }//end if (samplesPerPixel==1)
                    }

                    if (!isVOILUTSeq && windowWidth<0.1 && (smallestImagePixelValue>srcValue||largestImagePixelValue<srcValue))
                    {
                        smallestImagePixelValue = srcValue<smallestImagePixelValue?srcValue:smallestImagePixelValue;
                        largestImagePixelValue = srcValue>largestImagePixelValue?srcValue:largestImagePixelValue;
                    }
                }
                else if (samplesPerPixel==3)
                {
                    for (int sample=0; sample<samplesPerPixel; sample++)
                    {
                        if (planarConfiguration==0)//RGBRGBRGB...RGB
                        {
                            Uint8* rAddr = pPixSrcBuf + frame*nColumns*nRows*samplesPerPixel*nBitsAllocated/8
                                    + (r*nColumns+c)*samplesPerPixel*nBitsAllocated/8;
                            srcValue |= (*rAddr)<<16;
                            Uint8* gAddr = rAddr + nBitsAllocated/8;
                            srcValue |= (*gAddr)<<8;
                            Uint8* bAddr = gAddr + nBitsAllocated/8;
                            srcValue |= (*bAddr);

                            srcValue |= (0xFF<<24);
                        }
                        else if (planarConfiguration==1)//RRR...RGGG...GBBB...B
                        {
                            srcValue = 0xFF<<24;
                            Uint8* rAddr = pPixSrcBuf + frame*nColumns*nRows*nBitsAllocated/8
                                    + (r*nColumns+c)*nBitsAllocated/8;
                            srcValue |= (*rAddr)<<16;
                            Uint8* gAddr = rAddr + nColumns*nRows*nBitsAllocated/8;
                            srcValue |= (*gAddr)<<8;
                            Uint8* bAddr = gAddr + nColumns*nRows*nBitsAllocated/8;
                            srcValue |= (*bAddr);
                        }
                    }
                }//else if (samplesPerPixel == 3)
            }
        }

        uchar* pPixDstData = new uchar[nRows*nColumns];
        memset(pPixDstData, 0, nRows*nColumns*sizeof(uchar));
        QImage* img(NULL);
        if ( (strcmp(photometricInterpretation.c_str(), "MONOCHROME1")==0)
                || (strcmp(photometricInterpretation.c_str(), "MONOCHROME2")==0) )
        {
            for (int r=0; r<nRows; r++)
            {
                for (int c=0; c<nColumns; c++)
                {
                    int &srcValue = pSrcIntValue[r*nColumns+c];

                    //Modality LUT only use for grayscale
                    int modalityLUTValue(0);
                    if (isRescaleIntercept)
                    {
                        modalityLUTValue = rescaleSlope * srcValue + rescaleIntercept;
                    }
                    else
                    {
                    }

                    //VOI LUT only use for MONO1 or MONO2. ps3.3 2017e, C11.2.1.2.2 P1198.
                    Sint16 yMin(0), yMax(255);
                    Uint8 VOILUTValue(0);
                    if (isVOILUTSeq)
                    {
                    }
                    else
                    {
                        if (windowWidth<1 && (smallestImagePixelValue<yMin || largestImagePixelValue>yMax))
                        {
                            //if not def windowWidth/windowCenter, and pixel value not in range of yMin and yMax.
                            windowCenter = (smallestImagePixelValue+largestImagePixelValue)/2.0;
                            windowWidth = largestImagePixelValue-smallestImagePixelValue;
                        }
                        if (windowWidth>1)
                        {
                            if ( modalityLUTValue <= (windowCenter-0.5 - (windowWidth-1)/2.0) )
                            {
                                VOILUTValue = yMin;
                            }
                            else if( modalityLUTValue>(windowCenter-0.5 + (windowWidth-1)/2.0))
                            {
                                VOILUTValue = yMax;
                            }
                            else
                            {
                                VOILUTValue = ((modalityLUTValue-(windowCenter-0.5))/(windowWidth-1)+0.5)*(yMax-yMin) + yMin;
                            }
                        }
                        else
                        {
                            VOILUTValue = modalityLUTValue;
                        }
                    }//not VOI LUT Sequence

                    if (strcmp(photometricInterpretation.c_str(), "MONOCHROME1")==0)
                    {
                        VOILUTValue = yMax - VOILUTValue;
                    }
                    pPixDstData[r*nColumns+c] = VOILUTValue;
                }
            }

            img = new QImage(pPixDstData, (int)nColumns, (int)nRows, QImage::Format_Grayscale8);
        }
        else if (strcmp(photometricInterpretation.c_str(), "RGB")==0)
        {
            img = new QImage((Uint8*)pSrcIntValue, (int)nColumns, (int)nRows, QImage::Format_ARGB32);
        }
        else if (strcmp(photometricInterpretation.c_str(), "PALETTE COLOR")==0)
        {
            for (int r=0; r<nRows; r++)
            {
                for (int c=0; c<nColumns; c++)
                {
                    int &srcValue = pSrcIntValue[r*nColumns+c];

                    if ((pPaletteColorLUTDescrip[0]==0) || (srcValue < pPaletteColorLUTDescrip[0]))
                    {
                        if (srcValue<pPaletteColorLUTDescrip[1])
                        {
                            srcValue = pPaletteColorLUTDescrip[1];
                        }
                        unsigned short red = pPaletteColorLUT_Red[srcValue];
                        red = (float)red/65535.0f*255.0f;
                        unsigned short green = pPaletteColorLUT_Green[srcValue];
                        green = (float)green/65535.0f*255.0f;
                        unsigned short blue = pPaletteColorLUT_Blue[srcValue];
                        blue = (float)blue/65535.0f*255.0f;

                        srcValue = (0xFF<<24) | ((red&0xFF) << 16) | ((green&0xFF) << 8) | (blue&0xFF);
                    }
                }
            }

            img = new QImage((Uint8*)pSrcIntValue, (int)nColumns, (int)nRows, QImage::Format_ARGB32);
        }

        QPixmap file = QPixmap::fromImage(*img);
        m_pQPixmap[frame] = file;

        delete pSrcIntValue;
        pSrcIntValue = NULL;

        delete pPixDstData;
        pPixDstData = NULL;
    }//frame loop

    ReadAnnotationInfo();
    m_pAnnotationInfo->nWindowCenter = windowCenter<1? 128 : windowCenter;
    m_pAnnotationInfo->nWindowWidth = windowWidth<1? 256 : windowWidth;

    return true;
}

bool CReadDcmFile::ReadAnnotationInfo()
{
    if (m_pAnnotationInfo==NULL)
    {
        m_pAnnotationInfo=new stAnnotationInfo();
    }

    DcmElement* element = NULL;
    DcmDataset* dataset = m_pDcmFile->getDataset();
    OFCondition result = dataset->findAndGetElement(DCM_PatientName, element);
    if (result.good() && element != NULL)
    {
        element->getOFString(m_pAnnotationInfo->strPatientName, 0);
    }

    result = dataset->findAndGetElement(DCM_PatientID, element);
    if (result.good() && element!=NULL)
    {
        element->getOFString(m_pAnnotationInfo->strPatientID, 0);
    }
    result = dataset->findAndGetElement(DCM_PatientSex, element);
    if (result.good() && element!=NULL)
    {
        element->getOFString(m_pAnnotationInfo->strPatientSex, 0);
    }
    result = dataset->findAndGetElement(DCM_StudyID, element);
    if (result.good() && element!=NULL)
    {
        element->getOFString(m_pAnnotationInfo->strStudyID, 0);
    }
    result = dataset->findAndGetElement(DCM_SeriesNumber, element);
    if (result.good() && element!=NULL)
    {
        element->getOFString(m_pAnnotationInfo->strSeriesNum, 0);
    }
    result = dataset->findAndGetElement(DCM_StudyDate, element);
    if (result.good() && element!=NULL)
    {
        element->getOFString(m_pAnnotationInfo->strStudyDate, 0);
    }
    result = dataset->findAndGetElement(DCM_StudyTime, element);
    if (result.good() && element!=NULL)
    {
        element->getOFString(m_pAnnotationInfo->strStudyTime, 0);
    }
    result = dataset->findAndGetElement(DCM_NumberOfFrames, element);
    if (result.good() && element!=NULL)
    {
        string strFrameCount;
        element->getOFString(strFrameCount, 0);
        m_pAnnotationInfo->nFrameCount = atoi(strFrameCount.c_str());
    }
    result = dataset->findAndGetElement(DCM_WindowCenter, element);
    if (result.good() && element!=NULL)
    {
        Sint32 nWC;
        element->getSint32(nWC);
        m_pAnnotationInfo->nWindowCenter = nWC;
    }
    result = dataset->findAndGetElement(DCM_WindowWidth, element);
    if (result.good() && element!=NULL)
    {
        Sint32 nWW;
        element->getSint32(nWW);
        m_pAnnotationInfo->nWindowWidth = nWW;
    }



    return true;
}

ushort CReadDcmFile::GetFrameCount()
{
    return m_nMultiFrameCount;
}

QPixmap* CReadDcmFile::GetPixmap(ushort nFrame)
{
    if (m_pQPixmap == NULL)
    {
        return NULL;
    }

    return &(m_pQPixmap[nFrame]);
}


bool CReadDcmFile::WriteXML(const char* strFileName)
{
    if (m_pDcmFile==NULL)
    {
        return false;
    }

    std::ofstream fstream(strFileName);
    if (fstream.good())
    {
        OFCondition result = m_pDcmFile->writeXML(fstream);
        return result.good();
    }
    return false;
}
