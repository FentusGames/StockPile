#ifndef ASHITA_STOCKPILE_H_INCLUDED
#define ASHITA_STOCKPILE_H_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/**
 * Main Ashita SDK Header
 *
 * This is the main header to include while creating Ashita plugins. This header includes
 * the various interfaces, structures, and enumerations needed for working with Ashita. It
 * also includes the other headers included with the SDK in a single file.
 *
 * Update the path to this file to where you have the latest version of Ashita installed to.
 */

#include "S:\Steam\steamapps\common\FFXINA\SquareEnix\AshitaV4\plugins\sdk\Ashita.h"
#include "Control.h"

#include <filesystem>
#include <algorithm>
#include <fstream>
#include <format>
#include <string>
#include <ctime>
#include <list>
#include <map>

 /**
  * Stockpile Class Implementation
  *
  * This is the main class object the plugin will create an instance of and return to Ashita when
  * the 'CreatePlugin' export is called. This class MUST inherit from the 'IPlugin' interface to
  * work properly with Ashita.
  */
class Stockpile final : public IPlugin
{
    // ASHITA
    IAshitaCore* m_AshitaCore;          // The main AshitaCore object pointer.
    ILogManager* m_LogManager;          // The main LogManager object pointer.
    IDirect3DDevice8* m_Direct3DDevice; // The main Direct3D device object pointer.
    uint32_t m_PluginId;                // The plugins id. (It's base address.)

    char window_title[255]{};

    // CONFIG DEFAULTS
    // Tolerances (Bot)
    float tolerance_yaw = 0.25f;	    // YAW Tolerance				            0.10 to  0.5 Yalms (Default:  0.20)
    float tolerance_z = 4.0f;           // Z Tolerance					            0.50 to 20.0 Yalms (Default:  4.00)
    // Ranges (Bot)
    float range_new_target = 25.0f;     // Range to choose a new target	           20.00 to 50.0 Yalms (Default: 25.00)
    float range_engage = 19.0f;	        // Range to engage target		           10.00 to 30.0 Yalms (Default: 17.00)
    float range_attacking = 2.5f;	    // Range to stop moving			            2.00 to  4.0 Yalms (Default:  2.50)
    float range_minimum = 1.0f;	        // Range to move backwards		            1.00 to  2.0 Yalms (Default:  1.00)
    // Ranges (Pathing)
    float range_next_path = 3.0f;       // Range to process next node on path       5.00 to 20.0 Yalms (Default:  5.00)
    float range_auto_pathing = 10.0f;   // Range to create a new autopathing node   5.00 to 10.0 Yalms (Default:  5.00)

    // CONFIG STATIC
    bool debug = true;                  // Sets debug mode. (Default: false)

    // ZONE DATA
    struct MobDatData
    {
        char name[0x1C];
        long server_id;
        long target_index;
        long zone_id;
    };

    std::vector<MobDatData> mobs;
    std::vector<std::string> mobs_common_bans = {"???", "", "none", "EFFECTER"};
    std::vector<std::string> mobs_potential_bans = {",", ".", "#", "Moogle"};
    std::vector<std::string> mobs_unique_names;
    std::vector<std::string> mobs_selected;

    struct Pos
    {
        float x;
        float y;
        float z;
    };

    std::vector<Pos> auto_pathing_positions;
    Pos pos_old{};

    int item_current_idx = 0;
    char mtarget[128]{};

    // CONTROLS
    std::list<Control> controls
    {
        Control("numpad2", false, false),
        Control("numpad8", false, false),
        Control("left", false, false),
        Control("right", false, false),
    };

    // RUNNING
    bool running = false;
    bool auto_pathing = false;

    IEntity* entity;
    ITarget* target;

    // PLAYER
    int player_id;

    // IDS
    int targeted_id = -1;
    const char* targeted_name = "No Valid Target";

    // BOOLS
    bool has_lock = false;
    bool has_target = false;
    bool is_player_dead = false;
    bool reverse_path = false;

    // CLOSEST TARGET
    int closest_target_id = -1;
    const char* closest_target_name = "No Valid Target";
    float closest_target_distance = 0;

    // CLOSEST PATH ID
    int closest_path_id = -1;

    // TIMERS
    std::chrono::steady_clock::time_point begin_new_attack = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point begin_new_target = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point begin_new_select = std::chrono::steady_clock::now();

    // MOVING
    float oldx = -1;
    float oldy = -1;

    bool target_moving = true;

    // ZONE
    int zone_id = -1;

    // COLORS
    ImVec4 red      = ImVec4(0.83f, 0.33f, 0.28f, 1.00f);
    ImVec4 green    = ImVec4(0.33f, 0.83f, 0.28f, 1.00f);

    // BANS
    bool include_pit;

public:
    Stockpile(void);
    ~Stockpile(void);

    // Plugin Information Properties
    const char* GetName(void) const override;
    const char* GetAuthor(void) const override;
    const char* GetDescription(void) const override;
    const char* GetLink(void) const override;
    double GetVersion(void) const override;
    double GetInterfaceVersion(void) const override;
    int32_t GetPriority(void) const override;
    uint32_t GetFlags(void) const override;

    // General Methods
    bool Initialize(IAshitaCore* core, ILogManager* logger, uint32_t id) override;
    void Release(void) override;

    // Event Callbacks: ChatManager
    bool HandleCommand(int32_t mode, const char* command, bool injected) override;
    bool HandleIncomingText(int32_t mode, bool indent, const char* message, int32_t* modifiedMode, bool* modifiedIndent, char* modifiedMessage, bool injected, bool blocked) override;

    // Event Callbacks: PacketManager
    bool HandleIncomingPacket(uint16_t id, uint32_t size, const uint8_t* data, uint8_t* modified, uint32_t sizeChunk, const uint8_t* dataChunk, bool injected, bool blocked) override;
    bool HandleOutgoingPacket(uint16_t id, uint32_t size, const uint8_t* data, uint8_t* modified, uint32_t sizeChunk, const uint8_t* dataChunk, bool injected, bool blocked) override;

    // Event Callbacks: Direct3D
    bool Direct3DInitialize(IDirect3DDevice8* device) override;
    void Direct3DBeginScene(bool isRenderingBackBuffer) override;
    void Direct3DEndScene(bool isRenderingBackBuffer) override;
    void Direct3DPresent(const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion) override;

    // Helpers.cpp
    void Log(const std::string& str);
    void QueueCommand(int32_t, const std::string& str);
    int RandomFV(int factor, int var);
    int RandomA(int factor);
    int Random(int min, int max);
    void ltrim(std::string& s);
    void rtrim(std::string & s);
    void trim(std::string & s);
    bool contains_find(std::vector<std::string> vec, std::string search);
    bool contains_search(std::vector<std::string> vec, std::string search);
    float distance(Pos p1, Pos p2);

    // Control.cpp
    void ControlsReload();
    void ControlsReset();
    void ControlsDown(std::string control);
    void Controls();

    float GetHeadingDifference(float x2, float y2);

    // DATS
    void LoadMobDatData();
};

#endif // __ASHITA_STOCKPILE_H_INCLUDED__