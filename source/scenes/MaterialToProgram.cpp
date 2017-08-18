
#include "MaterialToProgram.h"

MaterialToProgram::MaterialToProgram(Graph::Device * dev) {
    m_Device = dev;
    enableTransform = false;
    enableArmature = false;
}

MaterialToProgram::~MaterialToProgram() {

}

Graph::Program * MaterialToProgram::generate(Scene::Material * mat) {
    bool hasBumpmap = false;
    bool hasTexture = false;
    bool hasLight = !mat->GetFlag(Scene::MATERIAL_SHADELESS);
    int totalTextures = 0;

    std::map<int, Scene::TextureLayer*> layers;
    for (int l = 0; l < 8; l++) {
        Scene::TextureLayer * layer = mat->GetTextureLayer(l);
        if (layer != NULL) {
            hasTexture = true;
            if ((layer->GetFactorFlag() & Scene::FACTOR_NORMAL) != 0)
                hasBumpmap = true;
            layers.insert(std::pair<int, Scene::TextureLayer*>(l, layer));
            totalTextures++;
        }
    }

    std::string src = "";

    // Fragment shader
    src += "#pragma fragment_shader_glx2\n";
    src += "#include \"core.cpp\"\n";
    if (hasLight) {
        src += "#include \"lights.cpp\"\n";
        if (hasBumpmap)
            src += "#include \"bumpmap.cpp\"\n";
    }

    MapFor(int, Scene::TextureLayer*, layers, i) {
        Graph::Texture * tex = i->second->GetTexture()->GetTexture();
        if (tex->GetType() == Graph::TEXTURE_2D) {
            src += "uniform sampler2D gphTexture" + Math::IntToString(i->first) + ";\n";
        }
        else if (tex->GetType() == Graph::TEXTURE_3D) {
            src += "uniform sampler3D gphTexture" + Math::IntToString(i->first) + ";\n";
        }
        else if (tex->GetType() == Graph::TEXTURE_CUBEMAP) {
            src += "uniform samplerCube gphTexture" + Math::IntToString(i->first) + ";\n";
        }
    }

    src += "void main(void){\n"
        "\tvec3 diff = vec3(0.0);\n"
        "\tvec3 spec = vec3(0.0);\n"
        "\tfloat alpha = gphAlpha;\n"
        "\tvec3 N = normalize(v_nor_mv);\n"
        "\tvec3 colDiff = rgb_to_srgb_approx(gphDiffuseColor.rgb);\n"
        "\tvec3 colSpec = rgb_to_srgb_approx(gphSpecularColor.rgb);\n";

    if (hasTexture) {
        MapFor(int, Scene::TextureLayer*, layers, i) {
            Scene::TextureLayer * tl = i->second;
            Graph::Texture * tex = tl->GetTexture()->GetTexture();
            std::string tString = "";

            if (tl->GetMappingObject() != NULL) {
                // Por enquanto sem uniforms para ligar a transform do Objecto.
                Scene::Object * obj = tl->GetMappingObject();
                if ((obj->GetScale() - Math::Vec3(1.0, 1.0, 1.0)).Length() > 0.01) {
                    if (tex->GetType() == Graph::TEXTURE_2D)
                        tString += "* vec2(" + Math::FloatToString(obj->GetScale().GetX(), 2) + "," + Math::FloatToString(obj->GetScale().GetY(), 2) + ")";
                    else
                        tString += "* vec3(" + Math::FloatToString(obj->GetScale().GetX(), 2) + "," + Math::FloatToString(obj->GetScale().GetY(), 2) + "," + Math::FloatToString(obj->GetScale().GetZ(), 2) + ")";
                }
                if ((obj->GetPosition() - Math::Vec3(0.0, 0.0, 0.0)).Length() > 0.001) {
                    if (tex->GetType() == Graph::TEXTURE_2D)
                        tString += "+ vec2(" + Math::FloatToString(obj->GetPosition().GetX(), 2) + "," + Math::FloatToString(obj->GetPosition().GetY(), 2) + ")";
                    else
                        tString += "+ vec3(" + Math::FloatToString(obj->GetPosition().GetX(), 2) + "," + Math::FloatToString(obj->GetPosition().GetY(), 2) + "," + Math::FloatToString(obj->GetPosition().GetZ(), 2) + ")";
                }
            }

            src += "\t{\n";

            if (tex->GetType() == Graph::TEXTURE_2D) {
                src += "\t\tvec4 tex = texture2D(gphTexture" + Math::IntToString(i->first) + ",gl_TexCoord[0].xy" + tString + ");\n";
            }
            else if (tex->GetType() == Graph::TEXTURE_3D) {
                src += "\t\tvec4 tex = texture3D(gphTexture" + Math::IntToString(i->first) + ",gl_TexCoord[0].xyz" + tString + ");\n";
            }
            else if (tex->GetType() == Graph::TEXTURE_CUBEMAP) {
                src += "\t\tvec4 tex = textureCube(gphTexture" + Math::IntToString(i->first) + ",gl_TexCoord[0].xyz" + tString + ");\n";
            }

            if ((tl->GetFactorFlag() & Scene::FACTOR_DIFFUSE_COLOR) != 0) {
                src += "\t\tcolDiff = mix(colDiff,tex.xyz," + Math::FloatToString(tl->GetFactorDiffuseColor(), 2) + ");\n";
            }

            if ((tl->GetFactorFlag() & Scene::FACTOR_SPECULAR_COLOR) != 0) {
                src += "\t\tcolSpec = mix(colSpec,tex.xyz," + Math::FloatToString(tl->GetFactorSpecularColor(), 2) + ");\n";
            }

            if ((tl->GetFactorFlag() & Scene::FACTOR_ALPHA) != 0) {
                src += "\t\talpha = mix(alpha,tex.a," + Math::FloatToString(tl->GetFactorAlpha(), 2) + ");\n";
            }

            if ((tl->GetFactorFlag() & Scene::FACTOR_NORMAL) != 0) {
                src += "\t\tN = bumpmap_mix_sample(N,tex.xyz,"+Math::FloatToString(tl->GetFactorNormal(), 2)+");\n";
            }

            src += "\t}\n";

        }
    }
    else {
        //src += "\tres = vec4(1.0);\n";
    }

    if (hasLight) {
        src += "\tlights_compute(v_pos_mv,N,diff,spec);\n";
        src += "\tgl_FragColor = vec4(diff * colDiff + spec * colSpec, alpha);\n"
            "}\n";
    }
    else {
        src += "\tgl_FragColor = vec4(colDiff, alpha);\n"
            "}\n";
    }
    



    src += "#pragma vertex_shader_glx2\n"
        "#include \"core.cpp\"\n";
    if (hasLight) {
        src += "#include \"lights.cpp\"\n";
        if(hasBumpmap)
            src += "#include \"bumpmap.cpp\"\n";
    }

    if (enableTransform)
        src += "#include \"transform.cpp\"\n";
    
    if(enableArmature)
        src += "#include \"armature.cpp\"\n";

    src += "void main(){\n"
        "\tvec4 P = gl_Vertex;\n"
        "\tvec3 N = gl_Normal;\n";

    if (enableArmature) {
        src += "\tvec3 aP = P.xyz;\n";
        src += "\tarmature_transform(gl_MultiTexCoord1.xyzw,gl_MultiTexCoord2.xyzw,aP,N);\n";
        src += "\tP = vec4(aP,1.0);\n";
    }

    src += "\tvec3 P_MV = vec3(gl_ModelViewMatrix * P);\n"
        "\tvec3 N_MV = normalize(gl_NormalMatrix * N);\n";

    src += "\tv_nor_w = normalize((gphModelMatrix * vec4(N,0.0)).xyz);\n";

    if (hasBumpmap) {
            src += "\tvec4 T = gl_MultiTexCoord1.xyzw;\n"
                "\tvec3 T_MV = normalize(gl_NormalMatrix * T.xyz);\n"
                "\tbumpmap_compute(P_MV,N_MV,vec4(T_MV,T.w));\n";
    }
   
    src += "\tcore_copy_texCoord();\n"
        "\tv_pos_mv = P_MV;\n"
        "\tv_nor_mv = N_MV;\n";

    if (enableTransform) {
        // Função de transform, mantem o aspecto, mas espatifa o resto.
        src += "\ttransform_project_and_twist(P);\n";
    }
    else {
        src += "\tcore_transform_to_screen(P);\n";
    }

    src += "}\n";

    Graph::Program * prog = NULL;

    try {
        prog = m_Device->LoadProgramFromText(src);
    }
    catch (const Core::Exception & e) {
        Core::DebugLog(src);
        THROW_EXCEPTION_STACK("Unable to generate shader", e);
    }

    return prog;
}