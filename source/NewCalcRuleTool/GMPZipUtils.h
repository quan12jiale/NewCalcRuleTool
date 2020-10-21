
/*!
*@file     
*@brief    ѹ������
*@author   zhaok
*@date     2013��9��25��
*@remarks  
*@version 3.0
*Copyright (c) 1998-2013 Glodon Corporation
*/
#ifndef GMPZIPUTILS_H
#define GMPZIPUTILS_H

#include "GLDStringList.h"
#include "GLDString.h"
class IGLDZipFile;
//zip����
class /*GMPCORE_EXPORT*/ GMCZipFile
{
public:
    GMCZipFile(const GString sPassWord);
    ~GMCZipFile();

    /*!
    *@brief   ��ѹ�ļ� 
    *@author  2016��12��9��
    *@param[in]   const GString sExtractFile ����ѹ�ļ�
    *@param[in]   const GString sDestFolder  Ŀ��Ŀ¼
    *@param[in]   GStrings* pFiles = NULL    ��ѹ����ļ��б��ⲿ���룩
    *@return       static bool               �ɹ���־
    */
    static bool extractFiles(const GString sExtractFile, const GString sDestFolder, GStrings* pFiles = NULL);

    /*!
    *@brief   ѹ���ļ� 
    *@author  2016��12��9��
    *@param[out]   GStrings& pFiles         ��ѹ���ļ��б�����Ŀ¼����
    *@param[in]   const GString sTempFolder ��ѹ���ļ�����Ŀ¼
    *@param[in]   const GString sDestFileName Ŀ���ļ���
    *@return       static bool
    */
    static bool compressFiles(GStrings& pFiles, const GString sTempFolder, const GString sDestFileName);

    /*!
    *@brief   ��ѹ��ָ�����ļ� 
    *@author  2016��12��9��
    *@param[in]   const GString &zipFile     ����ѹzip�ļ�
    *@param[in]   const GString &specifiedFile ָ����ѹ�����ļ���
    *@param[in]   const GString &destFileName  Ŀ���ļ���
    *@return       static bool
    */
    static bool uncompressFile(const GString &zipFile, const GString &specifiedFile, const GString &destFileName);
	bool getFileNames( const GString &zipFile, GStrings& pFiles);
protected:
    bool getZipFileNames( const GString &zipFile, GStrings& pFiles);
    bool get7ZipFileNames( const GString &zipFile, GStrings& pFiles);
    virtual void addFile(const GString sFileName);
    //�ϵ�ѹ���㷨
    bool doExtractFiles(const GString sExtractFile, const GString sDestFolder, GStrings* pFiles = NULL);
    bool doCompressFiles(GStrings& pFiles, const GString sTempFolder, const GString sDestFileName);
    bool doUncompressFile(const GString &zipFile, const GString &specifiedFile, const GString &destFileName);
    //�µ�7zipѹ���㷨
    bool doExtractFilesBy7zip(const GString sExtractFile, const GString sDestFolder, GStrings* pFiles = NULL);
    bool doCompressFilesBy7zip(GStrings& pFiles, const GString sTempFolder, const GString sDestFileName);
    bool doUncompressBy7zip(const GString &sZipFile, const GString &sSpecifiedFile, const GString &sDestFileName);
private:
    IGLDZipFile* m_pZipFile;
    GString m_sPassWord;
};

#endif
