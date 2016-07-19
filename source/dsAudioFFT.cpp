
/**
* NCK-DEMO SCENE
* https://github.com/nczeroshift/nck-demo
*/

#include "dsAudioFFT.h"

_DS_BEGIN

AudioFFT::AudioFFT(int size,int sampleRate, int channels, int formatSize) {
    fftValues = NULL;
    initFFT(size);
    this->sampleRate = sampleRate;
    this->channels = channels;
    fSize = formatSize;
    maxdB = 0;
    mindB = -50;
}

AudioFFT::~AudioFFT() {
    SafeDelete(fftValues);
    SafeDelete(fftSampleBuffer);
    SafeDelete(fftMutex);
}

void AudioFFT::initFFT(int size) {
    fftSize = (int)pow(2.0, ceil(log((double)size) / log(2.0)));
    fftMutex = Core::CreateMutex();
    // samples * (real & imaginary parts) * (negative real numbers & positive real numbers domains)
    fftValues = new double[(fftSize+1) * 4]; 
    memset(fftValues, 0, sizeof(double) * (fftSize+1) * 4);
    fftSampleBuffer = new Core::QueueBuffer();
}

void AudioFFT::AddSamples(uint8_t * data, uint32_t size) {
    fftSampleBuffer->Push(data, size);

    int sampleSize = (fSize * channels);
    int samplesCount = fftSampleBuffer->Size() / sampleSize;

    if (samplesCount >= fftSize)
    {
        fftSampleBuffer->Pop(NULL, fftSampleBuffer->Size() - fftSize * sampleSize);

        int16_t * samples = (int16_t*)fftSampleBuffer->GetData();

        fftMutex->Lock();
        for (int i = 0; i < fftSize; i++) {
            float v = (samples[i * 2 + 0] + samples[i * 2 + 1]) / (32767.f*2);
            if (v < -1) v = -1;
            else if (v > 1) v = 1;
            fftValues[i * 2 + 0] = v;
            fftValues[i * 2 + 1] = 0.0;
        }

        for (int i = fftSize; i < fftSize * 2; i++) {
            fftValues[i * 2 + 0] = 0;
            fftValues[i * 2 + 1] = 0;
        }

        compute(fftValues, fftSize, 1);
        fftMutex->Unlock();

        fftSampleBuffer->Clear();
    }
}

float AudioFFT::GetdB(float f) {
    int i = fftSize * f / sampleRate;
    if (i >= fftSize / 2)
        return mindB;

    fftMutex->Lock();

    // Left Side, Negative Real values
    float vLReal = fftValues[(i * 2) + 0] / (float)fftSize;
    float vLImaginary = fftValues[(i * 2) + 1] / (float)fftSize;

    // Single Side Spectrum Transform
    float m1 = sqrt(vLReal * vLReal + vLImaginary * vLImaginary);

    // Right Side, Positive Real values
    int n = fftSize - i;
    float vRReal = fftValues[(i * 2) + 0] / (float)fftSize;
    float vRImaginary = fftValues[(i * 2) + 1] / (float)fftSize;
    float m2 = sqrt(vRReal * vRReal + vRImaginary * vRImaginary) * 2; //  (not sure if correct...)

    fftMutex->Unlock();

    float dB = 10 * log10(m1 + m2);

    if (isnan(dB) || isinf(dB) || dB < mindB)
        dB = mindB;

    if (dB > maxdB)
        dB = maxdB;

    dB += (maxdB - mindB);
    return dB;
}


void AudioFFT::compute(double data[], int nn, int isign)
{
    int n, mmax, m, j, istep, i;
    double wtemp, wr, wpr, wpi, wi, theta;
    double tempr, tempi;

    n = nn << 1;
    j = 1;
    for (i = 1; i < n; i += 2) {
        if (j > i) {
            tempr = data[j];     data[j] = data[i];     data[i] = tempr;
            tempr = data[j + 1]; data[j + 1] = data[i + 1]; data[i + 1] = tempr;
        }
        m = n >> 1;
        while (m >= 2 && j > m) {
            j -= m;
            m >>= 1;
        }
        j += m;
    }
    mmax = 2;
    while (n > mmax) {
        istep = 2 * mmax;
        theta = TWOPI / (isign*mmax);
        wtemp = sin(0.5*theta);
        wpr = -2.0*wtemp*wtemp;
        wpi = sin(theta);
        wr = 1.0;
        wi = 0.0;
        for (m = 1; m < mmax; m += 2) {
            for (i = m; i <= n; i += istep) {
                j = i + mmax;
                tempr = wr*data[j] - wi*data[j + 1];
                tempi = wr*data[j + 1] + wi*data[j];
                data[j] = data[i] - tempr;
                data[j + 1] = data[i + 1] - tempi;
                data[i] += tempr;
                data[i + 1] += tempi;
            }
            wr = (wtemp = wr)*wpr - wi*wpi + wr;
            wi = wi*wpr + wtemp*wpi + wi;
        }
        mmax = istep;
    }
}

float AudioFFT::GetMax_dB(){ return maxdB; }
float AudioFFT::GetMin_dB(){ return mindB; }
void AudioFFT::setMax_dB(float val) { maxdB = val; }
void AudioFFT::setMin_dB(float val) { mindB = val; }


_DS_END