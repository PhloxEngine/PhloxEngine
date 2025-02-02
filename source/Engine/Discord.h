#pragma once
#include <discord_register.h> 
#include <discord_rpc.h> 
#include <Windows.h> 

class Discord {
public:
	static Discord& GetInstance() {
		static Discord instance;
		return instance;
	}

	void Initialize();
	void Update();

private:
	Discord() {} 
	Discord(const Discord&) = delete;
	Discord& operator=(const Discord&) = delete;
};