/**
 * @file
 *
 * @Author OmniBlade
 *
 * @brief Multiplayer settings handling class.
 *
 * @copyright Thyme is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef MULTIPLAYERSETTINGS_H
#define MULTIPLAYERSETTINGS_H

#ifndef THYME_STANDALONE
#include "hooker.h"
#endif

#include "asciistring.h"
#include "color.h"
#include "ini.h"
#include "money.h"
#include "subsysteminterface.h"
#include <map>
#include <vector>

class MultiplayerColorDefinition
{
public:
    MultiplayerColorDefinition();
    MultiplayerColorDefinition &operator=(MultiplayerColorDefinition &rval);
    void Set_Color(RGBColor rgb);
    void Set_Night_Color(RGBColor rgb);

    static void Parse_Color_Definition(INI *ini);

private:
    AsciiString m_tooltipName;
    RGBColor m_rgbValue;
    int m_color;
    RGBColor m_rgbNightValue;
    int m_nightColor;

    static FieldParse s_colorFieldParsetable[];
};

inline MultiplayerColorDefinition::MultiplayerColorDefinition() :
    m_tooltipName(),
    m_rgbValue{1.0f, 1.0f, 1.0f},
    m_color(-1),
    m_rgbNightValue{1.0f, 1.0f, 1.0f},
    m_nightColor(-1)
{
}

inline MultiplayerColorDefinition &MultiplayerColorDefinition::operator=(MultiplayerColorDefinition &rval)
{
    if (this != &rval) {
        m_tooltipName = rval.m_tooltipName;
        m_rgbValue = rval.m_rgbValue;
        m_color = rval.m_color;
        m_rgbNightValue = rval.m_rgbNightValue;
        m_nightColor = rval.m_nightColor;
    }

    return *this;
}

class MultiplayerSettings : public SubsystemInterface
{
public:
    MultiplayerSettings();

    // Subsystem interface implementation.
    virtual void Init() override {}
    virtual void Reset() override {}
    virtual void Update() override {}

    MultiplayerColorDefinition *Find_Color_Definition(AsciiString name);
    MultiplayerColorDefinition *New_Color_Definition(AsciiString name);

private:
    int m_initialCreditsMin;
    int m_initialCreditsMax;
    int m_startCountdownTimer;
    int m_maxBeaconsPerPlayer;
    bool m_useShroud;
    bool m_showRandomPlayerTemplate;
    bool m_showRandomStartPos;
    bool m_showRandomColor;
    std::map<AsciiString, MultiplayerColorDefinition> m_colorList;
    int m_numColors;
    MultiplayerColorDefinition m_colorDef1;
    MultiplayerColorDefinition m_colorDef2;
    std::vector<Money> m_moneyVec;
    Money m_startingMoney;
    bool m_moneyDefault;
};

#ifndef THYME_STANDALONE
extern MultiplayerSettings *&g_theMultiplayerSettings;
#else
#define g_theMultiplayerSettings (Make_Global<MultiplayerSettings *>(0x00A2BE58))
extern MultiplayerSettings *g_theMultiplayerSettings;
#endif

#endif // MULTIPLAYERSETTINGS_H