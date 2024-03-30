#include <Windows.h>
#include <mmsystem.h>
#include <mmreg.h>

#include "4klang.h"


float AUDIO_BUFFER[MAX_SAMPLES * 2];
WAVEHDR waveHdr;

void play_audio() {
	MMRESULT result;

	WAVEFORMATEX wfx;
	wfx.wFormatTag = WAVE_FORMAT_IEEE_FLOAT;
	wfx.nChannels = 2;
	wfx.nSamplesPerSec = SAMPLE_RATE;
	wfx.nAvgBytesPerSec = SAMPLE_RATE * 2 * sizeof(float);
	wfx.nBlockAlign = 2 * sizeof(float);
	wfx.wBitsPerSample = 8 * sizeof(float);
	wfx.cbSize = 0;

	HWAVEOUT hWaveOut;
	result = waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL | WAVE_FORMAT_DIRECT);
	if (result != MMSYSERR_NOERROR) {
		// whoops
		result = result;
	}

	_4klang_render(AUDIO_BUFFER);

	memset(&waveHdr, 0, sizeof(WAVEHDR));
	waveHdr.lpData = reinterpret_cast<char*>(AUDIO_BUFFER);
	waveHdr.dwBufferLength = MAX_SAMPLES * 2 * sizeof(float);

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

	// Now we have to wait normally...
}