#include "Discord.h"
#include "PhloxGame.h"
#include <time.h>
#include <chrono>

static int64_t eptime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();

void Discord::Initialize(const GameInfo& gameInfo)
{
	if (!gameInfo.discord.enabled) {
		return;
	}

	Discord_Shutdown(); 
	
	DiscordEventHandlers Handle;
	memset(&Handle, 0, sizeof(Handle));
	Discord_Initialize(gameInfo.discord.applicationId.c_str(), &Handle, 1, NULL);
}

void Discord::Update(const GameInfo& gameInfo)
{
	if (!gameInfo.discord.enabled) {
		return;
	}

	DiscordRichPresence discordPresence;
	memset(&discordPresence, 0, sizeof(discordPresence));
	discordPresence.state = gameInfo.discord.state.c_str();
	discordPresence.details = gameInfo.discord.details.c_str();
	discordPresence.startTimestamp = eptime;
	discordPresence.largeImageKey = gameInfo.discord.largeImageKey.c_str();
	discordPresence.largeImageText = gameInfo.discord.largeImageText.c_str();
	
	if (!gameInfo.discord.smallImageKey.empty()) {
		discordPresence.smallImageKey = gameInfo.discord.smallImageKey.c_str();
		discordPresence.smallImageText = gameInfo.discord.smallImageText.c_str();
	}
	
	Discord_UpdatePresence(&discordPresence);
}