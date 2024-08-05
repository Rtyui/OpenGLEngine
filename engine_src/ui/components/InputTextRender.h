#pragma once

#include "ui/components/TextRender.h"

class InputTextRender : public TextRender, public EventListener
{

private:

    static const float CURSOR_FLASH_INTERVAL;

    static float s_CursorWidth;
    static InputTextRender* s_SelectedInput;

    bool m_Selected;
    bool m_RenderCursor;
    
    float m_Cursor;
    float m_CursorTimer;

    Mesh* m_CursorMesh;
    glm::mat4 m_CursorTransform;

public:

    ~InputTextRender();

    virtual void Update() override;
    void SetSelected(const bool& selected);

    inline Mesh* GetCursorMesh() const { return m_CursorMesh; }
    inline const glm::mat4 GetCursorTransform() const { return m_CursorTransform; }
    inline const bool DoRenderCursor() const { return m_RenderCursor; }

    virtual void HandleEvent(KeyEvent* keyEvent) override;
    virtual void HandleEvent(CharEvent* charEvent) override;
    virtual void HandleEvent(MouseButtonEvent* mouseButtonEvent) override;

    static Component* CreateFromXMLNode(const pugi::xml_node& node, Entity* entity);
    static Component* Create(Font* font, const unsigned& fontSize, const glm::vec3& color, const glm::vec3& size);

protected:

    void InitTextParams(const glm::vec3& size, unsigned& maxCharCount);

private:

    InputTextRender(Font* font, const glm::vec3& color, const glm::vec3& size);

    virtual void Initialize(const glm::vec3& size) override;

    void CreateCursorMesh();
    void UpdateCursorTM(const unsigned& lineIndex);
    //void UpdateMeshTextCursor(const unsigned& lineIndex);
    void UpdateCursorAnimation();

};