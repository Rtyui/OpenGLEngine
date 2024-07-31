#pragma once
class TimeClock
{
private:

    static TimeClock* s_TimeClock;

    float m_DeltaTime;
    float m_LastTime;

public:

    static TimeClock* Singleton();
    static void Destroy();

    void Update();
    inline const float GetDeltaTime() const { return m_DeltaTime; }

private:

    TimeClock();
};

