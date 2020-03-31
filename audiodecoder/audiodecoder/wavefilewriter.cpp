#include "wavefilewriter.h"

struct chunk
{
	char id[4];
	quint32 size;
};

struct RIFFHeader
{
	chunk descriptor; // "RIFF"
	char type[4]; //"WAVE"
};

struct WAVEHeader
{
	chunk descriptor;
	quint16 audioFormat;
	quint16 numChannels;
	quint32 sampleRate;
	quint32 byteRate;
	quint16 blockAlign;
	quint16 bitsPerSample;
};

struct DATAHeader
{
	chunk descriptor;
};

struct CombinedHeader
{
	RIFFHeader riff;
	WAVEHeader wave;
	DATAHeader data;
};

static const int HeaderLength = sizeof(CombinedHeader);

WaveFileWriter::WaveFileWriter(QObject *parent)
	: QObject(parent)
	, m_dataLength(0)
{
}

WaveFileWriter::~WaveFileWriter()
{
	close();
}

bool WaveFileWriter::open(const QString &fileName, const QAudioFormat &format)
{
	if (file.isOpen())
		return false;

	if (format.codec() != "audio/pcm" || format.sampleType() != QAudioFormat::SignedInt) {
		return false;
	}

	file.setFileName(fileName);
	if (!file.open(QIODevice::WriteOnly))
		return false;

	if (!writeHeader(format))
		return false;

	m_format = format;
	return true;
}

bool WaveFileWriter::write(const QAudioBuffer &buffer)
{
	if (buffer.format() != m_format)
		return false;

	qint64 written = file.write((const char*)buffer.constData(), buffer.byteCount());
	m_dataLength += written;
	return written == buffer.byteCount();
}


bool WaveFileWriter::close()
{
	bool result = false;
	if (file.isOpen()) {
		Q_ASSERT(m_dataLength < INT_MAX);
		result = writeDataLength();

		m_dataLength = 0;
		file.close();
	}

	return result;
}

bool WaveFileWriter::writeHeader(const QAudioFormat &format)
{
	if (format.byteOrder() == QAudioFormat::BigEndian
		|| format.sampleType() != QAudioFormat::SignedInt)
		return false;

	CombinedHeader header;
	memset(&header, 0, HeaderLength);

#ifndef Q_LITTLE_ENDIAN
	return false;
#else

	memcpy(header.riff.descriptor.id, "RIFF", 4);
	// this will be updated with correct duration:
	// m_dataLength + HeaderLength - 8
	header.riff.descriptor.size = 0;

	memcpy(header.riff.type, "WAVE", 4);
	memcpy(header.wave.descriptor.id, "fmt ", 4);
	header.wave.descriptor.size = quint32(16);
	header.wave.audioFormat = quint16(1);
	header.wave.numChannels = qint16(format.channelCount());
	header.wave.sampleRate = qint32(format.sampleRate());
	header.wave.byteRate = qint32(format.sampleRate()
		*format.channelCount()*format.sampleSize() / 8);
	header.wave.blockAlign = quint16(format.channelCount()*format.sampleSize() / 8);
	header.wave.bitsPerSample = qint16(format.sampleSize());

	memcpy(header.data.descriptor.id, "data", 4);
	// this will be updated with correct data length: m_dataLength
	header.data.descriptor.size = 0;
	return (file.write(reinterpret_cast<const char*>(&header), HeaderLength) == HeaderLength);
#endif
}

bool WaveFileWriter::writeDataLength()
{
#ifndef Q_LITTLE_ENDIAN
	return false;
#endif

	if (file.isSequential())
		return false;

	if (!file.seek(4))
		return false;

	// seek to RIFF header size, see header.riff.descriptor.size above
	quint32 length = m_dataLength + HeaderLength - 8;
	if (file.write(reinterpret_cast<const char*>(&length), 4) != 4)
		return false;

	// seek to DATA header size, see header.data.descriptor.size above
	if (!file.seek(40))
		return false;

	return file.write(reinterpret_cast<const char*>(&m_dataLength), 4) == 4;
}