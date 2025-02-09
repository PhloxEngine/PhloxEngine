#include "Discord.h"
#include "PhloxGame.h"
#include <chrono>
#include <cstring> 

static int64_t eptime = std::chrono::duration_cast<std::chrono::seconds>(
	std::chrono::system_clock::now().time_since_epoch()).count();

void Discord::Initialize(const GameInfo& gameInfo)
{
	if (!gameInfo.discord.enabled) {
		return;
	}

	if (m_initialized) {
		Discord_Shutdown();
	}
	
	DiscordEventHandlers Handle;
	memset(&Handle, 0, sizeof(Handle));
	
	Discord_Initialize(
		gameInfo.discord.applicationId.c_str(), 
		&Handle, 
		1,
		nullptr 
	);

	m_initialized = true;
}

void Discord::Update(const GameInfo& gameInfo)
{
	if (!gameInfo.discord.enabled || !m_initialized) {
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

void Discord::Shutdown()
{
	if (m_initialized) {
		Discord_Shutdown();
		m_initialized = false;
	}
}