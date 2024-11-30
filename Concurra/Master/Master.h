#pragma once
#include "../config.h"

class Master
{
public:
	Master(int i_Workers);

private:
	int workerCount;
	int doneCount = 0;
	std::condition_variable m_cv;
	std::mutex mtx;

public:
	void SignalDone();
	void WaitUntilAllDone();
};