
#include <QtCore>
#include <QtWidgets>

#include <MyWidget.h>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);


	QJsonValue v = true;

	qDebug() << v.toString();

	MyWidget w;

	w.show();

	return a.exec();
}
