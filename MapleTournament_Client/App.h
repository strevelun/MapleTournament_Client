#pragma once

class App
{
public:
	App();
	~App();

	virtual int Run() = 0;
	virtual void Update() = 0;
};

