
/**
* NCtoolKit © 2007-2017 Luís F.Loureiro, under zlib software license.
* https://github.com/nczeroshift/nctoolkit
*
*/

#include "dsSceneParticles.h"

#define FFT_FETCH_COUNT 2

dsParticles::dsParticles(DS::Data * data, DS::AudioFFT * fft) : DS::Stage(data){
    fetchFFTCount = 0;
    particlesCountX = 128;
    particlesCountY = 128;
    this->fft = fft;
    fftTexRow = 0;
}

dsParticles::~dsParticles() {

}

Graph::Mesh * dsParticles::buildParticlesMesh(int dim_x, int dim_y)
{ 
    Graph::VertexProfile vp(2);
    vp.PushBack(0, 3, 1, 0);
    vp.PushBack(sizeof(float) * 3, 2, 4, 0);

    const unsigned int v_count = dim_x * dim_y * 4;
    const unsigned int f_count = dim_x * dim_y * 2;

    Core::QueueBuffer * vbdata = new Core::QueueBuffer(v_count * (3 + 2) * sizeof(float));
    Core::QueueBuffer * fbdata = new Core::QueueBuffer(f_count * 3 * sizeof(int));

    unsigned int faces_count = 0;
    unsigned int vIdx = 0;

    for (int x = 0; x < dim_x; x++)
    {
        for (int y = 0; y < dim_y; y++)
        {
            Math::Vec2 uv(x / (float)dim_x, y / (float)dim_y);
            Math::Vec3 p;

            p = Math::Vec3(-1, -1, 0);
            vbdata->Push((void*)&p, sizeof(Math::Vec3));
            vbdata->Push((void*)&uv, sizeof(Math::Vec2));

            p = Math::Vec3(-1, 1, 0);
            vbdata->Push((void*)&p, sizeof(Math::Vec3));
            vbdata->Push((void*)&uv, sizeof(Math::Vec2));

            p = Math::Vec3(1, 1, 0);
            vbdata->Push((void*)&p, sizeof(Math::Vec3));
            vbdata->Push((void*)&uv, sizeof(Math::Vec2));

            p = Math::Vec3(1, -1, 0);
            vbdata->Push((void*)&p, sizeof(Math::Vec3));
            vbdata->Push((void*)&uv, sizeof(Math::Vec2));

            unsigned int v1 = vIdx;
            unsigned int v2 = vIdx + 1;
            unsigned int v3 = vIdx + 2;
            unsigned int v4 = vIdx + 3;

            fbdata->Push((void*)&v1, sizeof(unsigned int));
            fbdata->Push((void*)&v2, sizeof(unsigned int));
            fbdata->Push((void*)&v3, sizeof(unsigned int));

            fbdata->Push((void*)&v1, sizeof(unsigned int));
            fbdata->Push((void*)&v3, sizeof(unsigned int));
            fbdata->Push((void*)&v4, sizeof(unsigned int));

            vIdx += 4;
            faces_count += 2;
        }
    }

    unsigned int gCount[2] = { faces_count ,0 };
    unsigned int * fdataGrouped[2] = { (unsigned int*)fbdata->GetData(),0 };

    Graph::Device * dev = m_Data->GetGraphicsDevice();

    Graph::Mesh * ret = dev->CreateMesh(v_count, vp, vbdata->GetData(), 1, gCount, fdataGrouped);

    SafeDelete(vbdata);
    SafeDelete(fbdata);

    return ret;
}

void dsParticles::Load(){
    Graph::Device * dev = m_Data->GetGraphicsDevice();

    // Enable z test
    dev->Enable(Graph::STATE_DEPTH_TEST);

    // Set correct states for alpha blending.
    dev->Enable(Graph::STATE_BLEND);
    dev->BlendFunc(Graph::BLEND_SRC_ALPHA, Graph::BLEND_INV_SRC_ALPHA);

    shader = m_Data->LoadProgram("shader://particle.cpp");
    starsShader = m_Data->LoadProgram("shader://particle_2.cpp");

    const int dim_x = particlesCountX;
    const int dim_y = particlesCountY;

    particlesMesh = buildParticlesMesh(dim_x, dim_y);

    starsMesh = buildParticlesMesh(32, 32);

    dev->ClearColor(0, 0, 0, 255);

    fftTexAux = new float[particlesCountX * particlesCountY];
    fftTexture = dynamic_cast<Graph::Texture2D*>(dev->CreateTexture(Graph::TEXTURE_2D, particlesCountX, particlesCountY, 0, Graph::FORMAT_R_32F, false));
    fftTexture->SetFilter(Graph::FILTER_MINIFICATION, Graph::FILTER_NEAREST);
    fftTexture->SetFilter(Graph::FILTER_MAGNIFICATION, Graph::FILTER_NEAREST);
    fftTexture->SetFilter(Graph::FILTER_MIPMAPPING, Graph::FILTER_NONE);

    starsTexture = dynamic_cast<Graph::Texture2D*>(dev->CreateTexture(Graph::TEXTURE_2D, 64, 64, 0, Graph::FORMAT_R_32F, false));
    starsTexture->SetFilter(Graph::FILTER_MINIFICATION, Graph::FILTER_NEAREST);
    starsTexture->SetFilter(Graph::FILTER_MAGNIFICATION, Graph::FILTER_NEAREST);
    starsTexture->SetFilter(Graph::FILTER_MIPMAPPING, Graph::FILTER_NONE);

    float * d = (float*)starsTexture->Lock();
    for(int i = 0;i<64;i++)
        for (int j = 0; j < 64; j++) {
            d[i * 64 + j] = Math::RandomValue(-1, 1);
        }
    starsTexture->Unlock();
}

void dsParticles::RenderFBO(int64_t start, int64_t end, int64_t time) {
    Graph::Device * dev = m_Data->GetGraphicsDevice();

    float dt = (time - start) / 1.0e6;

    if (fetchFFTCount++ > 1) {

        const float maxFreq = 15000;
        const float minFreq = 50;

        uint8_t * fftTexData = (uint8_t*)fftTexture->Lock();

        for (int i = 0; i < particlesCountX; i++) {
            float alpha = i / (float)particlesCountX;
            float freq = minFreq * (1.0 - alpha) + maxFreq * (alpha);
            double dB = fft->GetdB(freq);
            ((float*)fftTexData)[i] = dB;
        }

        memcpy(&fftTexData[sizeof(float)*particlesCountX], fftTexData, sizeof(float)*(particlesCountX * (particlesCountY - 1)));
        fftTexture->Unlock();

        fetchFFTCount = 0;
    }

    // Clear graphics rendering buffer.
    //dev->Clear();

    // Define the rendering area in window space.
    //dev->Viewport(0,0, m_Data->GetWidth(), m_Data->GetHeight());

    // Set projection matrix.
    dev->MatrixMode(Graph::MATRIX_PROJECTION);
    dev->Identity();

    dev->Perspective(m_Data->GetWidth() / m_Data->GetHeight());

    // Set the view (camera) matrix.
    dev->MatrixMode(Graph::MATRIX_VIEW);
    dev->Identity();

    // Set the model matrix.
    dev->MatrixMode(Graph::MATRIX_MODEL);
    dev->Identity();


    dev->Rotate(dt, 0, 0, 1);

    dev->Disable(Graph::STATE_CULL_FACE);
    dev->Disable(Graph::STATE_DEPTH_TEST);

    shader->Enable();
    shader->SetVariable1f("dt", dt);

    shader->SetVariable1f("flip", 1);
    //Math::Vec3 p(0, 0, 0);
    //RenderParticle(p);

    fftTexture->Enable();
    dev->PushMatrix();

    particlesMesh->Enable();
    particlesMesh->Render();
    particlesMesh->Disable();
    dev->PopMatrix();

    shader->SetVariable1f("flip", -1);
    dev->PushMatrix();
    dev->Translate(0, 0, 1);

    particlesMesh->Enable();
    particlesMesh->Render();
    particlesMesh->Disable();
    dev->PopMatrix();
    fftTexture->Disable();
    shader->Disable();

    starsShader->SetVariable1f("dt", dt);
    starsShader->Enable();
    starsTexture->Enable();
    starsMesh->Enable();
    starsMesh->Render();
    starsMesh->Disable();
    starsTexture->Disable();
    starsShader->Disable();

    /*
    dev->MatrixMode(Graph::MATRIX_PROJECTION);
    dev->Identity();

    dev->Ortho2D(m_Data->GetWidth(), m_Data->GetHeight());

    // Set the view (camera) matrix.
    dev->MatrixMode(Graph::MATRIX_VIEW);
    dev->Identity();

    // Set the model matrix.
    dev->MatrixMode(Graph::MATRIX_MODEL);
    dev->Identity();

    shader2->Enable();
    fftTexture->Enable();
    m_Data->GetShapeRenderer()->Square(0, 0, 256, 256, Math::Color4ub(255, 255, 255));
    fftTexture->Disable();
    shader2->Disable();
    */
}
void dsParticles::Render(int64_t start, int64_t end, int64_t time){
    RenderFBO(start, end, time);
}
