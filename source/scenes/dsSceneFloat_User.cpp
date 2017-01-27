
#include "dsSceneFloat_User.h"
#include "nckCamera.h"
#include "../dsUtils.h"


dsSceneFloat_User::dsSceneFloat_User(DS::Data * data) : dsSceneFloatPopup(data){
    userTex[0] = NULL;
    userTex[1] = NULL;
    userTex[2] = NULL;
    userProg = NULL;
    userId = 0;
}

dsSceneFloat_User::~dsSceneFloat_User() {
  
}

void dsSceneFloat_User::Load() {
    userProg = m_Data->LoadProgram("shader://user.cpp");
    if (userId == 0) {
        userTex[0] = m_Data->LoadTexture("texture://tex2d_jae_1.png");
        userTex[1] = m_Data->LoadTexture("texture://tex2d_jae_2.png");
        userTex[2] = m_Data->LoadTexture("texture://tex2d_jae_3.png");
    } else if (userId == 1) {
        userTex[0] = m_Data->LoadTexture("texture://tex2d_zeroshift_1.png");
        userTex[1] = m_Data->LoadTexture("texture://tex2d_zeroshift_2.png");
        userTex[2] = m_Data->LoadTexture("texture://tex2d_zeroshift_3.png");
    } else if (userId == 2) {
        userTex[0] = m_Data->LoadTexture("texture://tex2d_ps_1.png");
        userTex[1] = m_Data->LoadTexture("texture://tex2d_ps_2.png");
        userTex[2] = m_Data->LoadTexture("texture://tex2d_ps_3.png");
    }
}


void dsSceneFloat_User::RenderFBO(int64_t start, int64_t end, int64_t time) {
    Graph::Device * dev = m_Data->GetGraphicsDevice();

}

void dsSceneFloat_User::setUser(int user) {
    userId = user;
}

void dsSceneFloat_User::Render(int64_t start, int64_t end, int64_t time) {

    Graph::Device * dev = m_Data->GetGraphicsDevice();
    Gui::ShapeRenderer * shp = m_Data->GetShapeRenderer();

    prepare(start, end, time);

    Math::Vec2 pos = Math::LinearInterpolation(showPosA, showPosB, alpha);

    dev->Disable(Graph::STATE_DEPTH_TEST);

    dev->PushMatrix();
    dev->Translate(pos.GetX(), pos.GetY(), 0);

    dev->Color(255, 255, 255, 240);
  
    userTex[0]->Enable();
    userTex[1]->Enable(1);
    userTex[2]->Enable(2);

    userProg->Enable();
    userProg->SetVariable1i("gphTexture0", 0);
    userProg->SetVariable1i("gphTexture1", 1);
    userProg->SetVariable1i("gphTexture2", 2);

    userProg->SetVariable1f("time", (time - start + randomOff) / 1e6);
    userProg->SetVariable4f("size", width, height, alpha*height, 0);
    
    DS::RenderSquare(dev, width, alpha*height, false);
    
    userTex[0]->Disable();
    userTex[1]->Disable(1);
    userTex[2]->Disable(2);

    userProg->Disable();

    drawBorders();
}