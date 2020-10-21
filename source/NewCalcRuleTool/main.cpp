#include "widget.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	FindInFilesWidget* widget = new FindInFilesWidget(nullptr);
    widget->show();

    return a.exec();
}
