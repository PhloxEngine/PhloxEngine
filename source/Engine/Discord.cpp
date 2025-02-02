#include "Discord.h"
#include <time.h>
#include <chrono>
static int64_t eptime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();


void Discord::Initialize()
{
	DiscordEventHandlers Handle;
	memset(&Handle, 0, sizeof(Handle));
	Discord_Initialize("1155959383625318492", &Handle, 1, NULL); 
}

void Discord::Update()
{
	DiscordRichPresence discordPresence;
	memset(&discordPresence, 0, sizeof(discordPresence));
	discordPresence.state = "";
	discordPresence.details = "";
	discordPresence.startTimestamp = eptime;
	discordPresence.largeImageKey = "icon";
	discordPresence.largeImageText = "Phlox Engine by YoPhlox";
	//discordPresence.smallImageKey = "icon";
	Discord_UpdatePresence(&discordPresence);
}