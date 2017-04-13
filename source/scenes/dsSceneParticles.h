
/**
* NCtoolKit © 2007-2017 Luís F.Loureiro, under zlib software license.
* https://github.com/nczeroshift/nctoolkit
*
*/

#ifndef _NCK_DS_PARTICLES_H_
#define _NCK_DS_PARTICLES_H_

#include "../dsStage.h"
#include "../dsAudioFFT.h"

class dsParticles : public DS::Stage{
public:
    dsParticles(DS::Data * data, DS::AudioFFT * fft);
    virtual ~dsParticles();

    void Load();
    void Render(int64_t start, int64_t end, int64_t time);
    void RenderFBO(int64_t start, int64_t end, int64_t time);
    
private:
    int fftTexRow;
    int particlesCountX;
    int particlesCountY;
    int fetchFFTCount;
    float * fftTexAux;
    DS::AudioFFT * fft;
    Graph::Texture2D * fftTexture;
    Graph::Program * shader;
    Graph::Mesh * particlesMesh;

    Graph::Mesh * buildParticlesMesh(int dim_x, int dim_y);
    Graph::Mesh * starsMesh;
    Graph::Texture2D * starsTexture;
    Graph::Program * starsShader;
};

#endif
