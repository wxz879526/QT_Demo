#include "audiodecoder.h"
#include <iostream>

AudioDecoder::AudioDecoder(bool isPlayback, bool isDelete)
	: m_cout(stdout, QIODevice::WriteOnly)
{
	m_isPlayback = isPlayback;
	m_isDelete = isDelete;

	QAudioFormat format;
	format.setChannelCount(2);
	format.setSampleSize(16);
	format.setSampleRate(48000);
	format.setCodec("audio/pcm");
	format.setSampleType(QAudioFormat::SignedInt);
	m_decoder.setAudioFormat(format);

	connect(&m_decoder, &QAudioDecoder::bufferReady, this, &AudioDecoder::bufferReady);
	connect(&m_decoder, QOverload<QAudioDecoder::Error>::of(&QAudioDecoder::error), this, &AudioDecoder::error);
	connect(&m_decoder, &QAudioDecoder::stateChanged, this, &AudioDecoder::stateChanged);
	connect(&m_decoder, &QAudioDecoder::finished, this, &AudioDecoder::finished);
	connect(&m_decoder, &QAudioDecoder::positionChanged, this, &AudioDecoder::updateProgress);
	connect(&m_decoder, &QAudioDecoder::durationChanged, this, &AudioDecoder::updateProgress);

	connect(&m_soundEffect, &QSoundEffect::statusChanged, this, &AudioDecoder::playbackStatusChanged);
	connect(&m_soundEffect, &QSoundEffect::playingChanged, this, &AudioDecoder::playingChanged);

	m_progress = -1.0;
}

AudioDecoder::~AudioDecoder()
{
}


void AudioDecoder::setSourceFileName(const QString &fileName)
{
	m_decoder.setSourceFilename(fileName);
}

void AudioDecoder::start()
{
	m_decoder.start();
}

void AudioDecoder::stop()
{
	m_decoder.stop();
}


void AudioDecoder::setTargetFileName(const QString &fileName)
{
	m_targetFileName = fileName;
}

void AudioDecoder::bufferReady()
{
	QAudioBuffer buffer = m_decoder.read();
	if (!buffer.isValid())
		return;

	if (!m_fileWriter.isOpen() && !m_fileWriter.open(m_targetFileName, buffer.format()))
	{
		m_decoder.stop();
		return;
	}

	m_fileWriter.write(buffer);
}

void AudioDecoder::error(QAudioDecoder::Error error)
{
	switch (error)
	{
	case QAudioDecoder::NoError:
		return;
	case QAudioDecoder::ResourceError:
		m_cout << "Resource Error" << endl;
		break;
	case QAudioDecoder::FormatError:
		m_cout << "Format Error" << endl;
		break;
	case QAudioDecoder::AccessDeniedError:
		m_cout << "Access DeniedError" << endl;
		break;
	case QAudioDecoder::ServiceMissingError:
		m_cout << "ServiceMissing Error" << endl;
		break;
	}

	emit done();
}

void AudioDecoder::stateChanged(QAudioDecoder::State newState)
{
	switch (newState) {
	case QAudioDecoder::DecodingState:
		m_cout << "Decoding..." << endl;
		break;
	case QAudioDecoder::StoppedState:
		m_cout << "Decoding Stopped" << endl;
		break;
	}
}

void AudioDecoder::finished()
{
	if (!m_fileWriter.close())
		m_cout << "Failed to finilize output file" << endl;

	m_cout << "Decoding finished" << endl;

	if (m_isPlayback) {
		m_cout << "Starting playback" << endl;
		m_soundEffect.setSource(QUrl::fromLocalFile(m_targetFileName));
		m_soundEffect.play();
	}
	else {
		emit done();
	}
}


void AudioDecoder::playbackStatusChanged()
{
	if (m_soundEffect.status() == QSoundEffect::Error) {
		m_cout << "Playback error" << endl;
		emit done();
	}
}

void AudioDecoder::playingChanged()
{
	if (!m_soundEffect.isPlaying()) {
		m_cout << "Playback finished" << endl;
		if (m_isDelete)
			QFile::remove(m_targetFileName);

		emit done();
	}
}

void AudioDecoder::updateProgress()
{
	qint64 position = m_decoder.position();
	qint64 duration = m_decoder.duration();
	qreal progress = m_progress;
	if (position >= 0 && duration > 0)
		progress = position / (qreal)duration;

	if (progress > m_progress + 0.1) {
		m_cout << "Decoding progress: " << (int)(progress * 100.0) << "%" << endl;
		m_progress = progress;
	}
}
