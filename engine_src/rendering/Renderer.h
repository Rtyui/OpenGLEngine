#pragma once
#include "components/MeshRender.h"
#include "components/SpriteRender.h"
#include "components/TextRender.h"

#include <unordered_map>
#include <vendor/glm/glm.hpp>
class Renderer
{
private:

    typedef struct {
        std::unordered_map <Material*, std::unordered_map<Mesh*, std::vector<MeshRender*>>> meshRenderCache;
        std::unordered_map<Texture*, std::vector<SpriteRender*>> spriteRenderCache;
        std::vector<TextRender*> textRenderCache;
    } RenderCacheData;

    static Renderer* s_Renderer;

    //std::vector<SpriteRender*> m_SpriteRenderCache;
    //std::vector<TextRender*> m_TextRenderCache;
    std::vector<RenderCacheData> m_Layers;

public:

    static const unsigned LAYERS_NUM;

    static Renderer* Singleton();
    static void Destroy();

    void Clear();
    void Draw();
    void DrawEntities(RenderCacheData& layer);
    void DrawUI(RenderCacheData& layer);
    void DrawSprites(RenderCacheData& layer);
    void DrawText(RenderCacheData& layer);
    void Submit(MeshRender* meshRender);
    void Submit(SpriteRender* spriteRender);
    void Submit(TextRender* textRender);
    void UnSubmit(MeshRender* meshRender);
    void UnSubmit(SpriteRender* spriteRender);
    void UnSubmit(TextRender* textRender);

private:

    Renderer();
};

