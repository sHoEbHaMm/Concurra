#include "Master.h"

Master::Master(int i_Workers) : 
	workerCount(i_Workers)
{
}

void Master::SignalDone()
{
	{
		std::lock_guard lk{ mtx };
		doneCount++;
	}
	if (doneCount == workerCount) { m_cv.notify_one(); }
}

void Master::WaitUntilAllDone()
{
	{
		std::unique_lock ul { mtx };
		m_cv.wait(ul, [this] {return doneCount == workerCount; });
		doneCount = 0;
	}
}
