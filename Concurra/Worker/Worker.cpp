#include "Worker.h"

Worker::Worker(Master* i_Master) : 
	m_Master(i_Master), thread{ &Worker::Run_, this }
{
}

void Worker::SetJob(std::span<int> i_Dataset, int* i_Result)
{
	{
		std::lock_guard lk{ mtx };
		pDataset = i_Dataset;
		pResult = i_Result;
	}
	m_cv.notify_one();
}

void Worker::Kill()
{
	{
		std::lock_guard lk{ mtx };
		dying = true;
	}
	m_cv.notify_one();
}

void Worker::Run_()
{
	std::unique_lock ul{ mtx };

	while (true)
	{
		m_cv.wait(ul, [this] { return pResult != nullptr || dying; });

		if (dying) { break; }

		if(pResult)
			dSet::ProcessDataset(pDataset, *pResult);

		pDataset = {};
		pResult = nullptr;
		m_Master->SignalDone();
	}
}
