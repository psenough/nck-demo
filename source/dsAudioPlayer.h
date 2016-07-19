
/**
* NCK-DEMO SCENE
* https://github.com/nczeroshift/nck-demo
*/

#ifndef DS_AUDIO_PLAYER_H
#define DS_AUDIO_PLAYER_H

#include "dsAudioFFT.h"

_DS_BEGIN

// Stereo, Signed Short Audio Playback with reasonly precise clock and FFT
class AudioPlayer : public virtual Audio::DeviceCallback {
public:
    AudioPlayer(int sampleRate, int bufferSamples, int bufferCount);
    ~AudioPlayer();

    void Start();  // default: 44100, 1024, 4
    void LoadBG(const std::string & filename);

    bool isPlaying();
    void Play();
    void Pause();
    void Seek(int64_t time);
    void Stop();
    int64_t GetDuration();
    int64_t GetTime();
    AudioFFT * GetFFT();

    float GetdB(float f);
    void GetdBArray(float * values, int size);

    int Buffer(Audio::Device * dev, int bufferSize, uint8_t * bufferData);
private:
    void Prime();

    int sampleRate;
    int bufferSamples;
    int bufferCount;
    int channels;

    AudioFFT            * fft;
    uint8_t             * decodeBuffer;
    Core::Mutex         * mutex;
    Core::QueueBuffer   * buffer;
    Audio::OutputDevice * device;
    Audio::Stream       * stream;
    int64_t             firstTimestamp, repeatTimestamp;
};

_DS_END

#endif
