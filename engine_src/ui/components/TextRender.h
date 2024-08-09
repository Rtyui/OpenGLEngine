#pragma once

#include "components/Component.h"
#include "ui/primitives/Font.h"
#include "rendering/Mesh.h"
#include "interface/events/EventListener.h"
#include "interface/events/Event.h"
#include "components/Renderable.h"

#include <vendor/glm/glm.hpp>

class TextRender : public Renderable
{

protected:

    std::string m_Text;

    Font* m_Font;
    Mesh* m_Mesh;

    glm::vec3 m_Color;
    float m_FontScale;

private:

    unsigned m_FontSize;
    bool m_WrapLines;

    float m_MaxLineSize;
    unsigned m_MaxLinesNumber;

public:
    ~TextRender();


    virtual void Update() override;
    virtual void SetText(const std::string& text);
    void SetColor(const glm::vec3& color);

    inline const glm::vec3& GetColor() { return m_Color; }
    inline Font* GetFont() { return m_Font; }
    inline Mesh* GetMesh() { return m_Mesh; }
    inline const std::string& GetText() { return m_Text; }

    static Component* CreateFromXMLNode(const pugi::xml_node& node, Entity* entity);
    static Component* Create(Font* font, const unsigned& fontSize, const glm::vec3& color, const bool& input, const bool& wrapLines,
        const glm::vec3& size, const std::string& text);

protected:

    TextRender();
    TextRender(Font* font, const glm::vec3& color);

    virtual void Initialize(const glm::vec3& size);
    bool AddCharacter(std::vector<float>& vertices, std::vector<unsigned>& indices, const char& c, const unsigned& lineIndex, const unsigned& m_MeshIndex, float& cursor);

    virtual void InitTextParams(const glm::vec3& size, unsigned& maxCharCount);
    float GetTextSize(const std::string& text);
private:

    TextRender(Font* font, const unsigned& fontSize, const glm::vec3& color, const bool& wrapLines, const glm::vec3& size, const std::string& text);

    void FillMesh();
    std::vector<std::string> GetTextLines();

};

