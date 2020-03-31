#include <QtCore/QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QTextStream>
#include "audiodecoder.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	QTextStream cout(stdout, QIODevice::WriteOnly);
	if (a.arguments().size() < 2) {
		cout << "Usage: audiodecoder [-p] [-pd] SOURCEFILE [TARGETFILE]" << endl;
		cout << "Set -p option if you want to play output file." << endl;
		cout << "Set -pd option if you want to play output file and delete it after successful playback." << endl;
		cout << "Default TARGETFILE name is \"out.wav\" in the same directory as the source file." << endl;
		return 0;
	}

	bool isPlayback = false;
	bool isDelete = false;

	if (a.arguments().at(1) == "-p") {
		isPlayback = true;
	}
	else if (a.arguments().at(1) == "-pd") {
		isPlayback = true;
		isDelete = true;
	}

	QFileInfo sourceFile;
	QFileInfo targetFile;

	int sourceFileIndex = (isPlayback || isDelete) ? 2 : 1;
	if (a.arguments().size() <= sourceFileIndex) {
		cout << "Error: source filename is not specified." << endl;
		return 0;
	}

	sourceFile.setFile(a.arguments().at(sourceFileIndex));

	if (a.arguments().size() > sourceFileIndex + 1)
		targetFile.setFile(a.arguments().at(sourceFileIndex + 1));
	else
		targetFile.setFile(sourceFile.dir().absoluteFilePath("out.wav"));

	AudioDecoder decoder(isPlayback, isDelete);
	QObject::connect(&decoder, &AudioDecoder::done, &a, &QCoreApplication::quit);
	decoder.setSourceFileName(sourceFile.absoluteFilePath());
	decoder.setTargetFileName(targetFile.absoluteFilePath());
	decoder.start();

	return a.exec();
}
