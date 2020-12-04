#pragma once

#include <thread>
#include <vector>
#include <condition_variable>
#include <functional>
#include <queue>
#include <future>
#include <cassert>
#include <algorithm>


namespace Bubble
{
	struct ThreadPool
	{
		std::vector<std::thread> mPool;
		std::queue<std::function<void()>> mTasks;

		std::condition_variable mEvent;
		std::mutex mEventMutex;
		bool mStopping;

		ThreadPool(size_t threads = 0);
		~ThreadPool();

		void Resize(int size);
		void Stop() noexcept;
		int Size() { return mPool.size(); };

		template <typename Func, typename ...Args>
		auto add_task(Func&& task, Args&& ...args) -> std::future<decltype(task(args...))>
		{
			auto wrapper = std::make_shared<std::packaged_task<decltype(task(args...))()>>(
				std::bind(std::forward<Func>(task), std::forward<Args>(args)...));
			{
				std::unique_lock<std::mutex> lock(mEventMutex);
				mTasks.push([wrapper]() { (*wrapper)(); });
			}
			mEvent.notify_one();
			return wrapper->get_future();
		}

		template <typename Func>
		auto parallel_for(int from_param, int to_param, Func&& func) -> std::vector<std::future<decltype(func(1, 1))>>
		{
			std::vector<std::future<decltype(func(1, 1))>> res;
			res.reserve(Size());

			int width = to_param - from_param;
			int threads = std::min(width, Size());

			for (int i = 0; i < threads; i++)
			{
				int from = i * width / threads + from_param;
				int to = (i + 1) * width / threads + from_param;
				res.push_back(std::move(add_task(func, from, to)));
			}
			return res;
		}

	};

}