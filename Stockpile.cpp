#ifndef SOCKPILE_H_INCLUDED
#define SOCKPILE_H_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#define _USE_MATH_DEFINES

#include "Stockpile.h"
#include "Zones.h"

/**
 * Constructor and Deconstructor
 */
Stockpile::Stockpile(void)
    : m_AshitaCore(nullptr)
    , m_LogManager(nullptr)
    , m_Direct3DDevice(nullptr)
    , m_PluginId(0)
{}
Stockpile::~Stockpile(void)
{}

/**
 * Returns the plugins name.
 *
 * @return {const char*} The plugins name.
 */
const char* Stockpile::GetName(void) const
{
    return "Stockpile";
}

/**
 * Returns the plugins author.
 *
 * @return {const char*} The plugins author.
 */
const char* Stockpile::GetAuthor(void) const
{
    return "Fentus";
}

/**
 * Returns the plugins description.
 *
 * @return {const char*} The plugins description.
 */
const char* Stockpile::GetDescription(void) const
{
    return "Combat Bot";
}

/**
 * returns the plugins homepage link.
 *
 * @return {const char*} The plugins homepage link.
 */
const char* Stockpile::GetLink(void) const
{
    return "https://fentusgames.com/";
}

/**
 * Returns the plugins version.
 *
 * @return {double} The plugins version.
 */
double Stockpile::GetVersion(void) const
{
    return 1;
}

/**
 * Returns the plugins Ashita interface version it was compiled against.
 *
 * @return {double} The plugins interface version.
 */
double Stockpile::GetInterfaceVersion(void) const
{
    return ASHITA_INTERFACE_VERSION;
}

/**
 * Returns the plugins execution priority.
 *
 * @return {int32_t} The plugins execution priority.
 * @notes
 *
 *      The priority value is used to determine the execution order of the plugins loaded in Ashita. When an event is fired, Ashita
 *      will loop through each of the loaded plugins and allow them to attempt to handle the event. The order that the plugins are
 *      passed the event is determined by this priority. By default, 0 will load a plugin 'next' in the list of plugins who also are
 *      set to 0. Meaning if other plugins are already loaded at 0, and your plugin is also 0 your plugin will be last.
 *
 *      Plugins can force themselves sooner or later into the execution order by using this value (both negative and position).
 *      Negative values will place your plugin sooner in the execution order, while positive values will place it later.
 *
 *      Plugins should use 0 by default if order does not matter to their purpose.
 */
int32_t Stockpile::GetPriority(void) const
{
    return 0;
}

/**
 * Returns the plugins flags.
 *
 * @return {uint32_t} The plugins flags.
 * @notes
 *
 *      Ashita offers a various number of events that can be invoked inside of a plugin, allowing plugins to have a lot of control
 *      over the flow of the game. However, this can create a lot of overhead, more specifically on lower end machines. Plugins can
 *      use this property to specify the exact events it plans to make use of, allowing Ashita to only invoke ones it needs.
 *
 *      Plugins should explicitly state which flags they plan to make use of and avoid using 'All' flags.
 *      This can help with the performance of the game for lower end machines.
 *
 *      See the 'Ashita::PluginFlags' enumeration for more information on what each flag does.
 */
uint32_t Stockpile::GetFlags(void) const
{
    return (uint32_t) Ashita::PluginFlags::All;
}

/**
 * Event invoked when the plugin is being loaded and initialized.
 *
 * @param {IAshitaCore*} core - The main Ashita core instance used to interact with the Ashita hook.
 * @param {ILogManager*} logger - The log manager instance used to write to the debug log files.
 * @param {uint32_t} id - The plugins unique id. (Matches the plugins module base address.)
 * @return {bool} True on success, false otherwise.
 *
 * @notes
 *
 *      Plugins must return true from this function in order to be considered valid and continue to load.
 *      If your plugin fails to meet any requirements you feel are manditory for it to run, you should return false here and prevent it
 *      from loading further.
 */
bool Stockpile::Initialize(IAshitaCore* core, ILogManager* logger, uint32_t id)
{
    this->m_AshitaCore = core;
    this->m_LogManager = logger;
    this->m_PluginId = id;

    ::sprintf_s(this->window_title, "%s %.1f", this->GetName(), this->GetVersion());

    return true;
}

/**
 * Event invoked when the plugin is being unloaded.
 *
 * @notes
 *
 *      Plugins should use this event to cleanup all resources they created or used during their lifespan.
 *      (ie. Fonts, primitives, textures, Direct3D related resources, memory allocations, etc.)
 */
void Stockpile::Release(void)
{

}

/**
 * Event invoked when an input command is being processed by the game client.
 *
 * @param {int32_t} mode - The mode of the command. (See: Ashita::CommandMode)
 * @param {const char*} command - The raw command string.
 * @param {bool} injected - Flag if the command was injected from an Ashita related resource. (Addon, plugin or internally.)
 * @return {bool} True if handled and should be blocked, false otherwise.
 *
 * @notes
 *
 *      Only invoked if Ashita::PluginFlags::UseCommands flag is set.
 *
 *      Any commands sent through the games input handler will be passed to plugins to be processed first allowing plugins to intercept
 *      and handle any kind of command they wish. This includes commands typed into the chat bar, commands invoked from macros, menu items
 *      and so on. Commands that have been injected by Ashita or another plugin will be marked via the injected parameter.
 *
 *      If a plugin returns true, the command is blocked from further processing by Ashita or the game client and is considered handled.
 *
 *      Plugins should return true for any commands they have handled or reacted to when appropriate. To prevent deadlocks by trying to
 *      inject another command here, plugins should instead use the IChatManager::QueueCommand function for any manual command inserts
 *      back into the game.
 */
bool Stockpile::HandleCommand(int32_t mode, const char* command, bool injected)
{
    UNREFERENCED_PARAMETER(mode);
    UNREFERENCED_PARAMETER(command);
    UNREFERENCED_PARAMETER(injected);

    return false;
}

/**
 * Event invoked when incoming text is being processed by the game client, to be added to the chat window.
 *
 * @param {int32_t} mode - The message mode. (Determines its base color.)
 * @param {bool} indent - Flag that determines if the message is indented.
 * @param {const char*} message - The raw message string.
 * @param {int32_t*} modifiedMode - The modified mode, if any, that has been altered by another addon/plugin.
 * @param {bool*} modifiedIndent - The modified indent flag, if any, that has been altered by another addon/plugin.
 * @param {char*} modifiedMessage - The modified message string, if any, that has been altered by another addon/plugin.
 * @param {bool} injected - Flag if the message was injected from an Ashita related resource. (Addon, plugin, or internally.)
 * @param {bool} blocked - Flag if the message has been blocked by another addon/plugin.
 * @return {bool} True if handled and should be blocked, false otherwise.
 *
 * @notes
 *
 *      Only invoked if Ashita::PluginFlags::UseText flag is set.
 *
 *      If a plugin returns true, the block flag is set to true (cannot be unset), however the message will still be passed to all loaded
 *      plugins whom are marked to accept the event. Plugins should check if the blocked flag has been set first before reacting to messages
 *      in case a previous plugin has deemed it to not be displayed in the game chat log. Unless your plugin requires reacting to certain/all
 *      messages, then it should simply return immediately if blocked is already true.
 *
 *      mode and message contain the original, unmodified data of the event which can be used to see what was originally used to invoke the
 *      event, as well as for plugins that require the original unedited message to properly operate. Plugins should however check for changes
 *      by seeing if modifiedMessage has been altered. If modifiedMessage has any value (strlen > 0) then plugins should honor previous plugin
 *      changes made to the message. You should only completely overwrite modifiedMessage if your plugin specifically needs to take over a given
 *      message. (For example if a certain plugin injects color tags into a message, your plugin should not remove them unless absolutely necessary.)
 *
 *      modifiedMessage is an internal buffer of 4096 bytes, therefore it should have plenty of space for any message you wish to add.
 *
 *      You should not call Write, Writef, or AddChatMessage functions here! Otherwise you will cause a stack overflow.
 */
bool Stockpile::HandleIncomingText(int32_t mode, bool indent, const char* message, int32_t* modifiedMode, bool* modifiedIndent, char* modifiedMessage, bool injected, bool blocked)
{
    UNREFERENCED_PARAMETER(mode);
    UNREFERENCED_PARAMETER(indent);
    UNREFERENCED_PARAMETER(message);
    UNREFERENCED_PARAMETER(modifiedMode);
    UNREFERENCED_PARAMETER(modifiedIndent);
    UNREFERENCED_PARAMETER(modifiedMessage);
    UNREFERENCED_PARAMETER(injected);
    UNREFERENCED_PARAMETER(blocked);

    return false;
}

/**
 * Event invoked when the game client is processing an incoming packet.
 *
 * @param {uint16_t} id - The id of the packet.
 * @param {uint32_t} size - The size of the packet.
 * @param {const uint8_t*} data - The raw data of the packet.
 * @param {uint8_t*} modified - The modified packet data, if any, that has been altered by another addon/plugin.
 * @param {uint32_t} sizeChunk - The size of the full packet chunk that contained the packet.
 * @param {const uint8_t*} dataChunk - The raw data of the full packet chunk that contained the packet.
 * @param {bool} injected - Flag if the packet was injected from an Ashita related resource. (Addon, plugin, or internally.)
 * @param {bool} blocked - Flag if the packet has been blocked by another addon/plugin.
 * @return {bool} True if handled and should be blocked, false otherwise.
 *
 * @notes
 *
 *      Only invoked if Ashita::PluginFlags::UsePackets flag is set.
 *
 *      If a plugin returns true, the block flag is set to true (cannot be unset), however the event will still be passed to all loaded
 *      plugins whom are marked to accept it. Plugins should check if the blocked flag has been set first before reacting to the event
 *      in case a previous plugin has deemed it to be blocked. Unless your plugin requires reacting to certain/all packets, then it
 *      should simply return immediately if blocked is already true.
 *
 *      Packets in FFXI are sent in chunks, meaning there are multiple packets inside of each chunk. This information may be needed when
 *      dealing with certain packet ids, thus Ashita offers the ability to see the full chunk each packet was part of.
 *
 *      id, size, and data are all specific to the individual packet that caused the event to be invoked and contain the unmodified
 *      information about the individual packet. These should not be edited.
 *
 *      modified should be used to determine if changes have been made to the packet by Ashita or another addon/plugin. By default,
 *      modified is a direct copy of the original packet data.
 *
 *      sizeChunk and dataChunk hold the data of the entire chunk the packet was part of that is being processed in the event. These can
 *      be useful for plugins that may need to look at other packets in the chunk that relate to the current packet of the event. These
 *      should not be edited.
 */
bool Stockpile::HandleIncomingPacket(uint16_t id, uint32_t size, const uint8_t* data, uint8_t* modified, uint32_t sizeChunk, const uint8_t* dataChunk, bool injected, bool blocked)
{
    UNREFERENCED_PARAMETER(id);
    UNREFERENCED_PARAMETER(size);
    UNREFERENCED_PARAMETER(data);
    UNREFERENCED_PARAMETER(modified);
    UNREFERENCED_PARAMETER(sizeChunk);
    UNREFERENCED_PARAMETER(dataChunk);
    UNREFERENCED_PARAMETER(injected);
    UNREFERENCED_PARAMETER(blocked);

    return false;
}

/**
 * Event invoked when the game client is processing an outgoing packet.
 *
 * @param {uint16_t} id - The id of the packet.
 * @param {uint32_t} size - The size of the packet.
 * @param {const uint8_t*} data - The raw data of the packet.
 * @param {uint8_t*} modified - The modified packet data, if any, that has been altered by another addon/plugin.
 * @param {uint32_t} sizeChunk - The size of the full packet chunk that contained the packet.
 * @param {const uint8_t*} dataChunk - The raw data of the full packet chunk that contained the packet.
 * @param {bool} injected - Flag if the packet was injected from an Ashita related resource. (Addon, plugin, or internally.)
 * @param {bool} blocked - Flag if the packet has been blocked by another addon/plugin.
 * @return {bool} True if handled and should be blocked, false otherwise.
 *
 * @notes
 *
 *      Only invoked if Ashita::PluginFlags::UsePackets flag is set.
 *
 *      If a plugin returns true, the block flag is set to true (cannot be unset), however the event will still be passed to all loaded
 *      plugins whom are marked to accept it. Plugins should check if the blocked flag has been set first before reacting to the event
 *      in case a previous plugin has deemed it to be blocked. Unless your plugin requires reacting to certain/all packets, then it
 *      should simply return immediately if blocked is already true.
 *
 *      Packets in FFXI are sent in chunks, meaning there are multiple packets inside of each chunk. This information may be needed when
 *      dealing with certain packet ids, thus Ashita offers the ability to see the full chunk each packet was part of.
 *
 *      id, size, and data are all specific to the individual packet that caused the event to be invoked and contain the unmodified
 *      information about the individual packet. These should not be edited.
 *
 *      modified should be used to determine if changes have been made to the packet by Ashita or another addon/plugin. By default,
 *      modified is a direct copy of the original packet data.
 *
 *      sizeChunk and dataChunk hold the data of the entire chunk the packet was part of that is being processed in the event. These can
 *      be useful for plugins that may need to look at other packets in the chunk that relate to the current packet of the event. These
 *      should not be edited.
 */
bool Stockpile::HandleOutgoingPacket(uint16_t id, uint32_t size, const uint8_t* data, uint8_t* modified, uint32_t sizeChunk, const uint8_t* dataChunk, bool injected, bool blocked)
{
    UNREFERENCED_PARAMETER(id);
    UNREFERENCED_PARAMETER(size);
    UNREFERENCED_PARAMETER(data);
    UNREFERENCED_PARAMETER(modified);
    UNREFERENCED_PARAMETER(sizeChunk);
    UNREFERENCED_PARAMETER(dataChunk);
    UNREFERENCED_PARAMETER(injected);
    UNREFERENCED_PARAMETER(blocked);

    return false;
}

/**
 * Event invoked when the plugin is being initialized for Direct3D usage.
 *
 * @param {IDirect3DDevice8*} device - The Direct3D8 device object.
 * @return {bool} True on success, false otherwise.
 *
 * @notes
 *
 *      Only invoked if Ashita::PluginFlags::UseDirect3D flag is set.
 *
 *      Plugins must return true from this function in order to be considered valid and continue to load if they do use Direct3D features.
 *
 *      If your plugin fails to meet any Direct3D requirements you feel are manditory for it to run, you should return false here and
 *      prevent it from loading further.
 */
bool Stockpile::Direct3DInitialize(IDirect3DDevice8* device)
{
    this->m_Direct3DDevice = device;

    return true;
}

/**
 * Event invoked when the Direct3D device is beginning a scene.
 *
 * @param {bool} isRenderingBackBuffer - Flag set if the scene is being rendered to the back buffer.
 *
 * @notes
 *
 *      Only invoked if Ashita::PluginFlags::UseDirect3D flag is set.
 *
 *      This event is invoked before the actual IDirect3DDevice8::BeginScene call is invoked.
 *
 *      Multiple scenes can be rendered each frame, thus the isRenderingBackBuffer flag is available to determine when the scene is being
 *      rendered to the back buffer render target. (Previous Ashita versions only invoked this event when this flag would be true.)
 */
void Stockpile::Direct3DBeginScene(bool isRenderingBackBuffer)
{
    UNREFERENCED_PARAMETER(isRenderingBackBuffer);

    if (isRenderingBackBuffer)
    {
        // PLAYER
        player_id = this->m_AshitaCore->GetMemoryManager()->GetParty()->GetMemberTargetIndex(0);

        // ENTITY
        entity = this->m_AshitaCore->GetMemoryManager()->GetEntity();

        if (auto_pathing)
        {

            Pos pos_new{};

            pos_new.x = entity->GetLocalPositionX(player_id);
            pos_new.y = entity->GetLocalPositionY(player_id);
            pos_new.z = entity->GetLocalPositionZ(player_id);

            if (distance(pos_old, pos_new) >= range_auto_pathing)
            {
                auto_pathing_positions.push_back(pos_new);
                pos_old = pos_new;
            }
        }

        // ZONE CHANGE
        if (zone_id != this->m_AshitaCore->GetMemoryManager()->GetParty()->GetMemberZone(0))
        {
            LoadMobDatData();
            zone_id = this->m_AshitaCore->GetMemoryManager()->GetParty()->GetMemberZone(0);
        }

        // CHECK IF BOT IS RUNNING
        if (running)
        {
            // RESET TARGETED
            targeted_name = "No Valid Target";

            // TARGET
            target = this->m_AshitaCore->GetMemoryManager()->GetTarget();

            // IDS
            targeted_id = target->GetTargetIndex(target->GetIsSubTargetActive() ? 1 : 0);

            // BOOLS
            has_lock = target->GetLockedOnFlags() & 0x01;
            has_target = targeted_id != 0;
            is_player_dead = entity->GetStatus(player_id) == 2 || entity->GetStatus(player_id) == 3;

            if (!is_player_dead)
            {
                ControlsReset();

                // ATTACK IF ATTACKED
                if (has_target)
                {
                    closest_target_id = targeted_id;
                }

                // NEW TARGET
                if (closest_target_id == -1)
                {
                    // RESET CLOSEST TARGET
                    closest_target_id = -1;

                    float distance_target = FLT_MAX;

                    // GET CLOSEST TARGET
                    for (int index = 0; index < 2303; index++)
                    {
                        // entity.SpawnFlags[0x01 = PC, 0x02 = NPC, 0x10 = Mob, 0x0D = Self]
                        // entity.EntityType[0 = PC, 1 = NPC, 2 = NPC(Fixed Models), 3 = Doors etc.]

                        float entity_distance = entity->GetDistance(index);

                        if (sqrt(entity_distance) <= range_new_target && entity->GetActorPointer(index) != 0 && entity->GetHPPercent(index) > 0 && entity->GetSpawnFlags(index) == 0x10 && sqrt(abs(entity->GetLocalPositionZ(player_id) - entity->GetLocalPositionZ(index))) < tolerance_z)
                        {
                            std::string search(entity->GetName(index));

                            if (contains_find(mobs_selected, search))
                            {
                                if (sqrt(entity_distance) < distance_target)
                                {
                                    distance_target = sqrt(entity_distance);
                                    closest_target_id = index;
                                }
                            }
                        }
                    }

                    // SET NAME OF NEW TARGET
                    if (closest_target_id == -1) 
                    {
                        closest_target_name = "No Valid Target";

                        // USE PATHING
                        float distance_pos = FLT_MAX;

                        if (closest_path_id == -1)
                        {
                            Pos pos_new{};

                            pos_new.x = entity->GetLocalPositionX(player_id);
                            pos_new.y = entity->GetLocalPositionY(player_id);
                            pos_new.z = entity->GetLocalPositionZ(player_id);

                            for (int n = 0; n < int(auto_pathing_positions.size()); n++)
                            {
                                if (distance(auto_pathing_positions[n], pos_new) < distance_pos)
                                {
                                    distance_pos = distance(auto_pathing_positions[n], pos_new);
                                    closest_path_id = n;
                                }
                            }
                        }
                        else
                        {
                            Pos pos = auto_pathing_positions[closest_path_id];

                            float heading_difference = GetHeadingDifference(pos.x, pos.y);

                            bool facing = abs(heading_difference) < tolerance_yaw * 2;

                            if (!facing)
                            {
                                if (heading_difference < 0)
                                {
                                    ControlsDown("right");
                                }
                                if (heading_difference > 0)
                                {
                                    ControlsDown("left");
                                }
                            }
                            
                            ControlsDown("numpad8");

                            Pos pos_new{};

                            pos_new.x = entity->GetLocalPositionX(player_id);
                            pos_new.y = entity->GetLocalPositionY(player_id);
                            pos_new.z = entity->GetLocalPositionZ(player_id);

                            if (sqrt(distance(auto_pathing_positions[closest_path_id], pos_new)) < range_next_path)
                            {
                                if (reverse_path)
                                {
                                    if (closest_path_id == int(auto_pathing_positions.size() - 1))
                                    {
                                        reverse_path = false;
                                    }
                                    else
                                    {
                                        closest_path_id++;
                                    }
                                }
                                else
                                {
                                    if (closest_path_id == 0)
                                    {
                                        reverse_path = true;
                                    }
                                    else
                                    {
                                        closest_path_id--;
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        closest_path_id = -1;
                        closest_target_name = entity->GetName(closest_target_id);
                        begin_new_target = std::chrono::steady_clock::now();
                    }
                }
                else
                {
                    if (oldx == entity->GetLocalPositionX(closest_target_id) && oldy == entity->GetLocalPositionY(closest_target_id))
                    {
                        target_moving = false;
                    }
                    else
                    {
                        oldx = entity->GetLocalPositionX(closest_target_id);
                        oldy = entity->GetLocalPositionY(closest_target_id);

                        target_moving = true;
                    }

                    // GET CLOSEST TARGFET DISTANCE
                    closest_target_distance = sqrt(entity->GetDistance(closest_target_id));

                    float x2 = entity->GetLocalPositionX(closest_target_id);
                    float y2 = entity->GetLocalPositionY(closest_target_id);

                    float heading_difference = GetHeadingDifference(x2, y2);

                    // RANGE BRACKETS
                    bool gt_bracket_new_target = closest_target_distance >= range_new_target;
                    bool gt_bracket_engage = closest_target_distance >= range_engage;
                    bool gt_bracket_attacking = closest_target_distance >= range_attacking;
                    bool gt_bracket_attacking_and_lt_bracket_engage = closest_target_distance >= range_attacking && closest_target_distance < range_engage;
                    bool lt_bracket_minimum = closest_target_distance < range_minimum;
                    bool lt_bracket_engage = closest_target_distance < range_engage;

                    // FACING
                    bool facing = abs(heading_difference) < tolerance_yaw;

                    // CLAIMED
                    bool claimed = entity->GetClaimStatus(closest_target_id) == entity->GetServerId(player_id) || entity->GetClaimStatus(closest_target_id) == 0;

                    // HAS TARGET
                    if (has_target)
                    {
                        // UNLOCK SELF & UNLOCK CLAIMS
                        if (targeted_id == player_id) {
                            QueueCommand(-1, std::format("/sendkey escape down"));
                            QueueCommand(-1, std::format("/sendkey escape up"));
                        }

                        targeted_name = entity->GetName(targeted_id);
                    }

                    // GET NEW CLOSEST TARGET
                    if (gt_bracket_new_target || !claimed || entity->GetHPPercent(closest_target_id) == 0 || entity->GetActorPointer(closest_target_id) == 0 || entity->GetStatusServer(closest_target_id) == 2 || entity->GetStatusServer(closest_target_id) == 3) {
                        closest_target_id = -1;
                        targeted_id = -1;
                    }

                    // MOVE TO TARGET WITH NUMPAD
                    if (!has_lock) {
                        // MOVE LEFT OR RIGHT
                        if (gt_bracket_engage)
                        {
                            if (!facing)
                            {
                                if (heading_difference < 0)
                                {
                                    ControlsDown("right");
                                }
                                if (heading_difference > 0)
                                {
                                    ControlsDown("left");
                                }
                            }
                        }

                        // SELECT TARGET
                        if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - begin_new_target).count() >= 5 && gt_bracket_attacking_and_lt_bracket_engage) {
                            if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - begin_new_select).count() >= 3)
                            {
                                target->SetTarget(closest_target_id, false);
                                begin_new_select = std::chrono::steady_clock::now();
                            }
                        }
                    }

                    // ATTACK TARGET
                    if (has_target && !has_lock && lt_bracket_engage) {
                        if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - begin_new_attack).count() >= 3)
                        {
                            QueueCommand(-1, std::format("/attack"));
                            begin_new_attack = std::chrono::steady_clock::now();
                        }
                    }

                    // MOVE FORWARD OR BACKWARDS
                    if ((has_target && has_lock && gt_bracket_attacking) || (!has_lock && gt_bracket_attacking) || target_moving)
                    {
                        ControlsDown("numpad8");
                    }
                    else if ((has_target && has_lock && lt_bracket_minimum) || (has_target && has_lock && !facing))
                    {
                        ControlsDown("numpad2");
                    }
                }

                Controls();
            }
        }
    }
}

float Stockpile::GetHeadingDifference(float x2, float y2)
{
    // TARGET DATA
    float h1 = entity->GetHeading(player_id);
    float x1 = entity->GetLocalPositionX(player_id);
    float y1 = entity->GetLocalPositionY(player_id);

    float radians = atan2(-(y2 - y1), x2 - x1);

    float difference = radians - h1;
    float pi = float(M_PI);

    if (difference < -pi)
    {
        difference = difference + 2 * pi;
    }

    if (difference > pi)
    {
        difference = difference - 2 * pi;
    }

    return difference;
}

void Stockpile::LoadMobDatData()
{
    char buffer[MAX_PATH]{};
    if (!Ashita::Registry::GetInstallPath(Ashita::LanguageId::Default, Ashita::SquareEnixGameId::FinalFantasyXI, buffer, MAX_PATH))
    {
        Log("Failed to load zone mob data.");
        return;
    }

    const auto zone = this->m_AshitaCore->GetMemoryManager()->GetParty()->GetMemberZone(0);
    if (!ZoneMap.contains(zone))
    {
        Log("Failed to locate required map entry.");
        return;
    }

    std::filesystem::path path = buffer;

    std::error_code ec{};
    if (!std::filesystem::exists(path, ec) || ec)
    {
        Log("Failed to locate required map DAT file.");
        return;
    }

    path += "\\";
    path += std::get<0>(ZoneMap[zone]);

    Log(std::format("Loading ({}) {}", zone, std::get<1>(ZoneMap[zone])));
    Log(std::format("Path: {}", path.string()));

    FILE* f = nullptr;
    if (::fopen_s(&f, path.string().c_str(), "rb") != ERROR_SUCCESS)
    {
        Log("Failed to open zone mob list DAT file.");
        return;
    }

    ::fseek(f, 0, SEEK_END);
    const auto size = ::ftell(f);
    ::fseek(f, 0, SEEK_SET);

    if (size == 0 || (size % 0x20) != 0)
    {
        ::fclose(f);

        Log("Mob zone list is invalid / incorrectly sized.");
        return;
    }

    // 0000 - 1023 = static zone entities.
    // 1024 - 1791 = players
    // 1792 - 2303 = spawnables (pets, summons, dynamic event entities, etc.)

    for (auto x = 0; x < size / 0x20; x++)
    {
        MobDatData mob_dat_data{};
        ::fread(&mob_dat_data, sizeof(mob_dat_data.name) + sizeof(mob_dat_data.server_id), 1, f);

        mob_dat_data.target_index = mob_dat_data.server_id & 0x07FF;
        mob_dat_data.zone_id = (mob_dat_data.server_id >> 0x0C) & 0x7F;

        mobs.push_back(mob_dat_data);

        std::string name(mob_dat_data.name);

        trim(name);

        if (!contains_find(mobs_common_bans, name))
        {
            mobs_unique_names.push_back(name);
        }
    }

    std::sort(mobs_unique_names.begin(), mobs_unique_names.end());
    mobs_unique_names.erase(std::unique(mobs_unique_names.begin(), mobs_unique_names.end()), mobs_unique_names.end());

    Log(std::format("Mobs: {}", mobs.size()));
    Log(std::format("Mobs Unique: {}", mobs_unique_names.size()));

    ::fclose(f);
}

/**
 * Event invoked when the Direct3D device is ending a scene.
 *
 * @param {bool} isRenderingBackBuffer - Flag set if the scene is being rendered to the back buffer.
 *
 * @notes
 *
 *      Only invoked if Ashita::PluginFlags::UseDirect3D flag is set.
 *
 *      This event is invoked before the actual IDirect3DDevice8::EndScene call is invoked.
 *
 *      Multiple scenes can be rendered each frame, thus the isRenderingBackBuffer flag is available to determine when the scene is being
 *      rendered to the back buffer render target. (Previous Ashita versions only invoked this event when this flag would be true.)
 */
void Stockpile::Direct3DEndScene(bool isRenderingBackBuffer)
{
    UNREFERENCED_PARAMETER(isRenderingBackBuffer);
}

/**
 * Event invoked when the Direct3D device is presenting a scene.
 *
 * @param {const RECT*} pSourceRect - The source rect being presented.
 * @param {const RECT*} pDestRect - The destination rect being presented into.
 * @param {HWND} hDestWindowOverride - The override window handle to present into.
 * @param {const RGNDATA*} pDirtyRegion - The present dirty regions.
 *
 * @notes
 *
 *      Only invoked if Ashita::PluginFlags::UseDirect3D flag is set.
 *
 *      This event is invoked before the actual IDirect3DDevice8::Present call is invoked.
 *
 *      For best results of custom Direct3D rendering, it is best to do your own custom drawing here to draw over-top of all game related
 *      scenes and objects. Usage of ImGui should be done here as well.
 */
void Stockpile::Direct3DPresent(const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion)
{
    UNREFERENCED_PARAMETER(pSourceRect);
    UNREFERENCED_PARAMETER(pDestRect);
    UNREFERENCED_PARAMETER(hDestWindowOverride);
    UNREFERENCED_PARAMETER(pDirtyRegion);

    /**
     * Ashita's Current Default Styling
    */

    /*
    auto& style = imgui->GetStyle();
    style.ChildRounding = 0.0f;
    style.FrameBorderSize = 1.0f;
    style.FramePadding = ImVec2(4.0f, 3.0f);
    style.FrameRounding = 0.0f;
    style.GrabRounding = 0.0f;
    style.ItemSpacing = ImVec2(8.0f, 4.0f);
    style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
    style.TabBorderSize = 1.0f;
    style.TabRounding = 0.0f;
    style.ScrollbarRounding = 0.0f;
    style.WindowBorderSize = 1.0f;
    style.WindowPadding = ImVec2(8.0f, 8.0f);
    style.WindowRounding = 2.0f;

    const auto colors = style.Colors;
    colors[ImGuiCol_Text] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.94f, 0.94f, 0.94f, 0.29f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.18f, 0.20f, 0.23f, 0.96f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.22f, 0.24f, 0.27f, 0.96f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.90f);
    colors[ImGuiCol_Border] = ImVec4(0.05f, 0.05f, 0.10f, 0.80f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.17f, 0.20f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.14f, 0.14f, 0.14f, 0.78f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.83f, 0.33f, 0.28f, 0.69f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.83f, 0.33f, 0.28f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.83f, 0.33f, 0.28f, 0.50f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.12f, 0.13f, 0.17f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.12f, 0.13f, 0.17f, 1.00f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.83f, 0.33f, 0.28f, 0.69f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.83f, 0.33f, 0.28f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.83f, 0.33f, 0.28f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.83f, 0.33f, 0.28f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.83f, 0.33f, 0.28f, 0.69f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.83f, 0.33f, 0.28f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.83f, 0.33f, 0.28f, 0.78f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.83f, 0.33f, 0.28f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.83f, 0.33f, 0.28f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.83f, 0.33f, 0.28f, 0.78f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.83f, 0.33f, 0.28f, 1.00f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.83f, 0.33f, 0.28f, 1.00f);
    colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.05f, 0.05f, 0.05f, 0.69f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.83f, 0.33f, 0.28f, 1.00f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.83f, 0.33f, 0.28f, 1.00f);
    colors[ImGuiCol_Tab] = ImVec4(0.05f, 0.05f, 0.05f, 0.69f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.83f, 0.33f, 0.28f, 1.00f);
    colors[ImGuiCol_TabActive] = ImVec4(0.83f, 0.33f, 0.28f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.83f, 0.33f, 0.28f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.81f, 0.81f, 0.81f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.83f, 0.33f, 0.28f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.81f, 0.81f, 0.81f, 1.00f);
    colors[ImGuiCol_TableHeaderBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.69f);
    colors[ImGuiCol_TableBorderStrong] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_TableBorderLight] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.83f, 0.33f, 0.28f, 0.50f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 1.00f, 0.00f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.83f, 0.33f, 0.28f, 1.00f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.60f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.78f);
    */

    // GUI START
    const auto& imgui = m_AshitaCore->GetGuiManager();

    imgui->SetNextWindowSize(ImVec2(0, 0), ImGuiCond_Always); // SIZE OF GUI

    if (!imgui->Begin(this->window_title)) {
        imgui->End();
        return;
    }

    // GUI PADDING
    imgui->PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 6));

    // ADD GUI ELEMENTS...
    if (imgui->Button("Start Bot", ImVec2(150, 27))) {
        running = true;
        auto_pathing = false;
    }

    imgui->SameLine();

    if (imgui->Button("Stop Bot", ImVec2(150, 27))) {
        running = false;
        Stockpile::ControlsReload();
    }

    char buff[256]{};

    sprintf_s(buff, "Running: %s", running ? "True" : "False");
    imgui->TextColored(running ? green : red, buff);

    if (imgui->CollapsingHeader("Status's"))
    {
        sprintf_s(buff, "Has Target: %s", has_target ? "True" : "False");
        imgui->TextColored(has_target ? green : red, buff);

        sprintf_s(buff, "Has Lock: %s", has_lock ? "True" : "False");
        imgui->TextColored(has_lock ? green : red, buff);

        sprintf_s(buff, "Closest Target Name: %s", closest_target_name);
        imgui->Text(buff);

        sprintf_s(buff, "Closest Target ID: %d", closest_target_id);
        imgui->TextColored(closest_target_id != -1 ? green : red, buff);

        sprintf_s(buff, "Closest Target Distance: %.2f", closest_target_distance);
        imgui->Text(buff);

        sprintf_s(buff, "Target Name: %s", targeted_name);
        imgui->Text(buff);

        sprintf_s(buff, "Target ID: %d", targeted_id);
        imgui->TextColored(targeted_id != -1 ? green : red, buff);
    }

    if (imgui->CollapsingHeader("Settings (Tolerance & Range) "))
    {

        imgui->SliderFloat("Tolerance Yaw", &tolerance_yaw, 0.1f, 0.5f, "%.2f");
        imgui->SliderFloat("Tolerance Z", &tolerance_z, 0.5f, 20.0f, "%.2f");

        imgui->SliderFloat("Range New Target", &range_new_target, 20.0f, 50.0f, "%.2f");
        imgui->SliderFloat("Range Engage", &range_engage, 10.0f, 30.0f, "%.2f");
        imgui->SliderFloat("Range Attacking", &range_attacking, 2.0f, 4.0f, "%.2f");
        imgui->SliderFloat("Range Minimum", &range_minimum, 1.0f, 2.0f, "%.2f");
    }

    if (imgui->CollapsingHeader("Targets"))
    {
        // MANUAL TARGET
        imgui->Text("Target Name:");

        imgui->InputText("", mtarget, IM_ARRAYSIZE(mtarget));

        if (imgui->Button("Add Target", ImVec2(150, 27))) {
            Log(std::format("Added: {}", mtarget));
            mobs_selected.push_back(mtarget);
        }

        imgui->Checkbox("Include Potentially Invalid Targets", &include_pit);

        imgui->Text("Double click to add or remove targets.");

        if (imgui->ListBoxHeader("Available Targets"))
        {
            for (int n = 0; n < int(mobs_unique_names.size()); n++)
            {
                if (include_pit || !contains_search(mobs_potential_bans, mobs_unique_names[n]))
                {
                    const bool is_selected = (item_current_idx == n);

                    if (imgui->Selectable(mobs_unique_names[n].c_str(), is_selected, ImGuiSelectableFlags_AllowDoubleClick))
                    {
                        item_current_idx = n;

                        if (imgui->IsMouseDoubleClicked(0))
                        {
                            if (!contains_find(mobs_selected, mobs_unique_names[item_current_idx])) {
                                mobs_selected.push_back(mobs_unique_names[item_current_idx]);

                                Log(std::format("Added: {}", mobs_unique_names[item_current_idx]));
                            }
                        }
                    }
                }
            }
            imgui->ListBoxFooter();
        }

        int remove = -1;

        if (imgui->ListBoxHeader("Selected Targets"))
        {
            for (int n = 0; n < int(mobs_selected.size()); n++)
            {
                const bool is_selected = (item_current_idx == n);

                if (imgui->Selectable(mobs_selected[n].c_str(), is_selected, ImGuiSelectableFlags_AllowDoubleClick))
                {
                    item_current_idx = n;

                    if (imgui->IsMouseDoubleClicked(0))
                    {
                        remove = item_current_idx;
                    }
                }
            }
            imgui->ListBoxFooter();
        }

        if (remove != -1) {
            Log(std::format("Removed: {}", mobs_selected[item_current_idx].c_str()));

            mobs_selected.erase(mobs_selected.begin() + item_current_idx);
            remove = -1;
        }
    }

    if (imgui->CollapsingHeader("Pathing"))
    {
        if (imgui->Button("Start Auto-Pathing", ImVec2(150, 27))) {
            auto_pathing = true;
        }

        imgui->SameLine();

        if (imgui->Button("Stop Auto-Pathing", ImVec2(150, 27))) {
            auto_pathing = false;
        }

        imgui->SliderFloat("Distance Auto-Pathing", &range_auto_pathing, 5.0f, 30.0f, "%.1f");
        imgui->SliderFloat("Range to proccess next node", &range_next_path, 3.0f, 5.0f, "%.1f");

        sprintf_s(buff, "Auto-Pathing Running: %s", auto_pathing ? "True" : "False");
        imgui->TextColored(auto_pathing ? green : red, buff);

        int remove = -1;

        if (imgui->ListBoxHeader("Pathing Positions"))
        {
            for (int n = 0; n < int(auto_pathing_positions.size()); n++)
            {
                const bool is_selected = (item_current_idx == n);

                Pos p = auto_pathing_positions[n];

                if (imgui->Selectable(std::format("[{}] {:.1f}, {:.1f}, {:.1f}", n, p.x, p.y, p.z).c_str(), is_selected, ImGuiSelectableFlags_AllowDoubleClick))
                {
                    item_current_idx = n;

                    if (imgui->IsMouseDoubleClicked(0))
                    {
                        remove = item_current_idx;
                    }
                }
            }
            imgui->ListBoxFooter();
        }

        if (remove != -1) {
            Pos p = auto_pathing_positions[item_current_idx];

            Log(std::format("[{}] {:.1f}, {:.1f}, {:.1f}", item_current_idx, p.x, p.y, p.z).c_str());

            auto_pathing_positions.erase(auto_pathing_positions.begin() + item_current_idx);
            remove = -1;
        }

        if (imgui->Button("Remove All", ImVec2(150, 27))) {
            running = false;
            auto_pathing_positions.clear();
        }
    }

    // GUI POP STYLE VAR
    imgui->PopStyleVar();

    // GUI END
    imgui->End();

    // imgui->ShowDemoWindow();
}

/**
 * Returns an instance of this plugins main class.
 *
 * @param {const char*} args - The arguments passed to the plugin when it was loaded.
 * @return {IPlugin*} The plugins class object instance.
 * @notes
 *
 *      This export must be present for a plugin to load properly!
 *
 *      The args parameter can be used to prepare the plugin in any special manner for specific properties based on the arguments passed.
 *      For example, if you wish to have a password required to load the plugin, this can be used for that kind of thing. (Not recommended
 *      as this is a weak form of authentication, just an example.)
 */
__declspec(dllexport) IPlugin* __stdcall expCreatePlugin(const char* args)
{
    UNREFERENCED_PARAMETER(args);

    return new Stockpile();
}

/**
 * Returns the plugins Ashita interface version it was compiled against.
 *
 * @return {double} The plugins interface version.
 * @notes
 *
 *      This export must be present for a plugin to load properly!
 */
__declspec(dllexport) double __stdcall expGetInterfaceVersion(void)
{
    return ASHITA_INTERFACE_VERSION;
}

#endif // SOCKPILE_H_INCLUDED