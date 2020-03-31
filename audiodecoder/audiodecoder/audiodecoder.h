#pragma once

#include <QObject>
#include <QSoundEffect>
#include <QTextStream>
#include <QAudioDecoder>
#include "wavefilewriter.h"

class AudioDecoder : public QObject
{
	Q_OBJECT

public:
	explicit AudioDecoder(bool isPlayback, bool isDelete);
	~AudioDecoder();

	void setSourceFileName(const QString &fileName);
	void start();
	void stop();

	void setTargetFileName(const QString &fileName);

signals:
	void done();

public slots:
	void bufferReady();
	void error(QAudioDecoder::Error error);
	void stateChanged(QAudioDecoder::State newState);
	void finished();

	void playbackStatusChanged();
	void playingChanged();

private slots:
	void updateProgress();

private:
	bool m_isPlayback;
	bool m_isDelete;
	QAudioDecoder m_decoder;
	QTextStream m_cout;

	QString m_targetFileName;
	WaveFileWriter m_fileWriter;
	QSoundEffect m_soundEffect;

	qreal m_progress;
};
