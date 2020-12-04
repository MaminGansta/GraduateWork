
#include "thread_pool.h"


namespace Bubble
{
	ThreadPool::ThreadPool(size_t threads)
		: mStopping(false)
	{
		int size = threads ? threads : std::thread::hardware_concurrency();
		Resize(size);
	}

	ThreadPool::~ThreadPool()
	{
		Stop();
	}

	void ThreadPool::Stop() noexcept
	{
		mStopping = true;
		mEvent.notify_all();
		for (std::thread& thread : mPool)
		{
			thread.join();
		}
		mPool.clear();
	}

	void ThreadPool::Resize(int new_size)
	{
		if (new_size < Size())
		{
			mStopping = true;
			mEvent.notify_all();
			for (std::thread& thread : mPool)
			{
				thread.join();
			}
			mPool.clear();
			mStopping = false;
		}

		for (int i = Size(); i < new_size; i++)
		{
			mPool.push_back(std::thread([&]()
				{
					while (true)
					{
						std::function<void()> task;
						{
							std::unique_lock<std::mutex> lock(mEventMutex);

							mEvent.wait(lock, 
								[&]() { return mStopping || !mTasks.empty(); });

							if (mStopping && mTasks.empty()) {
								break;
							}
							task = std::move(mTasks.front());
							mTasks.pop();
						}
						task();
					}
				})
			);

		}

	}

}