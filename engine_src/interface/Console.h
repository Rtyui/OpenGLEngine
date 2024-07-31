#pragma once

#include "entities/Entity.h"
#include "ui/primitives/Font.h"
#include "interface/events/EventListener.h"

#include <vector>

class Console : public EventListener
{
    friend class Debug;
private:

    static const unsigned NUM_ROWS;
    static Console* s_Console;

    std::vector<Entity*> m_Rows;
    Entity* m_InputRow;
    Entity* m_Background;

    Font* m_Font;
    float m_RowHeight;

    bool m_Show;

public:

    static Console* Singleton();
    static void Destroy();

    void Update();
    void PushLine(const std::string& text, const glm::vec3 color = { 1.f, 1.f, 1.f });

private:

    Console();
    ~Console();

    virtual void HandleEvent(KeyEvent* keyEvent) override;
    void UpdateShow();
    void UpdateEntities();


    void CreateBackground();
    void CreateTextRows();
};

