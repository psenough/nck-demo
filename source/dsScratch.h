#include "dsStage.h"

class dsScratch1 : public DS::Stage {
public:
    dsScratch1(DS::Data * data);
    virtual ~dsScratch1();
    void Load();
    void Render(int64_t start, int64_t end, int64_t time);
private:
    DS::Compound * s1;
    Graph::Program * p;
};