#ifndef RENDER_NRECONTEXT_H
#define RENDER_NRECONTEXT_H

/**
 * @author  lyzirving
 * @date    2022-12-13
 * @brief   a context that contains all the resources needed for rendering
**/

class NreContext
{
public:
    NreContext();
    virtual ~NreContext();

    void init();
private:
};

#endif //RENDER_NRECONTEXT_H
