#ifndef WIDGET_H
#define WIDGET_H

#include <QtWidgets>
#include <QThread>
#include <QComboBox>
#include <QFileInfo>
#include <QMessageBox>
#include <QFileDialog>
#include <QProgressBar>
#include <QLabel>
#include <QToolButton>
#include <QKeyEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStandardItemModel>
#include <QAbstractItemView>
#include "GLDString.h"

class SearchThread : public QThread
{
    Q_OBJECT
signals:
	void sig_finished(bool);
	void sig_setValue(int value);
	void sig_setMaximum(int maximum);
public:
	QString rootpath;
	bool stopped;
	bool completed;

public:
	SearchThread(const QString& path, QObject* parent);
	
    void stop();
	void changeSubOrder();
    void changeSubOrder2();
	bool find_string_in_file(const QString& fname);
protected:
    void run() override;

private:

	GString getXMLFile(QStringList* strFiles);
	//处理gip类型的文件
	bool handleGIPFile(GString strFile);
	//处理文件夹存放形式规则文件
	bool handleDOCFile(GString strFile);
};

class SearchInComboBox : public QComboBox
{
	Q_OBJECT
public:
	QString path;
	QString external_path;

public:
	SearchInComboBox(QWidget* parent = nullptr);
	void add_external_path(const QString& path);

	void clear_external_paths();
	QString get_current_searchpath() const;

	bool eventFilter(QObject *widget, QEvent *event) override;

public slots:
	void path_selection_changed();
	QString select_directory();
};


class FindOptions : public QWidget
{
	Q_OBJECT
		signals :
	void find();
	void stop();
public:
	QToolButton* ok_button;
	QToolButton* stop_button;

	SearchInComboBox* path_selection_combo;

public:
	FindOptions(QWidget* parent);
	bool get_options(QString& path);

protected:
	void keyPressEvent(QKeyEvent *event) override;
};


class FindInFilesWidget : public QWidget
{
	Q_OBJECT
public:
	SearchThread* search_thread;
	QProgressBar* status_bar;
	FindOptions* find_options;

public:
	FindInFilesWidget(QWidget* parent);
	void stop_and_reset_thread(bool ignore_results = false);
	void closing_widget();

public slots:
	void find();
	void search_complete(bool completed);
};

#endif // WIDGET_H
