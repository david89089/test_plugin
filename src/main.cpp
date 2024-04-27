#include <swiftly/swiftly.h>
#include <swiftly/server.h>
#include <swiftly/database.h>
#include <swiftly/commands.h>
#include <swiftly/configuration.h>
#include <swiftly/logger.h>
#include <swiftly/timers.h>
#include <swiftly/gameevents.h>

Server *server = nullptr;
PlayerManager *g_playerManager = nullptr;
Database *db = nullptr;
Commands *commands = nullptr;
Configuration *config = nullptr;
Logger *logger = nullptr;
Timers *timers = nullptr;
int isLRActive = false;


void OnProgramLoad(const char *pluginName, const char *mainFilePath)
{
    Swiftly_Setup(pluginName, mainFilePath);

    server = new Server();
    g_playerManager = new PlayerManager();
    commands = new Commands(pluginName);
    config = new Configuration();
    logger = new Logger(mainFilePath, pluginName);
    timers = new Timers();
}

void OnPluginStart()
{
    print("Plugin start URAAA");
}

void OnPlayerSpawn(Player* player) {
    player->SendMsg(HUD_PRINTTALK, "%s - возродился", player->GetName());
    player->weapons->RemoveWeapons();
}

void OnPlayerDeath(Player* player, Player* attacker, Player* assister, bool assistedflash, const char* weapon, bool headshot, short dominated, short revenge, short wipe, short penetrated, bool noreplay, bool noscope, bool thrusmoke, bool attackerblind, float distance, short dmg_health, short dmg_armor, short hitgroup)
{
    int countT = 0;
    g_playerManager->SendMsg(HUD_PRINTTALK, "Умер %s", player->GetName());

    if(!isLRActive) {
        g_playerManager->SendMsg(HUD_PRINTTALK, "Неактивно, игроков %d", g_playerManager->GetPlayers());
        for (int i = 0; i < g_playerManager->GetPlayers(); i++) { 
            Player* iPlayer = g_playerManager->GetPlayer(i);
            if(iPlayer->health->Get() > 0 && iPlayer->team->Get() == TEAM_T) {
                countT++;
                g_playerManager->SendMsg(HUD_PRINTTALK, "Живых Т игроков %d", countT);
            }
        }
    }

    if(countT == 2) {
        isLRActive = true;
        g_playerManager->SendMsg(HUD_PRINTTALK, "Начался LR");
    }
}

void OnRoundStart(long timelimit, long fraglimit, const char* objective)
{
    isLRActive = false;
}

const char *GetPluginAuthor()
{
    return "Dave";
}

const char *GetPluginVersion()
{
    return "1.0.0";
}

const char *GetPluginName()
{
    return "test_plugin";
}

const char *GetPluginWebsite()
{
    return "";
}