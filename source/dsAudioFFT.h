
/**
* NCK-DEMO SCENE
* https://github.com/nczeroshift/nck-demo
*/

#ifndef DS_AUDIO_FFT_H
#define DS_AUDIO_FFT_H

#include "dsConfig.h"

_DS_BEGIN

class AudioFFT 
{
public:
    AudioFFT(int size, int sampleRate, int channels, int formatSize);
    ~AudioFFT();

    void AddSamples(uint8_t * data, uint32_t size);
    float GetdB(float f);

    float GetMax_dB();
    float GetMin_dB();

    void setMax_dB(float val);
    void setMin_dB(float val);

private:
    void initFFT(int size);
    static void compute(double data[], int nn, int isign);

    int                 fftSize;
    Core::QueueBuffer   * fftSampleBuffer;
    double              * fftValues;
    Core::Mutex         * fftMutex;

    float maxdB;
    float mindB;

    int sampleRate;
    int channels;
    int fSize;
};

_DS_END

#endif
