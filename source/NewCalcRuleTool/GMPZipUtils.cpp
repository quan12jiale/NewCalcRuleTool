
/*!
*@brief  ѹ���㷨  
*@author zhaok 2013��9��25��
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
*@brief  ��ѹ��  
*@author zhaok 2013��9��25��
*@param[out]   
*@param[in] const GString sExtractFile
            const GString sDestFolder
            GStrings* pFiles = NULL 
*@return �ɹ�/ʧ��  
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
*@brief  ѹ��  
*@author zhaok 2013��9��25��
*@param[out]   
*@param[in] GStrings& pFiles  ѹ���ļ��б�
            const GString sTempFolder �ļ���
            const GString sDestFileName Ŀ���ļ�
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
*@brief  ����   
*@author zhaok 2013��9��25��
*@param[out]   
*@param[in]  const GString sPassWord ����  
*@return   
*/
GMCZipFile::GMCZipFile(const GString sPassWord)
{
    m_pZipFile = createZipFile(sPassWord);
    m_sPassWord = sPassWord;
}

/*!
*@brief  ��ѹ��   
*@author zhaok 2013��9��25��
*@param[out]   
*@param[in] const GString sExtractFile ����ѹ���ļ�
            const GString sDestFolder  Ŀ���ļ���
            GStrings* pFiles = NULL     ��ѹ�б�
*@return    �Ƿ�ɹ�
*/
bool GMCZipFile::doExtractFiles( const GString sExtractFile, const GString sDestFolder, GStrings* pFiles /*= NULL*/ )
{
    //�Ƿ�
    if (sExtractFile.isEmpty() || sDestFolder.isEmpty())
    {
        return false;
    }
    if (!isZipFile(sExtractFile))
    {
        return false;
    }
    //��ʽ�����·��
    if (pFiles)
    {
        pFiles->clear();
    }
    GString sFolder = includeTrailingPathDelimiter(sDestFolder);
    //��ȡѹ���ļ�
    m_pZipFile->clear();
    m_pZipFile->loadFromFile(sExtractFile);
    //��ѹ��
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
*@brief  ѹ��  
*@author zhaok 2013��9��25��
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
    //���ָ���ļ�
    for (int i = 0; i < pFiles.size(); i++)
    {
        GString sFileName = pFiles.at(i);

        //�����ظ���־�ļ���Ҫѹ��
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
*@brief  ����  
*@author zhaok 2013��9��25��
*@param[out]   
*@param[in]    
*@return   
*/
GMCZipFile::~GMCZipFile()
{
    m_pZipFile->Release();
}

/*!
*@brief  ����ļ�  
*@author zhaok 2013��9��25��
*@param[out]   
*@param[in]    
*@return   
*/
void GMCZipFile::addFile( const GString sFileName )
{
    m_pZipFile->addFromFile(extractFileName(sFileName), sFileName);
}

/*!
*@brief    ��ѹ��ָ�����ļ�
*@author liqx 2014��3��27��
*@param[in]    const GString &zipFile ѹ���ļ�
*@param[in]    const GString &specifiedFile ָ�����ļ�
*@param[in]    const GString &destFileName ��ѹ������ļ�
*@return       bool �Ƿ�ɹ�
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
*@brief    ��ѹ��ָ�����ļ�
*@author liqx 2014��3��27��
*@param[in]    const GString &zipFile ѹ���ļ�
*@param[in]    const GString &specifiedFile ָ�����ļ�
*@param[in]    const GString &destFileName ��ѹ������ļ�
*@return       bool
*/
bool GMCZipFile::doUncompressFile(const GString &zipFile, const GString &specifiedFile, const GString &destFileName)
{
    //�Ƿ�
    if (zipFile.isEmpty())
    {
        return false;
    }
    if (!isZipFile(zipFile))
    {
        return false;
    }
    //��ȡѹ���ļ�
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
    //��ѹ���ļ��б�
    int nFileCount = pFiles.size();
    vector<wchar_t*> pSrcFiles;
    for (int i = 0; i < nFileCount; i++)
    {
        GString sSrcFile = pFiles.at(i);
        wchar_t* pSrcFile = const_cast< wchar_t*>(reinterpret_cast <const wchar_t *>(sSrcFile .utf16()));
        pSrcFiles.push_back(pSrcFile);
    }
    //Ŀ���ļ�
    wchar_t* pDesFile = const_cast< wchar_t*>(reinterpret_cast <const wchar_t *>(sDestFileName .utf16()));
    bool bRet = C7zManager::CompressFiles7z(pSrcFiles, nullptr, pDesFile);
    //�ͷ���Դ
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
*@brief  ѹ���ļ��е��ļ��б�  
*@author zhaok 2015��11��10��
*@param[out]   
*@param[in] const GString &zipFile      �ļ�
            GStrings* pFiles = NULL     �ļ��б�
*@return    �Ƿ�ɹ�
*/
bool GMCZipFile::getFileNames( const GString &zipFile, GStrings& pFiles )
{
	//�Ƿ�
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
    //��ȡѹ���ļ�
    m_pZipFile->clear();
    m_pZipFile->loadFromFile(zipFile);
    if (m_pZipFile->count() == 0)
        return false;
    pFiles.clear();
    //��ѹ��	
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
    /*brief  ��ȡ7zip��ʽ��ѹ�����е��ļ��б�
    @param[in] pSrcFile 7zip��ʽѹ�����ľ���·��
    @param[in] pPassword ��ѹ����(�����������������Ϊnullptr)
    @param[out] pDesFiles ѹ�����е��ļ��б�
    @return ��ȡ�ļ��б��Ƿ�ɹ�
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
