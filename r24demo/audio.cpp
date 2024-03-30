#include <Windows.h>
#include <mmsystem.h>
#include <mmreg.h>

WAVEHDR waveHdr;

void play_audio(void *buffer, unsigned size) {
	MMRESULT result;

	WAVEFORMATEX wfx;
	wfx.wFormatTag = WAVE_FORMAT_IEEE_FLOAT;
	wfx.nChannels = 2;
	wfx.nSamplesPerSec = 44100;
	wfx.nAvgBytesPerSec = 44100 * 2 * sizeof(float);
	wfx.nBlockAlign = 2 * sizeof(float);
	wfx.wBitsPerSample = 8 * sizeof(float);
	wfx.cbSize = 0;

	HWAVEOUT hWaveOut;
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

	// Now we have to wait normally...
}