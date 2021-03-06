#include "widget.h"
#include "GMPZipUtils.h"
#include "GSPEngineUtils.h"
#include "GLDFileUtils.h"
#include "GSP\GSPKeyLib.h"


/*************************************************************************/
// 数据库定义
static const GString gsdbBusiness = "Business";

// 表定义
static const GString ptnShowCostItem = "ShowCostItem";
static const GString ptnBranchDataSetting = "BranchDataSetting";

// 字段定义
static const GString pfnReportID = "ReportID";
static const GString pfnUnitPrice = "UnitPrice";
static const GString pfnValence = "Valence";
static const GString pfnLabourCostsUnit = "LabourCostsUnit";
static const GString pfnLabourCostsValence = "LabourCostsValence";
static const GString pfnMaterialsCostUnit = "MaterialsCostUnit";
static const GString pfnMaterialsCostValence = "MaterialsCostValence";
static const GString pfnMachineryCostsUnit = "MachineryCostsUnit";
static const GString pfnMachineryCostsValence = "MachineryCostsValence";
static const GString pfnComprehensiveUnit = "ComprehensiveUnit";
static const GString pfnComprehensiveValence = "ComprehensiveValence";
static const GString pfnRemark = "Remark";

static const GString pfnShowGrade = "ShowGrade";
static const GString pfnShowChapter = "ShowChapter";
static const GString pfnShowPart = "ShowPart";
static const GString pfnDeleteBranch = "DeleteBranch";
static const GString pfnSectionCode = "SectionCode";
static const GString pfnBranchNorm = "BranchNorm";
static const GString pfnReportName = "ReportName";
/***************************************************************************/

const int CWD = 0;

const int SELECT_OTHER = 2;
const int CLEAR_LIST = 3;

const int EXTERNAL_PATHS = 5;

const int MAX_PATH_LENGTH = 60;
const int MAX_PATH_HISTORY = 15;

const int widgetheight = 35;


void walk(const QString& absolute_path, QStringList& list)
{
    QDirIterator it(absolute_path, QStringList() << "RegionRule_Calc.GSP", QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        list.append(it.next());
    }
}

void copyRule()
{
    QDirIterator it("F:\\规则库\\Build\\OutputGIP", QDir::Files, QDirIterator::Subdirectories);
    QString dir = "F:\\规则库\\Build\\SetupMaker\\RegionRuleInstallProgram\\Disk\\Files\\";
    while (it.hasNext()) {
        it.next();
        QFileInfo fileInfo = it.fileInfo();
        QString filePath = dir + fileInfo.fileName();
        bool ok = QFile::copy(fileInfo.filePath(), filePath);
        qDebug() << ok;
        if (!ok) {
            qDebug() << fileInfo.filePath();
        }
    }
    // vs运行这个函数有问题的话，就用qt creator
}


/********** SearchThread **********/
static QMutex mutex;

SearchThread::SearchThread(const QString& path, QObject *parent)
	: QThread(parent)
{
	this->rootpath = path;

	this->stopped = false;
	this->completed = false;
}

void SearchThread::run()
{
	changeSubOrder2();

	this->stop();
	emit sig_finished(this->completed);
}

void SearchThread::stop()
{
	QMutexLocker locker(&mutex);
	this->stopped = true;
}

bool SearchThread::find_string_in_file(const QString& fname)
{
	GSPModel oGSPModel = gspEngine().createModel();
	GSPModelPersistent modelPersistent(oGSPModel);
	modelPersistent.loadFromFile(fname);
	oGSPModel.setMode(gmRuntime);

	GSPTable tableShowCostItem = oGSPModel.findTable(gsdbBusiness, ptnShowCostItem);
	if (nullptr == tableShowCostItem)
	{
		return false;
	}

	GSPTable tableBranchDataSetting = oGSPModel.findTable(gsdbBusiness, ptnBranchDataSetting);
	if (nullptr == tableBranchDataSetting)
	{
		return false;
	}

    for (int i = 0; i < tableShowCostItem.recordCount(); ++i)
    {
        GSPRecord oRecord = tableShowCostItem.records(i);
        GString strReportName = oRecord.asWideString(pfnRemark);
        if (strReportName == QString::fromStdWString(L"施工段清单汇总表"))
        {
            return false;
        };
    }

	// 写入显示费用项
	{
		GSPRecord ptrRecord = tableShowCostItem.newRecord();
		ptrRecord.setAsInteger(pfnReportID, 62);
		ptrRecord.setAsWideString(pfnRemark, QString::fromStdWString(L"施工段清单汇总表"));
		ptrRecord.setAsInteger(pfnUnitPrice, 2);//0：非选中，1：选中，2：无
		ptrRecord.setAsInteger(pfnValence, 2);
		ptrRecord.setAsInteger(pfnLabourCostsUnit, 0);//默认值，非选中
		ptrRecord.setAsInteger(pfnLabourCostsValence, 0);
		ptrRecord.setAsInteger(pfnMaterialsCostUnit, 0);
		ptrRecord.setAsInteger(pfnMaterialsCostValence, 0);
		ptrRecord.setAsInteger(pfnMachineryCostsUnit, 0);
		ptrRecord.setAsInteger(pfnMachineryCostsValence, 0);
		ptrRecord.setAsInteger(pfnComprehensiveUnit, 0);
		ptrRecord.setAsInteger(pfnComprehensiveValence, 0);
		tableShowCostItem.append(ptrRecord);
	}

	{
		GSPRecord ptrRecord = tableShowCostItem.newRecord();
		ptrRecord.setAsInteger(pfnReportID, 63);//报表ID
		ptrRecord.setAsWideString(pfnRemark, QString::fromStdWString(L"施工段清单定额汇总表"));
		ptrRecord.setAsInteger(pfnUnitPrice, 0);//默认值，非选中
		ptrRecord.setAsInteger(pfnValence, 0);
		ptrRecord.setAsInteger(pfnLabourCostsUnit, 0);//默认值，非选中
		ptrRecord.setAsInteger(pfnLabourCostsValence, 0);
		ptrRecord.setAsInteger(pfnMaterialsCostUnit, 0);
		ptrRecord.setAsInteger(pfnMaterialsCostValence, 0);
		ptrRecord.setAsInteger(pfnMachineryCostsUnit, 0);
		ptrRecord.setAsInteger(pfnMachineryCostsValence, 0);
		ptrRecord.setAsInteger(pfnComprehensiveUnit, 0);
		ptrRecord.setAsInteger(pfnComprehensiveValence, 0);
		tableShowCostItem.append(ptrRecord);
	}

	{
		GSPRecord ptrRecord = tableShowCostItem.newRecord();
		ptrRecord.setAsInteger(pfnReportID, 64);//报表ID
		ptrRecord.setAsWideString(pfnRemark, QString::fromStdWString(L"施工段定额汇总表"));
		ptrRecord.setAsInteger(pfnUnitPrice, 0);
		ptrRecord.setAsInteger(pfnValence, 0);
		ptrRecord.setAsInteger(pfnLabourCostsUnit, 0);//默认值，非选中
		ptrRecord.setAsInteger(pfnLabourCostsValence, 0);
		ptrRecord.setAsInteger(pfnMaterialsCostUnit, 0);
		ptrRecord.setAsInteger(pfnMaterialsCostValence, 0);
		ptrRecord.setAsInteger(pfnMachineryCostsUnit, 0);
		ptrRecord.setAsInteger(pfnMachineryCostsValence, 0);
		ptrRecord.setAsInteger(pfnComprehensiveUnit, 2);
		ptrRecord.setAsInteger(pfnComprehensiveValence, 2);
		tableShowCostItem.append(ptrRecord);
	}

	// 写入分部整理设置
	{
		GSPRecord ptrRecord = tableBranchDataSetting.newRecord();
		ptrRecord.setAsInteger(pfnReportID, 62);
		ptrRecord.setAsBoolean(pfnShowGrade, false);
		ptrRecord.setAsBoolean(pfnShowChapter, false);
		ptrRecord.setAsBoolean(pfnShowPart, false);
		ptrRecord.setAsBoolean(pfnDeleteBranch, true);
		ptrRecord.setAsBoolean(pfnSectionCode, true);
		ptrRecord.setAsBoolean(pfnBranchNorm, false);
		ptrRecord.setAsWideString(pfnReportName, QString::fromStdWString(L"施工段清单汇总表"));
		tableBranchDataSetting.append(ptrRecord);
	}

	{
		GSPRecord ptrRecord = tableBranchDataSetting.newRecord();
		ptrRecord.setAsInteger(pfnReportID, 63);
		ptrRecord.setAsBoolean(pfnShowGrade, false);
		ptrRecord.setAsBoolean(pfnShowChapter, false);
		ptrRecord.setAsBoolean(pfnShowPart, false);
		ptrRecord.setAsBoolean(pfnDeleteBranch, true);
		ptrRecord.setAsBoolean(pfnSectionCode, true);
		ptrRecord.setAsBoolean(pfnBranchNorm, false);
		ptrRecord.setAsWideString(pfnReportName, QString::fromStdWString(L"施工段清单定额汇总表"));
		tableBranchDataSetting.append(ptrRecord);
	}

	{
		GSPRecord ptrRecord = tableBranchDataSetting.newRecord();
		ptrRecord.setAsInteger(pfnReportID, 64);
		ptrRecord.setAsBoolean(pfnShowGrade, false);
		ptrRecord.setAsBoolean(pfnShowChapter, false);
		ptrRecord.setAsBoolean(pfnShowPart, false);
		ptrRecord.setAsBoolean(pfnDeleteBranch, true);
		ptrRecord.setAsBoolean(pfnSectionCode, true);
		ptrRecord.setAsBoolean(pfnBranchNorm, true);
		ptrRecord.setAsWideString(pfnReportName, QString::fromStdWString(L"施工段定额汇总表"));
		tableBranchDataSetting.append(ptrRecord);
	}

	GSPModelPersistent(oGSPModel).saveToFile(fname);
	completed = true;
	return true;
}

/*********************************************/
void SearchThread::changeSubOrder()
{
	bool bSuccess = false;

	GString strPath = this->rootpath;
	if (strPath.isEmpty())
	{
		QMessageBox::about(NULL, "Prompt", QStringLiteral("请选择规则库路径"));
		return;
	}
	GStringList oFileList, oNameFilter;
	oNameFilter << "";
	getFiles(strPath, oNameFilter, oFileList, true);
	int maximum = oFileList.count();
	emit sig_setMaximum(maximum);

	for (int i = 0; i < maximum; ++i)
	{
		{
			QMutexLocker locker(&mutex);
			if (stopped)
				return;
		}
		emit sig_setValue(i);

		GString strFile = strPath + "\\" + oFileList[i];
		if (strFile.endsWith("CalcRulePermission.gip"))
		{
			continue;
		}
		if (sameStr(extractFileExt(oFileList[i]), ".gip"))
		{
			bSuccess = handleGIPFile(strFile);
		}
		else
		{
			bSuccess = handleDOCFile(strFile);
		}
	}
}

void SearchThread::changeSubOrder2()
{
    QStringList strList;
    walk(this->rootpath, strList);
    int maximum = strList.count();
    emit sig_setMaximum(maximum);

    for (int i = 0; i < maximum; ++i)
    {
        {
            QMutexLocker locker(&mutex);
            if (stopped)
                return;
        }
        emit sig_setValue(i);
        QString strPath = strList[i];
        find_string_in_file(strPath);
    }
}

GString SearchThread::getXMLFile(GStringList* strFiles)
{
	GString strResult = "";
	for (int i = 0; i < strFiles->count(); ++i)
	{
		GString strTemp = extractFileExt(strFiles->at(i));
		if (sameStr(strTemp, ".xml"))
		{
			strResult = strFiles->at(i);
			break;
		}
	}
	return strResult;
}

bool SearchThread::handleGIPFile(GString strFile)
{
	GString strPath = this->rootpath;
	GString strTempFolder = strPath + "\\TempFolder\\";
	createDir(strTempFolder);
	GStringList* pFiles = new GStringList();
	bool bResult = GMCZipFile::extractFiles(strFile, strTempFolder, pFiles);

	if (bResult)
	{
		GString strGSPFile = strTempFolder + "MDB\\RegionRule_Calc.GSP";
		bResult = find_string_in_file(strGSPFile);

		deleteFile(strFile);
		GStringList oFiles;
		oFiles << getXMLFile(pFiles);//直接压回去.gip里面就没有文件夹了，这里只能这么干了
		oFiles << strTempFolder + "MDB";
		bResult = GMCZipFile::compressFiles(oFiles, strPath, strFile);
	}
	delete pFiles;
	deleteTree(strTempFolder);

	return bResult;
}

bool SearchThread::handleDOCFile(GString strFile)
{
	bool bResult = false;

	GString strGSP = strFile + "\\RegionRule_Calc.GSP";
	if (fileExists(strGSP))//如果能直接找到GSP,打开并修改
	{
		bResult = find_string_in_file(strGSP);
	}
	else//否则继续嵌套寻找文件夹
	{
		GStringList oFileList, oNameFilter;
		oNameFilter << "";
		getFiles(strFile, oNameFilter, oFileList, true);
		for (int i = 0; i < oFileList.count(); ++i)
		{
			if (bResult = handleDOCFile(strFile + "\\" + oFileList[i]))
				break;
		}
	}
	return bResult;
}
/********************************************/



/********** SearchInComboBox **********/
SearchInComboBox::SearchInComboBox(QWidget* parent)
	: QComboBox(parent)
{
	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	this->setToolTip("Select directory");
	this->setEditable(false);
	// 设置QComboBox下拉框子项高度
	setStyleSheet("QComboBox { min-height: 40px; min-width: 60px; }"
		"QComboBox QAbstractItemView::item { min-height: 40px; min-width: 60px; }");
	setView(new QListView());

	this->path = "";

	this->addItem("Current working directory");
	QString ttip = "Search in all files and directories present on the current path";
	this->setItemData(0, ttip, Qt::ToolTipRole);

	this->insertSeparator(1);

	this->addItem("Select other directory");
	ttip = "Search in other folder present on the file system";
	this->setItemData(2, ttip, Qt::ToolTipRole);

	this->addItem("Clear this list");
	ttip = "Clear the list of other directories";
	this->setItemData(3, ttip, Qt::ToolTipRole);

	this->insertSeparator(4);

	connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(path_selection_changed()));
	this->view()->installEventFilter(this);
}

void SearchInComboBox::add_external_path(const QString &path)
{
	if (!QFileInfo::exists(path))
		return;
	this->removeItem(this->findText(path));
	this->addItem(path);
	this->setItemData(this->count() - 1, path, Qt::ToolTipRole);
	while (this->count() > MAX_PATH_HISTORY + EXTERNAL_PATHS)
		this->removeItem(EXTERNAL_PATHS);
}

void SearchInComboBox::clear_external_paths()
{
	while (this->count() > EXTERNAL_PATHS)
		this->removeItem(EXTERNAL_PATHS);
}

QString SearchInComboBox::get_current_searchpath() const
{
	int idx = this->currentIndex();
	if (idx >= EXTERNAL_PATHS)
		return external_path;
	else
		return QString();
}

void SearchInComboBox::path_selection_changed()
{
	int idx = this->currentIndex();
	if (idx == SELECT_OTHER) {
		QString external_path = this->select_directory();
		if (external_path.size() > 0) {
			this->add_external_path(external_path);
			this->setCurrentIndex(this->count() - 1);
		}
		else
			this->setCurrentIndex(CWD);
	}
	else if (idx == CLEAR_LIST) {
		QMessageBox::StandardButton reply = QMessageBox::question(this, "Clear other directories",
			"Do you want to clear the list of other directories?",
			QMessageBox::Yes | QMessageBox::No);
		if (reply == QMessageBox::Yes)
			this->clear_external_paths();
		this->setCurrentIndex(CWD);
	}
	else if (idx >= EXTERNAL_PATHS)
		this->external_path = this->itemText(idx);
}

QString SearchInComboBox::select_directory()
{
	QString directory = QFileDialog::getExistingDirectory(this, "Select directory", this->path);
	//getExistingDirectory()函数返回绝对路径
	return directory;
}

bool SearchInComboBox::eventFilter(QObject *widget, QEvent *event)
{
	QKeyEvent* keyevent = dynamic_cast<QKeyEvent*>(event);
	if (event->type() == QEvent::KeyPress && keyevent->key() == Qt::Key_Delete) {
		int index = this->view()->currentIndex().row();
		if (index >= EXTERNAL_PATHS) {
			this->removeItem(index);
			this->showPopup();
			int new_index = qMin(this->count() - 1, index);
			if (new_index < EXTERNAL_PATHS)
				new_index = 0;
			this->view()->setCurrentIndex(model()->index(new_index, 0));
			this->setCurrentIndex(new_index);
		}
		return true;
	}
	return QComboBox::eventFilter(widget, event);
}


/********** FindOptions **********/
FindOptions::FindOptions(QWidget* parent)
	: QWidget(parent)
{
	QHBoxLayout* hlayout1 = new QHBoxLayout;
	ok_button = new QToolButton(this);
	ok_button->setText("Start"); 
	connect(ok_button, SIGNAL(clicked()), this, SIGNAL(find()));
	ok_button->setToolTip("Start search");
	ok_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	ok_button->setFixedHeight(widgetheight);

	stop_button = new QToolButton(this);
	stop_button->setText("Stop");
	connect(stop_button, SIGNAL(clicked()), this, SIGNAL(stop()));
	stop_button->setToolTip("Stop search");
	stop_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	stop_button->setEnabled(false);
	stop_button->setFixedHeight(widgetheight);

	hlayout1->addWidget(this->ok_button);
	hlayout1->addWidget(this->stop_button);

	QHBoxLayout* hlayout2 = new QHBoxLayout;
	QLabel* search_on_label = new QLabel("Search in:");
	path_selection_combo = new SearchInComboBox(parent);

	hlayout2->addWidget(search_on_label);
	hlayout2->addWidget(path_selection_combo);

	QVBoxLayout* vlayout = new QVBoxLayout;
	vlayout->addLayout(hlayout1);
	vlayout->addLayout(hlayout2);
	setLayout(vlayout);

	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
}

bool FindOptions::get_options(QString &path)
{
	path = path_selection_combo->get_current_searchpath();
	return !path.isEmpty();
}

void FindOptions::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
		emit this->find();
	else
		QWidget::keyPressEvent(event);
}


/********** FindInFilesWidget **********/
FindInFilesWidget::FindInFilesWidget(QWidget* parent)
	: QWidget(parent)
{
//     initGSPKey(gspEngine());
//     GSPModel oGSPModel = gspEngine().createModel();
//     GSPModelPersistent modelPersistent(oGSPModel);

	setWindowTitle("Find in files");
	this->search_thread = nullptr;
	status_bar = new QProgressBar(this);
	status_bar->hide();

	find_options = new FindOptions(this);
	connect(find_options, SIGNAL(find()), this, SLOT(find()));
	connect(find_options, &FindOptions::stop,
		[&]() {this->stop_and_reset_thread(); });

	QVBoxLayout* layout = new QVBoxLayout;
	layout->addWidget(find_options);
	layout->addStretch();
	layout->addWidget(status_bar);
	setLayout(layout);
}

void FindInFilesWidget::find()
{
	QString path;
	if (!find_options->get_options(path))
		return;
	stop_and_reset_thread(true);
	search_thread = new SearchThread(path, this);
	connect(search_thread, SIGNAL(sig_finished(bool)), this, SLOT(search_complete(bool)));
	connect(search_thread, SIGNAL(sig_setValue(int)), status_bar, SLOT(setValue(int)));
	connect(search_thread, SIGNAL(sig_setMaximum(int)), status_bar, SLOT(setMaximum(int)));

	status_bar->reset();
	search_thread->start();
	find_options->ok_button->setEnabled(false);
	find_options->stop_button->setEnabled(true);
	status_bar->show();
}

void FindInFilesWidget::stop_and_reset_thread(bool ignore_results)
{
	if (this->search_thread != nullptr) {
		if (this->search_thread->isRunning()) {
			if (ignore_results) {
				disconnect(search_thread, SIGNAL(sig_finished(bool)),
					this, SLOT(search_complete(bool)));
			}
			search_thread->stop();
			search_thread->wait();
		}
		search_thread->setParent(nullptr);
		search_thread->deleteLater();
		search_thread = nullptr;
	}
}

void FindInFilesWidget::closing_widget()
{
	stop_and_reset_thread(true);
}

void FindInFilesWidget::search_complete(bool completed)
{
	Q_UNUSED(completed);
	find_options->ok_button->setEnabled(true);
	find_options->stop_button->setEnabled(false);
	status_bar->hide();
	if (!search_thread)
		return;
	stop_and_reset_thread();
}
