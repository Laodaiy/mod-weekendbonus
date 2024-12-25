#include "mod_weekendbonus.h"

void WeekendBonus::OnStartup()
{
    Triggered = false;
    LocalTime = time(NULL);

    if ((localtime(&LocalTime)->tm_wday == Day::FRIDAY && localtime(&LocalTime)->tm_hour >= 18) || localtime(&LocalTime)->tm_wday == Day::SATURDAY || localtime(&LocalTime)->tm_wday == Day::SUNDAY)
        SetRates(true);
}

void WeekendBonus::OnUpdate(uint32 diff)
{
    CheckTime += Milliseconds(diff);
    if (CheckTime > CheckFrequency)
    {
        LocalTime = time(NULL);

        if ((localtime(&LocalTime)->tm_wday == Day::FRIDAY && localtime(&LocalTime)->tm_hour >= 18) && !Triggered)
        {
            sWorld->SendServerMessage(SERVER_MSG_STRING, "周末经验翻倍活动开始啦！");
            SetRates(true);
        }
        else if (localtime(&LocalTime)->tm_wday == Day::MONDAY && Triggered)
        {
            sWorld->SendServerMessage(SERVER_MSG_STRING, "周末经验翻倍活动已结束。");
            SetRates(false);
        }

        CheckTime = 0s;
    }

    if (Triggered)
    {
        AnnouncementTime += Milliseconds(diff);
        if (AnnouncementTime > AnnouncementFrequency)
        {
            sWorld->SendServerMessage(SERVER_MSG_STRING, "周末经验翻倍活动已激活！");
            AnnouncementTime = 0s;
        }
    }
}
