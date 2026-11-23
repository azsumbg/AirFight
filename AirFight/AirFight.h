#pragma once

#ifdef AIRFIGHT_EXPORTS
#define AIRFIGHT_API __declspec(dllexport)
#else 
#define AIRFIGHT_API _declspec(dllimport)
#endif

#include <random>
#include <iterator>
#include <d2d1.h>

#pragma comment(lib, "d2d1.lib")

constexpr float scr_width{ 1000.0f };
constexpr float scr_height{ 800.0f };
constexpr float sky{ 50.0f };
constexpr float ground{ 750.0f };

constexpr D2D1_RECT_F FULL_SCREEN{ 0.0f, 0.0f, scr_width, scr_height };
constexpr D2D1_RECT_F GAME_SCREEN{ 0.0f, sky, scr_width, ground };

constexpr int ERR_PTR{ 6001 };
constexpr int ERR_PARAM{ 6002 };
constexpr int ERR_INDEX{ 6003 };
constexpr int ERR_UNKNOWN{ 6004 };
constexpr int ERR_ITERATOR{ 6005 };

enum class dirs { stop = 0, left = 1, right = 2, up = 3, down = 4, up_left = 5, up_right = 6, down_left = 7, down_right = 8 };

enum class assets { armor = 0, gun = 1, points = 2, repair = 3 };
enum class evils { amphibia = 0, bomber = 1, small_fighter = 2, fighter = 3, boss1 = 4, boss2 = 5 };
enum class fields { bridge = 0, isle = 1, river = 2, small_cloud = 3, mid_cloud = 4, big_cloud = 5, intro = 6, pause = 7 };

enum class ammo { bomb = 0, bullet = 1 };

namespace air
{
	class AIRFIGHT_API EXCEPTION
	{
	private:
		int _what{ ERR_UNKNOWN };

	public:
		EXCEPTION(int what_happened = ERR_UNKNOWN);

		const wchar_t* eGet()const;
	};
	class AIRFIGHT_API RANDIT
	{
	private:
		std::mt19937* twister{ nullptr };

	public:
		RANDIT();
		~RANDIT();

		int operator() (int min, int max);
		float operator() (float min, float max);
	};
	class AIRFIGHT_API FADING
	{
	private:
		float opacity{ 1.0f };
		int delay{ 80 };
		int max_delay{ delay };

	public:
		assets type{ assets::points };
		D2D1_RECT_F position{};

		FADING(assets what, float sx, float sy);

		float get_opacity();
	};

	template<typename T> class BAG
	{
	private:
		size_t max_size{ 1 };
		T* mPtr{ nullptr };
		size_t next_pos{ 0 };

	public:

		BAG()
		{
			mPtr = reinterpret_cast<T*>(calloc(max_size, sizeof(T)));

			if(!mPtr)throw EXCEPTION()
		}
		BAG(size_t lenght)
		{
			max_size = lenght;
			mPtr = reinterpret_cast<T*>(calloc(max_size, sizeof(T)));
			if (!mPtr)throw EXCEPTION();
		}
		BAG(BAG& other)
		{
			if (!other.mPtr)
			{
				mPtr = reinterpret_cast<T*>(calloc(max_size, sizeof(T)));
				if (!mPtr)throw EXCEPTION();
			}
			else
			{
				max_size = other.max_size;
				next_pos = other.next_pos;

				mPtr = reinterpret_cast<T*>(calloc(max_size, sizeof(T)));

				if (!mPtr)throw EXCEPTION();
				else if (next_pos > 0)for (size_t i = 0; i < next_pos; ++i)mPtr[i] = other.mPtr[i];
			}
		}
		BAG(BAG&& other)
		{
			if (!other.mPtr)
			{
				mPtr = reinterpret_cast<T*>(calloc(max_size, sizeof(T)));

				if (!mPtr)throw EXCEPTION();
			}
			else
			{
				max_size = other.max_size;
				next_pos = other.next_pos;

				mPtr = other.mPtr;

				other.mPtr = nullptr;
			}
		}

		BAG& operator= (BAG& other)
		{
			if (!other.mPtr)throw EXCEPTION(ERR_PARAM);
			else
			{
				if (mPtr == other.mPtr)return (*this);
				else
				{
					free(mPtr);

					max_size = other.max_size;
					next_pos = other.next_pos;

					mPtr = reinterpret_cast<T*>(calloc(max_size, sizeof(T)));

					if (!mPtr)throw EXCEPTION(ERR_PTR);
					else if (next_pos > 0)for (size_t i = 0; i < next_pos; ++i)mPtr[i] = other.mPtr[i];
				}
			}

			return (*this);
		}
		BAG& operator= (BAG&& other)
		{
			if (!other.mPtr)throw EXCEPTION(ERR_PARAM);
			else
			{
				if (mPtr == other.mPtr)return (*this);
				else
				{
					free(mPtr);

					max_size = other.max_size;
					next_pos = other.next_pos;

					mPtr = other.mPtr;
					other.mPtr = nullptr;
				}
			}

			return (*this);
		}

		T& operator[] (size_t index)
		{
			if (!mPtr)throw EXCEPTION(ERR_PTR);
			else if (index >= next_pos)throw EXCEPTION(ERR_INDEX);
			
			return mPtr[index];
		}

		bool empty()const
		{
			return(next_pos == 0);
		}
		size_t size()const
		{
			return next_pos;
		}
		size_t capacity()const
		{
			return max_size;
		}

		void clear()
		{
			free(mPtr);

			max_size = 1;
			next_pos = 0;

			mPtr = reinterpret_cast<T*>(calloc(max_size, sizeof(T)));

			if (!mPtr)throw EXCEPTION();
		}

		void push_front(T element)
		{
			if (!mPtr)throw EXCEPTION(ERR_PTR);
			else
			{
				if (next_pos + 1 <= max_size)
				{
					for (size_t index = next_pos; index > 0; --index)mPtr[index] = mPtr[index - 1];
					
					*mPtr = element;
					
					++next_pos;
				}
				else
				{
					++max_size;

					T* temp{ reinterpret_cast<T*>(realloc(mPtr, max_size * sizeof(T))) };
					if (!temp)throw EXCEPTION();
					else
					{
						mPtr = temp;
						temp = nullptr;

						if (mPtr)
						{
							for (size_t index = next_pos; index > 0; --index)mPtr[index] = mPtr[index - 1];

							*mPtr = element;

							++next_pos;
						}
					}
				}
			}
		}
		void push_front(T* element)
		{
			if (!mPtr)throw EXCEPTION(ERR_PTR);
			else
			{
				if (next_pos + 1 <= max_size)
				{
					for (size_t index = next_pos; index > 0; --index)mPtr[index] = mPtr[index - 1];

					*mPtr = *element;

					++next_pos;
				}
				else
				{
					++max_size;

					T* temp{ reinterpret_cast<T*>(realloc(mPtr, max_size * sizeof(T))) };
					if (!temp)throw EXCEPTION();
					else
					{
						mPtr = temp;
						temp = nullptr;

						if (mPtr)
						{
							for (size_t index = next_pos; index > 0; --index)mPtr[index] = mPtr[index - 1];

							*mPtr = *element;

							++next_pos;
						}
					}
				}
			}
		}

		void push_back(T element)
		{
			if (!mPtr)throw EXCEPTION();
			else
			{
				if (next_pos + 1 <= max_size)
				{
					mPtr[next_pos] = element;
					++next_pos;
				}
				else
				{
					++max_size;

					T* temp{ reinterpret_cast<T * (realloc(mPtr, max_size * sizeof(T))) };

					if (!temp)throw EXCEPTION();
					else
					{
						mPtr = temp;
						temp = nullptr;

						if (mPtr)mPtr[next_pos] = element;

						++next_pos;
					}
				}
			}
		}
		void push_back(T* element)
		{
			if (!mPtr)throw EXCEPTION();
			else
			{
				if (next_pos + 1 <= max_size)
				{
					mPtr[next_pos] = *element;
					++next_pos;
				}
				else
				{
					++max_size;

					T* temp{ reinterpret_cast<T * (realloc(mPtr, max_size * sizeof(T))) };

					if (!temp)throw EXCEPTION();
					else
					{
						mPtr = temp;
						temp = nullptr;

						if (mPtr)mPtr[next_pos] = *element;

						++next_pos;
					}
				}
			}
		}

		void erase(size_t index)
		{
			if (!mPtr)throw EXCEPTION();
			else
			{
				if (index >= next_pos)throw EXCEPTION(ERR_PARAM);

				for (size_t i = index; i < next_pos; ++i)mPtr[i] = mPtr[i + 1];

				--max_size;
				--next_pos;

				T* temp{ reinterpret_cast<T*>(realloc(mPtr, max_size * sizeof(T))) };

				if (!temp)throw EXCEPTION();
				else
				{
					mPtr = temp;
					temp = nullptr;
				}
			}
		}

	};


}