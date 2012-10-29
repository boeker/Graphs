#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);
	QTranslator translator;
	translator.load("graphs_" + QLocale::system().name());
	app.installTranslator(&translator);
	
	QTranslator qtTranslator;
	qtTranslator.load("qt_" + QLocale::system().name(),
		QLibraryInfo::location(QLibraryInfo::TranslationsPath));
	app.installTranslator(&qtTranslator);

	MainWindow mw;
	mw.show();

	return app.exec();
}