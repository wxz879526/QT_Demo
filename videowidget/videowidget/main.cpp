#include <QtWidgets/QApplication>
#include <QtWidgets/QDesktopWidget>
#include <QtCore/QCommandLineOption>
#include <QtCore/QCommandLineParser>
#include <QtCore/QDir>
#include "videoplayer.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	
	QCoreApplication::setApplicationName("Video Widget Example");
	QCoreApplication::setOrganizationName("QtProject");
	QGuiApplication::setApplicationDisplayName(QCoreApplication::applicationName());

	QCommandLineParser parser;
	parser.setApplicationDescription("Qt Video Widget Example");
	parser.addHelpOption();
	parser.addVersionOption();
	parser.addPositionalArgument("url", "The URL to Open");
	parser.process(a);

	VideoPlayer player;
	if (!parser.positionalArguments().isEmpty()) {
		const QUrl url = QUrl::fromUserInput(parser.positionalArguments().constFirst(),
			QDir::currentPath(), QUrl::AssumeLocalFile);
		player.setUrl(url);
	}

	const QRect availableGeometry = QApplication::desktop()->availableGeometry(&player);
	player.resize(availableGeometry.width() / 6, availableGeometry.height() / 4);
	player.show();

	return a.exec();
}
