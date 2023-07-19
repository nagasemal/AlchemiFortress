#include "pch.h"
#include "Animation.h"
#include "NecromaLib/Singleton/DeltaTime.h"

Animation::Animation():
    m_nextIndex(0),
    m_animTimer(),
    m_animData()
{
}

Animation::~Animation()
{
}

void Animation::AddAnim(std::vector<AnimData> animData)
{
    m_animData = animData;
    
    for (int i = 0; i < m_animData.size(); i++)
    {
        m_animTimer.push_back(0.0f);
    }

}

void Animation::AddAnim(float animFirst, float animLast, float speed, float flyingTime)
{
    m_animData.push_back(AnimData(animFirst, animLast, speed,flyingTime));
     m_animTimer.push_back(0.0f);
}

void Animation::ClearAnim()
{
    m_animData.clear();
    m_animTimer.clear();
}

float Animation::Animatior()
{

     if (m_animTimer[m_nextIndex] <= 1 && m_nextIndex > m_animTimer.size())
     {
         m_animTimer[m_nextIndex] += m_animData[m_nextIndex].speed * DeltaTime::GetInstance().GetDeltaTime();

         return m_animTimer[m_nextIndex];
     }
     else
     {
         m_nextIndex++;
     }

     // ëSÇƒ1Ç…Ç»Ç¡ÇΩÇÃÇ≈è¡Ç∑
     if (m_nextIndex <= m_animTimer.size()) ClearAnim();

     return 1.0f;
}
