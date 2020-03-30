#pragma once

#include <QObject>
#include <QAudioBuffer>
#include <QFile>

class WaveFileWriter : public QObject
{
	Q_OBJECT

public:
	explicit WaveFileWriter(QObject *parent = nullptr);
	~WaveFileWriter();

	bool open(const QString &fileName, const QAudioFormat &format);
	bool write(const QAudioBuffer &buffer);
	bool close();
	bool isOpen() const { return file.isOpen(); }

private:
	bool writeHeader(const QAudioFormat &format);
	bool writeDataLength();

	QFile file;
	QAudioFormat m_format;
	qint64 m_dataLength;
};
