#include <cassert>

#include <Windows.h>
#include <mmreg.h>

#include "audio.h"

WAVEHDR waveHdr;

Playback play_audio(void *buffer, unsigned size) {
	MMRESULT result;

	WAVEFORMATEX wfx;
	wfx.wFormatTag = WAVE_FORMAT_IEEE_FLOAT;
	wfx.nChannels = 2;
	wfx.nSamplesPerSec = 44100;
	wfx.nAvgBytesPerSec = 44100 * 2 * sizeof(float);
	wfx.nBlockAlign = 2 * sizeof(float);
	wfx.wBitsPerSample = 8 * sizeof(float);
	wfx.cbSize = 0;

	HWAVEOUT hWaveOut{ 0 };
	result = waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL | WAVE_FORMAT_DIRECT);
	if (result != MMSYSERR_NOERROR) {
		// whoops
		result = result;
	}

	memset(&waveHdr, 0, sizeof(WAVEHDR));
	waveHdr.lpData = reinterpret_cast<char*>(buffer);
	waveHdr.dwBufferLength = size;

	result = waveOutPrepareHeader(hWaveOut, &waveHdr, sizeof(WAVEHDR));
	if (result != MMSYSERR_NOERROR) {
		// whoops
		result = result;
	}
	result = waveOutWrite(hWaveOut, &waveHdr, sizeof(WAVEHDR));
	if (result != MMSYSERR_NOERROR) {
		// whoops
		result = result;
	}

	return Playback(hWaveOut);
}


float Playback::get_progress() const {
	MMTIME time{ 0 };
	waveOutGetPosition(m_hWaveOut, &time, sizeof(time));

	assert(time.wType ==  TIME_BYTES);
	return float(time.u.cb / float(44100 * 2 * sizeof(float)));
}
