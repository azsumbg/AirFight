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

// FADING CLASS ***************************

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

// PROTON CLASS **************************

air::PROTON::PROTON(float sx = 0, float sy = 0)
{
	start.x = sx;
	start.y = sy;

	x_rad = _width / 2.0f;
	y_rad = _height / 2.0f;

	center.x = start.x + x_rad;
	center.y = start.y + y_rad;

	end.x = start.x + _width;
	end.y = start.y + _height;

	rect.left = start.x;
	rect.top = start.y;
	rect.right = end.x;
	rect.bottom = end.y;
}
air::PROTON::PROTON(float sx, float sy, float first_width, float first_height)
{
	start.x = sx;
	start.y = sy;

	_width = first_width;
	_height = first_height;

	x_rad = _width / 2.0f;
	y_rad = _height / 2.0f;

	center.x = start.x + x_rad;
	center.y = start.y + y_rad;

	end.x = start.x + _width;
	end.y = start.y + _height;

	rect.left = start.x;
	rect.top = start.y;
	rect.right = end.x;
	rect.bottom = end.y;
}
air::PROTON::PROTON(D2D1_POINT_2F start_point, float first_width, float first_height)
{
	start.x = start_point.x;
	start.y = start_point.y;

	_width = first_width;
	_height = first_height;

	x_rad = _width / 2.0f;
	y_rad = _height / 2.0f;

	center.x = start.x + x_rad;
	center.y = start.y + y_rad;

	end.x = start.x + _width;
	end.y = start.y + _height;

	rect.left = start.x;
	rect.top = start.y;
	rect.right = end.x;
	rect.bottom = end.y;
}

void air::PROTON::set_edges()
{
	center.x = start.x + x_rad;
	center.y = start.y + y_rad;

	end.x = start.x + _width;
	end.y = start.y + _height;

	rect.left = start.x;
	rect.top = start.y;
	rect.right = end.x;
	rect.bottom = end.y;
}
void air::PROTON::set_width(float new_width)
{
	_width = new_width;
	
	x_rad = _width / 2.0f;
	
	center.x = start.x + x_rad;
	
	end.x = start.x + _width;
	
	rect.right = end.x;
}
void air::PROTON::set_height(float new_height)
{
	_height = new_height;

	y_rad = _height / 2.0f;

	center.y = start.y + y_rad;

	end.y = start.y + _height;

	rect.bottom = end.y;
}
void air::PROTON::new_dims(float new_width, float new_height)
{
	_width = new_width;
	_height = new_height;

	x_rad = _width / 2.0f;
	y_rad = _height / 2.0f;

	center.x = start.x + x_rad;
	center.y = start.y + y_rad;

	end.x = start.x + _width;
	end.y = start.y + _height;

	rect.left = start.x;
	rect.top = start.y;
	rect.right = end.x;
	rect.bottom = end.y;
}

void air::PROTON::set_path(float ex, float ey)
{
	move_sx = start.x;
	move_sy = start.y;

	move_ex = ex;
	move_ey = ey;

	float delta_x = move_ex - move_sx;
	float delta_y = move_ey - move_sy;

	float vec_lenght = (float)(sqrt(pow(delta_x, 2) + pow(delta_y, 2)));

	move_next_x = delta_x / vec_lenght;
	move_next_y = delta_y / vec_lenght;
}

float air::PROTON::get_width()const
{
	return _width;
}
float air::PROTON::get_height()const
{
	return _height;
}

float air::PROTON::get_move_ex()const
{
	return move_ex;
}
float air::PROTON::get_move_ey()const
{
	return move_ey;
}

//////////////////////////////////////////