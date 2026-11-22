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
		EXCEPTION(int what_happened);

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


}