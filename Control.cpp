#ifndef CONTROL_HELPERS_H_INCLUDED
#define CONTROL_HELPERS_H_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "Stockpile.h"
#include "Zones.h"

void Stockpile::ControlsReload()
{
    QueueCommand(-1, std::format("/releasekeys"));

    for (std::list<Control>::iterator it = controls.begin(); it != controls.end(); ++it)
    {
        it->SetC(false);
        it->SetO(false);
    }
}

void Stockpile::ControlsReset()
{
    for (std::list<Control>::iterator it = controls.begin(); it != controls.end(); ++it)
    {
        it->SetC(false);
    }
}

void Stockpile::ControlsDown(std::string control)
{
    for (std::list<Control>::iterator it = controls.begin(); it != controls.end(); ++it)
    {
        if (it->GetControl() == control)
        {
            it->SetC(true);
        }
    }
}

void Stockpile::Controls()
{
    for (std::list<Control>::iterator it = controls.begin(); it != controls.end(); ++it)
    {
        if (it->GetC() && !it->GetO())
        {
            QueueCommand(-1, std::format("/sendkey {} down", it->GetControl()));
            it->SetO(it->GetC());
        }

        if (!it->GetC() && it->GetO())
        {
            QueueCommand(-1, std::format("/sendkey {} up", it->GetControl()));
            it->SetO(false);
        }
    }
}

#endif // CONTROL_HELPERS_H_INCLUDED