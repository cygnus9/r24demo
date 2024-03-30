#pragma once

#include <mmsystem.h>

class Playback {
public:
    Playback(HWAVEOUT hWaveOut): m_hWaveOut(hWaveOut) {}
    float get_progress() const;

private:
    HWAVEOUT m_hWaveOut;
};

Playback play_audio(void* buffer, unsigned size);
