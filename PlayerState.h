#pragma once
class PlayerState
{
public:
	PlayerState();
	~PlayerState();
	void updateState(const char c);
	char getState();

private:
	static char currentState;
};

