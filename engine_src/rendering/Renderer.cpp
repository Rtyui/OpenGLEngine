#include "rendering/Renderer.h"
#include "entities/Entity.h"
#include "interface/Debug.h"
#include "Resources.h"
#include "components/Transform.h"

#include <vendor/glm/gtc/matrix_transform.hpp>

Renderer* Renderer::s_Renderer = nullptr;
const unsigned Renderer::LAYERS_NUM = 10;

Renderer::Renderer() :
    m_Layers(LAYERS_NUM)
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.f);
}

void Renderer::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Submit(Renderable* renderable)
{
    if (MeshRender* meshRender = dynamic_cast<MeshRender*>(renderable))
    {
        Submit(meshRender);
    }
    else if (SpriteRender* spriteRender = dynamic_cast<SpriteRender*>(renderable))
    {
        Submit(spriteRender);
    }
    else if (TextRender* textRender = dynamic_cast<TextRender*>(renderable))
    {
        Submit(textRender);
    }
    else
    {
        Debug::Log(Debug::DebugLevel::Error, "Unknown renderable type of entity " + renderable->GetEntity()->GetName());
    }
}

void Renderer::UnSubmit(Renderable* renderable)
{
    if (MeshRender* meshRender = dynamic_cast<MeshRender*>(renderable))
    {
        UnSubmit(meshRender);
    }
    else if (SpriteRender* spriteRender = dynamic_cast<SpriteRender*>(renderable))
    {
        UnSubmit(spriteRender);
    }
    else if (TextRender* textRender = dynamic_cast<TextRender*>(renderable))
    {
        UnSubmit(textRender);
    }
    else
    {
        Debug::Log(Debug::DebugLevel::Error, "Unknown renderable type of entity " + renderable->GetEntity()->GetName());
    }
}

void Renderer::Submit(MeshRender* meshRender)
{
    if (!meshRender)
    {
        Debug::Log(Debug::Warning, "meshRender null");
        return;
    }

    unsigned layer = meshRender->GetLayer();
    if (layer >= m_Layers.size())
    {
        Debug::LogConsole(Debug::Warning, "Layer " + std::to_string(layer) + "out of bounds!");
        return;
    }

    Material* material = meshRender->GetMaterial();
    Mesh* mesh = meshRender->GetMesh();
    RenderCacheData& layerData = m_Layers[layer];

    if (layerData.meshRenderCache.find(material) == layerData.meshRenderCache.end())
    {
        layerData.meshRenderCache[material] = std::unordered_map<Mesh*, std::vector<MeshRender*>>();
    }
    if (layerData.meshRenderCache[material].find(mesh) == layerData.meshRenderCache[material].end())
    {
        layerData.meshRenderCache[material][mesh] = std::vector<MeshRender*>();
    }
    layerData.meshRenderCache[material][mesh].push_back(meshRender);
}

void Renderer::Submit(SpriteRender* spriteRender)
{
    if (!spriteRender)
    {
        Debug::Log(Debug::Warning, "spriteRender null");
        return;
    }

    unsigned layer = spriteRender->GetLayer();
    if (layer >= m_Layers.size())
    {
        Debug::LogConsole(Debug::Warning, "Layer " + std::to_string(layer) + "out of bounds!");
        return;
    }

    RenderCacheData& layerData = m_Layers[layer];
    if (layerData.spriteRenderCache.find(spriteRender->GetSprite()) == layerData.spriteRenderCache.end())
    {
        layerData.spriteRenderCache[spriteRender->GetSprite()] = std::vector<SpriteRender*>();
    }
    layerData.spriteRenderCache[spriteRender->GetSprite()].push_back(spriteRender);
}

void Renderer::Submit(TextRender* textRender)
{
    if (!textRender)
    {
        Debug::Log(Debug::Warning, "textRender null");
        return;
    }

    unsigned layer = textRender->GetLayer();
    if (layer >= m_Layers.size())
    {
        Debug::LogConsole(Debug::Warning, "Layer " + std::to_string(layer) + "out of bounds!");
        return;
    }

    RenderCacheData& layerData = m_Layers[layer];

    layerData.textRenderCache.push_back(textRender);
}

void Renderer::UnSubmit(MeshRender* meshRender)
{
    if (!meshRender)
    {
        Debug::Log(Debug::Warning, "spriteRender null");
        return;
    }

    unsigned layer = meshRender->GetLayer();
    if (layer >= m_Layers.size())
    {
        Debug::LogConsole(Debug::Warning, "Layer " + std::to_string(layer) + "out of bounds!");
        return;
    }

    Material* material = meshRender->GetMaterial();
    Mesh* mesh = meshRender->GetMesh();

    RenderCacheData& layerData = m_Layers[layer];

    if (layerData.meshRenderCache.find(material) == layerData.meshRenderCache.end())
    {
        Debug::Log(Debug::Error, "Cannot remove meshRender, material " + material->GetName() + " has no entry");
    }
    if (layerData.meshRenderCache[material].find(mesh) == layerData.meshRenderCache[material].end())
    {
        Debug::Log(Debug::Error, "Cannot remove meshRender, mesh" + mesh->GetName() + " has no entry");
    }
    auto& meshRenderArray = layerData.meshRenderCache[material][mesh];
    for (unsigned i = 0; i < meshRenderArray.size(); ++i)
    {
        if (meshRenderArray[i] == meshRender)
        {
            meshRenderArray.erase(meshRenderArray.begin() + i);
            return;
        }
    }
}

void Renderer::UnSubmit(SpriteRender* spriteRender)
{
    unsigned layer = spriteRender->GetLayer();
    if (layer >= m_Layers.size())
    {
        Debug::LogConsole(Debug::Warning, "Layer " + std::to_string(layer) + "out of bounds!");
        return;
    }

    RenderCacheData& layerData = m_Layers[layer];
    if (layerData.spriteRenderCache.find(spriteRender->GetSprite()) == layerData.spriteRenderCache.end())
    {
        Debug::LogConsole(Debug::Error, "SpriteRender of " + spriteRender->GetEntity()->GetName() + " does not exist!");
        return;
    }
    std::vector<SpriteRender*> spriteRenders = layerData.spriteRenderCache[spriteRender->GetSprite()];

    for (unsigned i = 0; i < spriteRenders.size(); ++i)
    {
        if (spriteRenders[i] == spriteRender)
        {
            spriteRenders.erase(spriteRenders.begin() + i);
            return;
        }
    }
}

void Renderer::UnSubmit(TextRender* textRender)
{
    unsigned layer = textRender->GetLayer();
    if (layer >= m_Layers.size())
    {
        Debug::LogConsole(Debug::Warning, "Layer " + std::to_string(layer) + "out of bounds!");
        return;
    }
    RenderCacheData& layerData = m_Layers[layer];

    for (unsigned i = 0; i < layerData.textRenderCache.size(); ++i)
    {
        if (layerData.textRenderCache[i] == textRender)
        {
            layerData.textRenderCache.erase(layerData.textRenderCache.begin() + i);
        }
    }
}

void Renderer::Draw()
{
    for (RenderCacheData& layer : m_Layers)
    {
        DrawEntities(layer);
        DrawUI(layer);
    }
}

void Renderer::DrawEntities(RenderCacheData& layer)
{
    Entity* entity;

    for (auto it = layer.meshRenderCache.begin(); it != layer.meshRenderCache.end(); ++it)
    {
        Material* material = it->first;
        material->Bind();
        for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2)
        {
            Mesh* mesh = it2->first;
            mesh->Bind();
            for (MeshRender* meshRender : it2->second)
            {
                if (!meshRender->IsActive())
                {
                    continue;
                }

                entity = meshRender->GetEntity();
                if (!entity)
                {
                    Debug::Log(Debug::Error, "Cannot draw meshrender without entity!");
                    continue;
                }
                material->GetShader()->SetUniformMat4f("u_TransformationMatrix", entity->GetTransform()->GetTransformationMatrix());
                glDrawElements(GL_TRIANGLES, mesh->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
            }
        }
    }
}

void Renderer::DrawUI(RenderCacheData& layer)
{
    glDisable(GL_DEPTH_TEST);
    DrawSprites(layer);
    DrawText(layer);
    glEnable(GL_DEPTH_TEST);
}

void Renderer::DrawSprites(RenderCacheData& layer)
{
    Shader* shader = Resources::Singleton()->GetShader("Sprite");
    Mesh* mesh = Resources::Singleton()->GetMesh("Quad");

    shader->Bind();
    mesh->Bind();

    for (auto& it : layer.spriteRenderCache)
    {
        it.first->Bind(0);
        shader->SetUniform1i("u_Texture", 0);

        for (SpriteRender* spriteRender : it.second)
        {
            if (!spriteRender->IsActive())
            {
                continue;
            }
            shader->SetUniform4fv("u_Color", spriteRender->GetColor());
            shader->SetUniformMat4f("u_TransformationMatrix", spriteRender->GetEntity()->GetTransform()->GetTransformationMatrix());
            glDrawElements(GL_TRIANGLES, mesh->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
        }
    }
}

void Renderer::DrawText(RenderCacheData& layer)
{
    Shader* textShader = Resources::Singleton()->GetShader("Text");
    Shader* textCursorShader = Resources::Singleton()->GetShader("TextCursor");

    Mesh* mesh = nullptr;
    textShader->Bind();


    for (TextRender* textRender : layer.textRenderCache)
    {
        if (!textRender->IsActive())
        {
            continue;
        }

        mesh = textRender->GetMesh();
        mesh->Bind();
        textShader->SetUniform3fv("u_Color", textRender->GetColor());
        textRender->GetFont()->GetTextureAtlas()->Bind(0);
        textShader->SetUniform1i("u_Texture", 0);
        textShader->SetUniformMat4f("u_TransformationMatrix", textRender->GetEntity()->GetTransform()->GetNoScaleTM());
        glDrawElements(GL_TRIANGLES, mesh->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
    }

    textCursorShader->Bind();
    for (TextRender* textRender : layer.textRenderCache)
    {
        InputTextRender* inputTextRender = dynamic_cast<InputTextRender*>(textRender);
        if (!inputTextRender || !inputTextRender->DoRenderCursor() || !textRender->IsActive())
        {
            continue;
        }
        mesh = inputTextRender->GetCursorMesh();
        mesh->Bind();
        textCursorShader->SetUniform3fv("u_Color", textRender->GetColor());
        textCursorShader->SetUniformMat4f("u_TransformationMatrix", textRender->GetEntity()->GetTransform()->GetNoScaleTM() * inputTextRender->GetCursorTransform());
        glDrawElements(GL_TRIANGLES, mesh->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

    }
}

Renderer* Renderer::Singleton()
{
    if (!s_Renderer)
    {
        s_Renderer = new Renderer();
    }
    return s_Renderer;
}

void Renderer::Destroy()
{
    if (!s_Renderer)
    {
        return;
    }
    delete s_Renderer;
}
