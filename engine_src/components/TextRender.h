#pragma once

#include "components/Component.h"
#include "ui/primitives/Font.h"
#include "rendering/Mesh.h"
#include "interface/events/EventListener.h"
#include "interface/events/Event.h"
#include "components/Renderable.h"

#include <vendor/glm/glm.hpp>

class TextRender : public EventListener, public Renderable
{

private:

    static const float CURSOR_FLASH_INTERVAL;

    static float s_CursorWidth;
    static TextRender* s_SelectedTextRender;

    std::string m_OriginalText;
    std::string m_MeshText;

    Font* m_Font;
    Mesh* m_Mesh;
    Mesh* m_CursorMesh;

    glm::vec3 m_Color;
    unsigned m_FontSize;

    bool m_WrapLines;
    bool m_Input;
    bool m_RenderCursor;
    bool m_Selected;

    float m_FontScale;
    float m_Cursor;
    float m_MaxLineSize;
    unsigned m_MaxLinesNumber;
    unsigned m_MeshIndex;

    float m_CursorTimer;

public:
    ~TextRender();
    virtual void HandleEvent(KeyEvent* keyEvent) override;
    virtual void HandleEvent(CharEvent* charEvent) override;
    virtual void HandleEvent(MouseButtonEvent* mouseButtonEvent) override;

    virtual void Update() override;
    void SetText(const std::string& text);
    void SetSelected(const bool& selected);
    void SetColor(const glm::vec3& color);

    inline const glm::vec3& GetColor() { return m_Color; }
    inline Font* GetFont() { return m_Font; }
    inline Mesh* GetMesh() { return m_Mesh; }
    inline Mesh* GetCursorMesh() { return m_CursorMesh; }
    inline const bool& DoRenderCursor() const { return m_RenderCursor; }
    inline const bool& IsInput() const { return m_Input; }
    inline const std::string& GetText() { return m_MeshText; }

    static Component* CreateFromXMLNode(const pugi::xml_node& node, Entity* entity);
    static Component* Create(Font* font, const unsigned& fontSize, const glm::vec3& color, const bool& input, const bool& wrapLines,
        const glm::vec3& size, const std::string& text);

private:

    TextRender();
    TextRender(Font* font, const unsigned& fontSize, const glm::vec3& color, const bool& input, const bool& wrapLines,
        const glm::vec3& size, const std::string& text);

    void Initialize(const glm::vec3& size);
    bool AddCharacter(std::vector<float>& vertices, std::vector<unsigned>& indices, const char& c, const unsigned& lineIndex);
    void UpdateMeshText();
    void UpdateMeshTextCursor(const unsigned& lineIndex);
    std::vector<std::string> GetTextLines();
    float GetWordSize(const std::string& word);

    void UpdateCursorAnimation();
};

