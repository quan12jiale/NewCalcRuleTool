
/*!
*@brief  压缩算法  
*@author zhaok 2013年9月25日
*@param[out]   
*@param[in]    
*@return   
*/
/*#include "stdafx.h"*/
#include "GMPZipUtils.h"
#include "GLDSysUtils.h"
#include "GLDFileUtils.h"
#include "GLDException.h"
#include "Dll7zip/Dll7zipMgr.h"
#include "GLDZipFile.h"
/*!
*@brief  解压缩  
*@author zhaok 2013年9月25日
*@param[out]   
*@param[in] const GString sExtractFile
            const GString sDestFolder
            GStrings* pFiles = NULL 
*@return 成功/失败  
*/
bool GMCZipFile::extractFiles(const GString sExtractFile, const GString sDestFolder, GStrings* pFiles/* = NULL*/)
{
    GMCZipFile oZipFile("");
    try
    {
        if (isZipFile(sExtractFile))
        {
            return oZipFile.doExtractFiles(sExtractFile, sDestFolder, pFiles);
        }
        else
        {
			//return oZipFile.doExtractFiles(sExtractFile, sDestFolder, pFiles);
            return oZipFile.doExtractFilesBy7zip(sExtractFile, sDestFolder, pFiles);
        }
    }
    catch (GLDException* e)
    {
        return false;
    }
}

/*!
*@brief  压缩  
*@author zhaok 2013年9月25日
*@param[out]   
*@param[in] GStrings& pFiles  压缩文件列表
            const GString sTempFolder 文件夹
            const GString sDestFileName 目标文件
*@return   
*/
bool GMCZipFile::compressFiles(GStrings& pFiles, const GString sTempFolder, const GString sDestFileName)
{
    //QP_FUN("GMCZipFile::compressFiles");
    GMCZipFile oZipFile("");

    oZipFile.doCompressFilesBy7zip(pFiles, sTempFolder, sDestFileName);
    return true;
}

/*!
*@brief  构造   
*@author zhaok 2013年9月25日
*@param[out]   
*@param[in]  const GString sPassWord 密码  
*@return   
*/
GMCZipFile::GMCZipFile(const GString sPassWord)
{
    m_pZipFile = createZipFile(sPassWord);
    m_sPassWord = sPassWord;
}

/*!
*@brief  解压缩   
*@author zhaok 2013年9月25日
*@param[out]   
*@param[in] const GString sExtractFile 待解压缩文件
            const GString sDestFolder  目标文件夹
            GStrings* pFiles = NULL     解压列表
*@return    是否成功
*/
bool GMCZipFile::doExtractFiles( const GString sExtractFile, const GString sDestFolder, GStrings* pFiles /*= NULL*/ )
{
    //非法
    if (sExtractFile.isEmpty() || sDestFolder.isEmpty())
    {
        return false;
    }
    if (!isZipFile(sExtractFile))
    {
        return false;
    }
    //格式化输出路径
    if (pFiles)
    {
        pFiles->clear();
    }
    GString sFolder = includeTrailingPathDelimiter(sDestFolder);
    //读取压缩文件
    m_pZipFile->clear();
    m_pZipFile->loadFromFile(sExtractFile);
    //解压缩
    if (m_pZipFile->count() != 0)
    {
        for (int i = 0; i < m_pZipFile->count(); i++)
        {
            IGLDZipFileEntry *pFileEnty = m_pZipFile->items(i);
            try
            {
				QString subFolder = pFileEnty->name();
                pFileEnty->saveToFile(sFolder + subFolder);
            }
            catch (GLDException e)
            {
                pFileEnty->Release();
                return false;
            }

            if (pFiles)
            {
                pFiles->append(sFolder + pFileEnty->name());
            }
            pFileEnty->Release();
        }
    }

    return true;
    
}
/*!
*@brief  压缩  
*@author zhaok 2013年9月25日
*@param[out]   
*@param[in]    
*@return   
*/
bool GMCZipFile::doCompressFiles( GStrings& pFiles, const GString sTempFolder, const GString sDestFileName)
{
    if (sDestFileName.isEmpty() || sTempFolder.isEmpty())
    {
        return false;
    }    
    m_pZipFile->clear();
    //添加指定文件
    for (int i = 0; i < pFiles.size(); i++)
    {
        GString sFileName = pFiles.at(i);

        //撤销回复日志文件不要压缩
        if (sFileName == "ProjectData.GDB.ggdblog")
        {
            continue;
        }
        if (!sFileName.isEmpty() && fileExists(pFiles.at(i)))
        {
            addFile(sFileName);
        }
    }
    m_pZipFile->saveToFile(sDestFileName);
    return true;
}

/*!
*@brief  析构  
*@author zhaok 2013年9月25日
*@param[out]   
*@param[in]    
*@return   
*/
GMCZipFile::~GMCZipFile()
{
    m_pZipFile->Release();
}

/*!
*@brief  添加文件  
*@author zhaok 2013年9月25日
*@param[out]   
*@param[in]    
*@return   
*/
void GMCZipFile::addFile( const GString sFileName )
{
    m_pZipFile->addFromFile(extractFileName(sFileName), sFileName);
}

/*!
*@brief    解压缩指定的文件
*@author liqx 2014年3月27日
*@param[in]    const GString &zipFile 压缩文件
*@param[in]    const GString &specifiedFile 指定的文件
*@param[in]    const GString &destFileName 解压缩后的文件
*@return       bool 是否成功
*/
bool GMCZipFile::uncompressFile(const GString &zipFile, const GString &specifiedFile, const GString &destFileName)
{
    if (zipFile.isEmpty())
    {
        return false;
    }
    GMCZipFile oZipFile("");
	bool bRet = false;

    if (!isZipFile(zipFile))
    {
        bRet = oZipFile.doUncompressBy7zip(zipFile, specifiedFile, destFileName);
    }
    else
    {
        bRet = oZipFile.doUncompressFile(zipFile, specifiedFile, destFileName);
    }

    return bRet;
}

/*!
*@brief    解压缩指定的文件
*@author liqx 2014年3月27日
*@param[in]    const GString &zipFile 压缩文件
*@param[in]    const GString &specifiedFile 指定的文件
*@param[in]    const GString &destFileName 解压缩后的文件
*@return       bool
*/
bool GMCZipFile::doUncompressFile(const GString &zipFile, const GString &specifiedFile, const GString &destFileName)
{
    //非法
    if (zipFile.isEmpty())
    {
        return false;
    }
    if (!isZipFile(zipFile))
    {
        return false;
    }
    //读取压缩文件
    m_pZipFile->clear();
    m_pZipFile->loadFromFile(zipFile);

    if (m_pZipFile->count() == 0)
        return false;

    int nIndex = m_pZipFile->indexOf(specifiedFile);
    if (nIndex < 0)
        return false;

	if (fileExists(destFileName))
		deleteFile(destFileName);

	IGLDZipFileEntry *pFileEnty = m_pZipFile->items(nIndex);
	try
	{
		pFileEnty->saveToFile(destFileName);
	}
	catch (GLDException& e)
	{
		pFileEnty->Release();
		return false;
	}
	pFileEnty->Release();

    return true;
}

bool GMCZipFile::doExtractFilesBy7zip(const GString sExtractFile, const GString sDestFolder, GStrings* pFiles /*= NULL*/)
{
    wchar_t *pSrcFile = const_cast< wchar_t*>(reinterpret_cast <const wchar_t *>(sExtractFile .utf16()));
    wchar_t *pDestFolder = const_cast< wchar_t*>(reinterpret_cast <const wchar_t *>(sDestFolder .utf16()));;
    vector<wchar_t*> pDesFiles;
	bool bRet = C7zManager::ExtractFiles7z(pSrcFile, pDestFolder, nullptr, pDesFiles);
    int nFileNum = pDesFiles.size();
    for(int i = 0; i < nFileNum; i++)
    {
        GString szDestFile = GString::fromStdWString(pDesFiles[i]);
        if (pFiles)
        {
            pFiles->append(szDestFile);
        }
		delete[] (pDesFiles[i]);
        pDesFiles[i] = nullptr;
    }
    pDesFiles.clear();
	return bRet;
}

bool GMCZipFile::doCompressFilesBy7zip(GStrings& pFiles, const GString sTempFolder, const GString sDestFileName)
{  
    //待压缩文件列表
    int nFileCount = pFiles.size();
    vector<wchar_t*> pSrcFiles;
    for (int i = 0; i < nFileCount; i++)
    {
        GString sSrcFile = pFiles.at(i);
        wchar_t* pSrcFile = const_cast< wchar_t*>(reinterpret_cast <const wchar_t *>(sSrcFile .utf16()));
        pSrcFiles.push_back(pSrcFile);
    }
    //目标文件
    wchar_t* pDesFile = const_cast< wchar_t*>(reinterpret_cast <const wchar_t *>(sDestFileName .utf16()));
    bool bRet = C7zManager::CompressFiles7z(pSrcFiles, nullptr, pDesFile);
    //释放资源
    pSrcFiles.clear();
    return bRet;
}

bool GMCZipFile::doUncompressBy7zip(const GString &sZipFile, const GString &sSpecifiedFile, const GString &sDestFileName)
{
    wchar_t* pSrcFile = const_cast< wchar_t*>(reinterpret_cast <const wchar_t *>(sZipFile .utf16()));
    wchar_t* pSpecifiedFile = const_cast< wchar_t*>(reinterpret_cast <const wchar_t *>(sSpecifiedFile .utf16()));
    wchar_t* pDestFileName = const_cast< wchar_t*>(reinterpret_cast <const wchar_t *>(sDestFileName .utf16()));
    bool bRet = C7zManager::ExtractSpecifiedFile7z(pSrcFile, pSpecifiedFile, nullptr, pDestFileName);
    return bRet;
}

/*!
*@brief  压缩文件中的文件列表  
*@author zhaok 2015年11月10日
*@param[out]   
*@param[in] const GString &zipFile      文件
            GStrings* pFiles = NULL     文件列表
*@return    是否成功
*/
bool GMCZipFile::getFileNames( const GString &zipFile, GStrings& pFiles )
{
	//非法
	if (zipFile.isEmpty())
	{
		return false;
	}
    try
    {
        if (isZipFile(zipFile))
        {
            return getZipFileNames(zipFile, pFiles);
        }
        else
        {
            return get7ZipFileNames(zipFile, pFiles);
        }
    }
    catch (GLDException* e)
    {
        return false;
    }
    return true;
}

bool GMCZipFile::getZipFileNames(const GString &zipFile, GStrings& pFiles)
{
    //读取压缩文件
    m_pZipFile->clear();
    m_pZipFile->loadFromFile(zipFile);
    if (m_pZipFile->count() == 0)
        return false;
    pFiles.clear();
    //解压缩	
    for (int i = 0; i < m_pZipFile->count(); i++)
    {
        IGLDZipFileEntry *pFileEnty = m_pZipFile->items(i);
        pFiles.append(pFileEnty->name());
        pFileEnty->Release();
    }
    return true;
}

bool GMCZipFile::get7ZipFileNames(const GString &zipFile, GStrings& pFiles)
{
    wchar_t *pSrcFile = const_cast< wchar_t*>(reinterpret_cast <const wchar_t *>(zipFile .utf16()));
    vector<wchar_t*> pDesFiles;
    /*brief  获取7zip格式的压缩包中的文件列表
    @param[in] pSrcFile 7zip格式压缩包的绝对路径
    @param[in] pPassword 解压密码(如无需密码该项设置为nullptr)
    @param[out] pDesFiles 压缩包中的文件列表
    @return 获取文件列表是否成功
     */
    bool bRet = C7zManager::LoadFilesFrom7z(pSrcFile, nullptr, pDesFiles);
    int nFileNum = pDesFiles.size();
    if (!bRet || nFileNum == 0)
    {
        return false;
    }
    pFiles.clear();
    for(int i = 0; i < nFileNum; i++)
    {
        GString szDestFile = GString::fromStdWString(pDesFiles[i]);
        pFiles.append(szDestFile);
        delete [](pDesFiles[i]);
        pDesFiles[i] = nullptr;
    }
    pDesFiles.clear();
    return true;
}
