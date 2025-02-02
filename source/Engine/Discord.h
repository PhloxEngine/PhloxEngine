#pragma once
#include <discord_register.h> 
#include <discord_rpc.h> 
#include <Windows.h> 
#include "PhloxGame.h"

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

private:
	Discord() {} 
	Discord(const Discord&) = delete;
	Discord& operator=(const Discord&) = delete;
};