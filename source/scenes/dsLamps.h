
#ifndef DS_LAMPS_H
#define DS_LAMPS_H

#include "../dsStage.h"

class LampConfig {
public:
    LampConfig() {};

    static void bind(LampConfig & config, Graph::Program * prog);
    static LampConfig & generate(DS::Compound * c, Math::Mat44 viewMatrix);
      
private:
    Math::Vec4 lamp_pos[8]; // position, energy
    Math::Vec4 lamp_dir[8]; // normal, type (0 - point, 1 - spot, 2 - sun, 3 -area) 
    Math::Vec4 lamp_color[8]; // rgb, distance
    Math::Vec4 lamp_params[8]; // ????
};


#endif