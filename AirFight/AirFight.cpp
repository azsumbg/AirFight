#include "pch.h"
#include "AirFight.h"

// EXCEPTION CLASS ************************

air::EXCEPTION::EXCEPTION(int what_happened) :_what{ what_happened } {};

const wchar_t* air::EXCEPTION::eGet()const
{
	switch (_what)
	{
	case ERR_INDEX:
		return L"Bad index passed to BAG container !";

	case ERR_PTR:
		return L"Bad pointer passed to BAG container !";

	case ERR_PARAM:
		return L"Bad parameter passed to BAG container !";

	case ERR_ITERATOR:
		return L"Bad iterator passed to BAG container !";
	}

	return L"Unknown error occurred in BAG container !";
}

///////////////////////////////////////////

// RANDIT CLASS ***************************

air::RANDIT::RANDIT()
{
	std::random_device rd{};
	std::seed_seq sq{ rd(),rd(), rd(), rd(), rd(), rd(), rd(), rd() };

	twister = new std::mt19937(sq);
}
air::RANDIT::~RANDIT()
{
	if (twister)delete twister;
}

int air::RANDIT::operator() (int min, int max)
{
	if (max < min)return -1;

	std::uniform_int_distribution distr(min, max);

	return distr(*twister);
}
float air::RANDIT::operator() (float min, float max)
{
	if (max < min)return -1.0f;

	std::uniform_real_distribution<float> distr(min, max);

	return distr(*twister);
}

//////////////////////////////////////////

//FADING CLASS ***************************

air::FADING::FADING(assets what, float sx, float sy)
{
	type = what;

	position.left = sx;
	position.right = sx + 32.0f;
	position.top = sy;
	position.bottom = sy + 32.0f;
}

float air::FADING::get_opacity()
{
	--delay;
	if (delay <= 0)
	{
		delay = max_delay;
		opacity -= 0.1f;
		if (opacity < 0)opacity = 0;
	}

	return opacity;
}

//////////////////////////////////////////