#ifndef __SKYBOX_H_INCLUDED__
#define __SKYBOX_H_INCLUDED__

class CSkybox
{
public:
    CSkybox();
    ~CSkybox();

    void Initialize(float size);
    bool LoadTextures(char* top, char* bottom, char* front, char* back, char* left, char* right);
    void Render(float cameraX, float cameraY, float cameraZ);
    void Release();

    enum {
        SKY_TOP,
        SKY_BOTTOM,
        SKY_FRONT,
        SKY_BACK,
        SKY_LEFT,
        SKY_RIGHT
    };

protected:
    unsigned int m_textures[6];     // 6 texture objects
    float     m_size;
};

#endif // __SKYBOX_H_INCLUDED__