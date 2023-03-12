#ifndef CONTROL_H_INCLUDED
#define CONTROL_H_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

class Control final
{
    std::string control;
    bool c;
    bool o;

public:
    Control(const std::string& _control, const bool _c, const bool _o)
        : control(_control)
        , c(_c)
        , o(_o)
    {}
    ~Control(void) {}

    std::string GetControl() const
    {
        return this->control;
    }

    bool GetC() const
    {
        return this->c;
    }

    bool GetO() const
    {
        return this->o;
    }

    void SetControl(const std::string& _control)
    {
        this->control = _control;
    }

    void SetC(const bool _c)
    {
        this->c = _c;
    }

    void SetO(const bool _o)
    {
        this->o = _o;
    }
};

#endif // CONTROL_H_INCLUDED