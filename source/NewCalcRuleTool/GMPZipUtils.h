
/*!
*@file     
*@brief    压缩方法
*@author   zhaok
*@date     2013年9月25日
*@remarks  
*@version 3.0
*Copyright (c) 1998-2013 Glodon Corporation
*/
#ifndef GMPZIPUTILS_H
#define GMPZIPUTILS_H

#include "GLDStringList.h"
#include "GLDString.h"
class IGLDZipFile;
//zip处理
class /*GMPCORE_EXPORT*/ GMCZipFile
{
public:
    GMCZipFile(const GString sPassWord);
    ~GMCZipFile();

    /*!
    *@brief   解压文件 
    *@author  2016年12月9日
    *@param[in]   const GString sExtractFile 待解压文件
    *@param[in]   const GString sDestFolder  目标目录
    *@param[in]   GStrings* pFiles = NULL    解压后的文件列表（外部传入）
    *@return       static bool               成功标志
    */
    static bool extractFiles(const GString sExtractFile, const GString sDestFolder, GStrings* pFiles = NULL);

    /*!
    *@brief   压缩文件 
    *@author  2016年12月9日
    *@param[out]   GStrings& pFiles         待压缩文件列表（不含目录名）
    *@param[in]   const GString sTempFolder 待压缩文件所在目录
    *@param[in]   const GString sDestFileName 目标文件名
    *@return       static bool
    */
    static bool compressFiles(GStrings& pFiles, const GString sTempFolder, const GString sDestFileName);

    /*!
    *@brief   解压缩指定的文件 
    *@author  2016年12月9日
    *@param[in]   const GString &zipFile     待解压zip文件
    *@param[in]   const GString &specifiedFile 指定解压出的文件名
    *@param[in]   const GString &destFileName  目标文件名
    *@return       static bool
    */
    static bool uncompressFile(const GString &zipFile, const GString &specifiedFile, const GString &destFileName);
	bool getFileNames( const GString &zipFile, GStrings& pFiles);
protected:
    bool getZipFileNames( const GString &zipFile, GStrings& pFiles);
    bool get7ZipFileNames( const GString &zipFile, GStrings& pFiles);
    virtual void addFile(const GString sFileName);
    //老的压缩算法
    bool doExtractFiles(const GString sExtractFile, const GString sDestFolder, GStrings* pFiles = NULL);
    bool doCompressFiles(GStrings& pFiles, const GString sTempFolder, const GString sDestFileName);
    bool doUncompressFile(const GString &zipFile, const GString &specifiedFile, const GString &destFileName);
    //新的7zip压缩算法
    bool doExtractFilesBy7zip(const GString sExtractFile, const GString sDestFolder, GStrings* pFiles = NULL);
    bool doCompressFilesBy7zip(GStrings& pFiles, const GString sTempFolder, const GString sDestFileName);
    bool doUncompressBy7zip(const GString &sZipFile, const GString &sSpecifiedFile, const GString &sDestFileName);
private:
    IGLDZipFile* m_pZipFile;
    GString m_sPassWord;
};

#endif
