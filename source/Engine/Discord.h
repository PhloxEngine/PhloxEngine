#pragma once
#include <discord_register.h> 
#include <discord_rpc.h> 

#ifdef _WIN32
#include <Windows.h> 
#endif

#include "PhloxGame.h"

#ifndef DISCORD_EXPORT
#if defined(_WIN32)
#define DISCORD_EXPORT __declspec(dllexport)
#else
#define DISCORD_EXPORT __attribute__((visibility("default")))
#endif
#endif

class Discord {
public:
	static Discord& GetInstance() {
		static Discord instance;
		return instance;
	}

	void Initialize() {
		GameInfo defaultInfo;
		Initialize(defaultInfo);
	}

	void Initialize(const GameInfo& gameInfo);
	void Update(const GameInfo& gameInfo);
	void Shutdown();

private:
	Discord() {} 
	Discord(const Discord&) = delete;
	Discord& operator=(const Discord&) = delete;
	bool m_initialized = false;
};