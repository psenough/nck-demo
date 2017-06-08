
/**
* NCK-DEMO SCENE
* https://github.com/nczeroshift/nck-demo
*/

#include "dsAudioPlayer.h"
#include "nckUtils.h"

_DS_BEGIN

AudioPlayer::AudioPlayer(int sampleRate, int bufferSamples, int bufferCount) {
    this->sampleRate = sampleRate;
    this->bufferSamples = bufferSamples;
    this->bufferCount = bufferCount;
    channels = 2;
    repeatTimestamp = 0;
    firstTimestamp = 0;
    fft = NULL;
    buffer = new Core::QueueBuffer();
    device = NULL;
    decodeBuffer = new uint8_t[2 * sampleRate * sizeof(int16_t) * channels]; // at least 2 second buffer
    mutex = Core::CreateMutex();
}

AudioPlayer::~AudioPlayer() {
    SafeDelete(buffer);
    SafeDelete(device);
    SafeArrayDelete(decodeBuffer);
    SafeDelete(mutex);
}

void AudioPlayer::Start() {
    int csize = sizeof(int16_t);

    device = dynamic_cast<Audio::OutputDevice*>(Audio::CreateDevice(Audio::DEVICE_OUTPUT, 
        sampleRate, 
        channels, 
        Audio::FORMAT_S16, 
        bufferSamples * csize, 
        bufferCount));

    device->SetCallback(this);

    fft = new AudioFFT(bufferSamples, sampleRate, channels, csize);
}

bool AudioPlayer::isPlaying() {
    return device->GetState() == Audio::STATE_PLAYING;
}

void AudioPlayer::LoadBG(const std::string & filename) {
    stream = device->LoadStream(filename);
    if (stream->GetSampleRate() != device->GetSampleRate()) {
        Core::DebugLog("Stream sample rate doesn't match output device");
    }
}

void AudioPlayer::Play() {
    device->Play();
}

void AudioPlayer::Stop() {
    device->Stop();
}

void AudioPlayer::Pause() {
    device->Pause();
}

void AudioPlayer::Prime() {
    // Reduce inicial decoding overhead, queing the data to the buffer
    const int bufferData = channels * sizeof(int16_t) * bufferSamples;
    int readSize = stream->Read(bufferData, decodeBuffer);
    buffer->Push(decodeBuffer, readSize);

    uint64_t currentTime = dynamic_cast<Audio::SeekStream*>(stream)->GetTime();
    uint64_t dataTime = (1e6 * readSize) / (long double)((stream->GetSampleRate() * stream->GetChannelsCount() * sizeof(int16_t)));
    firstTimestamp = currentTime - dataTime;
}

void AudioPlayer::Seek(int64_t time) {
    bool isPlaying = device->GetState() == Audio::STATE_PLAYING;

    device->Stop();

    mutex->Lock();
    buffer->Clear();
    repeatTimestamp = 0;
    dynamic_cast<Audio::SeekStream*>(stream)->SetTime(time);
    Prime();
    mutex->Unlock();

    if(isPlaying)
        device->Play();
}

int64_t AudioPlayer::GetDuration() {
    if(stream)
        return dynamic_cast<Audio::SeekStream*>(stream)->GetDuration();
    return 0;
}

int64_t AudioPlayer::GetTime() {
    if (device->GetState() != Audio::STATE_STOPPED) {
        return device->GetTime() + firstTimestamp - repeatTimestamp;
    }
    return 0;
}

float AudioPlayer::GetdB(float f) {
    return fft->GetdB(f);
}

void AudioPlayer::GetdBArray(float * values, int size) {

}

int AudioPlayer::Buffer(Audio::Device * dev, int bufferSize, uint8_t * bufferData) {
    const int dataToRead = dev->GetChannelsCount() * sizeof(int16_t) * bufferSamples;

    mutex->Lock();

    if (buffer->Size() >= dataToRead)
    {
        int size = (int)buffer->Pop(bufferData, dataToRead);
        mutex->Unlock();
        fft->AddSamples(bufferData, size);
        return size;
    }

    int readSize = stream->Read(dataToRead, decodeBuffer);

    if (!readSize) {
        firstTimestamp = 0;
        repeatTimestamp = device->GetTime();
        dynamic_cast<Audio::SeekStream*>(stream)->SetTime(0);
        readSize = stream->Read(dataToRead, decodeBuffer);
    }

    mutex->Unlock();

    if (!readSize) {
        Core::DebugLog("Error: Unable to restart audio file\n");
    }
    else {
        buffer->Push(decodeBuffer, readSize);
        readSize = (int)buffer->Pop(bufferData, dataToRead);
    }

    fft->AddSamples(bufferData, readSize);
   
    return readSize;
}

AudioFFT * AudioPlayer::GetFFT() {
    return fft;
}

_DS_END