#include "mod_weekendbonus.h"

#include "WorldSessionMgr.h"

void WeekendBonus::OnStartup()
{
    if (!HasActiveMultipliers())
    {
        return;
    }

    Triggered = false;
    LocalTime = time(NULL);

    if ((localtime(&LocalTime)->tm_wday == Day::FRIDAY && localtime(&LocalTime)->tm_hour >= 18) || localtime(&LocalTime)->tm_wday == Day::SATURDAY || localtime(&LocalTime)->tm_wday == Day::SUNDAY)
    {
        SetRates(true);
    }
}

void WeekendBonus::OnUpdate(uint32 diff)
{
    if (!HasActiveMultipliers())
    {
        return;
    }

    CheckTime += Milliseconds(diff);
    if (CheckTime > CheckFrequency)
    {
        LocalTime = time(NULL);

        if ((localtime(&LocalTime)->tm_wday == Day::FRIDAY && localtime(&LocalTime)->tm_hour >= 18) && !Triggered)
        {
            sWorldSessionMgr->SendServerMessage(SERVER_MSG_STRING, "周末经验翻倍活动开始啦！");
            SetRates(true);
        }
        else if (localtime(&LocalTime)->tm_wday == Day::MONDAY && Triggered)
        {
            sWorldSessionMgr->SendServerMessage(SERVER_MSG_STRING, "周末经验翻倍活动已结束。");
            SetRates(false);
        }

        CheckTime = 0s;
    }

    if (Triggered)
    {
        AnnouncementTime += Milliseconds(diff);
        if (AnnouncementTime > AnnouncementFrequency)
        {
            sWorldSessionMgr->SendServerMessage(SERVER_MSG_STRING, "周末经验翻倍活动已激活！");
            AnnouncementTime = 0s;
        }
    }
}

bool WeekendBonus::HasActiveMultipliers()
{
    if (ExperienceMultiplier > 1 || MoneyMultiplier > 1 || ProfessionsMultiplier > 1 || ReputationMultiplier > 1 || ProficienciesMultiplier > 1 || HonorMultiplier > 1)
    {
        return true;
    }

    return false;
}
