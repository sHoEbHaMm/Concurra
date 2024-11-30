#pragma once
#include "../config.h"
#include "../Dataset.h"
#include "Master.h"


class Worker
{
public:
	Worker(Master* i_Master);

public:
	void SetJob(std::span<int> i_Dataset, int* i_Result);
	void Kill();

private:
	Master* m_Master;
	std::jthread thread;
	std::condition_variable m_cv;
	std::mutex mtx;

	/* Shared memory */
	std::span<int> pDataset;
	int* pResult = nullptr;
	bool dying = false;

	/* Run Thread */
	void Run_();
};