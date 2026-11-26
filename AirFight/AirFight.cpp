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

// FIELD CLASS ***************************

air::FIELD::FIELD(fields _what, float _sx, float _sy) :PROTON(_sx, _sy)
{
	type = _what;

	switch (type)
	{
	case fields::bridge:
		new_dims(1000.0f, 700.0f);
		max_frames = 3;
		frame_delay = 22;
		break;

	case fields::isle:
		new_dims(1000.0f, 700.0f);
		max_frames = 9;
		frame_delay = 7;
		break;

	case fields::river:
		new_dims(1000.0f, 700.0f);
		max_frames = 88;
		frame_delay = 1;
		break;

	case fields::intro:
		new_dims(1000.0f, 800.0f);
		max_frames = 71;
		frame_delay = 1;
		break;

	case fields::pause:
		new_dims(1000.0f, 800.0f);
		max_frames = 16;
		frame_delay = 4;
		break;

	case fields::small_cloud:
		new_dims(120.0f, 63.0f);
		speed = 0.8f;
		break;

	case fields::mid_cloud:
		new_dims(180.0f, 81.0f);
		speed = 0.7f;
		break;

	case fields::big_cloud:
		new_dims(200.0f, 79.0f);
		speed = 0.6f;
		break;
	}

	max_frame_delay = frame_delay;
}

fields air::FIELD::get_type() const
{
	return type;
}

int air::FIELD::get_frame()
{
	--frame_delay;

	if (frame_delay <= 0)
	{
		frame_delay = max_frame_delay;

		++frame;

		if (frame > max_frames)frame = 0;
	}

	return frame;
}
		
bool air::FIELD::move(dirs to_where, float gear)
{
	if (type == fields::intro || type == fields::pause)return true;

	float my_speed = speed + gear / 10.0f;

	switch (to_where)
	{
	case dirs::left:
		start.x -= my_speed;
		set_edges();
		if (end.x <= -scr_width)return false;
		break;

	case dirs::right:
		start.x += my_speed;
		set_edges();
		if (start.x >= 2.0f * scr_width)return false;
		break;

	case dirs::down:
		start.y += my_speed;
		set_edges();
		if (start.y >= 2.0f * scr_height)return false;
		break;

	case dirs::up:
		start.y -= my_speed;
		set_edges();
		if (end.y <= -scr_height)return false;
		break;
	}

	return true;
}

void air::FIELD::Release() 
{
	delete this;
}

air::FIELD* air::FIELD::create(fields what, float sx, float sy)
{
	return new FIELD(what, sx, sy);
}

//////////////////////////////////////////

// SHOT CLASS ****************************

air::SHOT::SHOT(ammo _what, float _sx, float _sy, float _ex, float _ey, int _damage) :PROTON(_sx, _sy)
{
	type = _what;
	damage = _damage;

	switch (type)
	{
	case ammo::bomb:
		new_dims(15.0f, 49.0f);
		speed = nature_speed + 0.3f;
		break;

	case ammo::bullet:
		new_dims(15.0f, 15.0f);
		speed = 5.0f;
		break;
	}

	if (type == ammo::bullet)set_path(_ex, _ey);
}

bool air::SHOT::move(float gear)
{
	float my_speed{ speed + gear / 5.0f };

	if (type == ammo::bomb)
	{
		--altitude_delay;

		if (altitude_delay <= 0)
		{
			altitude_delay = max_altitude_delay;
			altitude -= 0.1f;

			float width_specifier{ (rect.right - rect.left) * altitude };
			float height_specifier{ (rect.bottom - rect.top) * altitude };

			end.x = start.x + width_specifier;
			end.y = start.y + height_specifier;
			
			rect.right = end.x;
			rect.bottom = end.y;

			if (altitude <= 0)return false;
		}
	}
	else
	{
		start.x += move_next_x * my_speed;
		start.y += move_next_y * my_speed;

		set_edges();

		if (end.x <= 0 || start.x >= scr_width || end.y <= sky || start.y >= ground)return false;
	}

	return true;
}

ammo air::SHOT::get_type() const
{
	return type;
}

void air::SHOT::Release()
{
	delete this;
}

air::SHOT* air::SHOT::create(ammo what, float sx, float sy, float ex, float ey, int damage_init)
{
	return new SHOT(what, sx, sy, ex, ey, damage_init);
}

//////////////////////////////////////////

// EXPLOSION CLASS ***********************

air::EXPLOSION::EXPLOSION(float _sx, float _sy) :PROTON(_sx, _sy, 100.0f, 114.0f) {};
int air::EXPLOSION::get_frame()
{
	--frame_delay;
	if (frame_delay <= 0)
	{
		frame_delay = max_frame_delay;
		++frame;
		if (frame > 23)return EXPLOSION_ENDED;
	}

	return frame;
}

void air::EXPLOSION::Release()
{
	delete this;
}

air::EXPLOSION* air::EXPLOSION::create(float sx, float sy)
{
	return new EXPLOSION(sx, sy);
}

///////////////////////////////////////////

// HERO CLASS ****************************

air::HERO::HERO(float sx, float sy) :PROTON(sx, sy, 84.0f, 100.0f) 
{
	set_path(center.x, sky);
};

void air::HERO::set_angle(dirs rotation_dir)
{
	if (rotation_dir == dirs::left)
	{
		if (move_ex - 10.0f > 0) set_path(move_ex - 10.0f, move_ey);
		else
		{
			set_path(0, move_ey);
			fly_angle = 270.0f;
			return;
		}

		if (move_ex == center.x)
		{
			fly_angle = 0;
			return;
		}
		else if (move_ex > center.x)
		{
			float opposite{ 0 };
			float adjanced{ 0 };

			opposite = move_ex - center.x;
			adjanced = center.y - sky;

			fly_angle = std::atan2f(opposite, adjanced) + (180.0f / 3.1415);
		}
		else if (move_ex < center.x)
		{
			float opposite{ 0 };
			float adjanced{ 0 };

			opposite = center.x - move_ex;
			adjanced = center.y - sky;

			fly_angle = std::atan2f(opposite, adjanced) + (180.0f / 3.1415);
			fly_angle += 270.0f;
		}
	}
	else if (rotation_dir == dirs::right)
	{
		if (move_ex + 10.0f < scr_width) set_path(move_ex + 10.0f, move_ey);
		else
		{
			set_path(scr_width, move_ey);
			fly_angle = 90.0f;
			return;
		}

		if (move_ex == center.x)
		{
			fly_angle = 0;
			return;
		}
		else if (move_ex > center.x)
		{
			float opposite{ 0 };
			float adjanced{ 0 };

			opposite = move_ex - center.x;
			adjanced = center.y - sky;

			fly_angle = std::atan2f(opposite, adjanced) + (180.0f / 3.1415);
		}
		else if (move_ex < center.x)
		{
			float opposite{ 0 };
			float adjanced{ 0 };

			opposite = center.x - move_ex;
			adjanced = center.y - sky;

			fly_angle = std::atan2f(opposite, adjanced) + (180.0f / 3.1415);
			fly_angle += 270.0f;
		}
	}
}

float air::HERO::get_angle() const
{
	return fly_angle;
}

void air::HERO::move(float gear)
{
	float my_speed = speed * 4.0f + gear / 5.0f;

	start.x += move_next_x * my_speed;
	start.y += move_next_y * my_speed;
	set_edges();

	if (start.x < 0)
	{
		start.x = 0;
		set_edges();
	}
	if (end.x > scr_width)
	{
		end.x = scr_width;
		start.x = end.x - _width;
		set_edges();
	}
	if (start.y < sky)
	{
		start.y = sky;
		set_edges();
	}
	if (end.y > ground)
	{
		end.y = ground;
		start.y = end.y - _height;
		set_edges();
	}
}

/////////////////////////////////////////

// EVILS CLASS **************************

air::EVILS::EVILS(evils _what, float _sx, float _sy) :PROTON(_sx, _sy)
{
	type = _what;

	switch (type)
	{
	case evils::amphibia:
		new_dims(120.0f, 58.0f);
		speed = 0.6f;
		lifes = 100;
		damage = 10;
		armor = 1;
		max_frames = 5;
		frame_delay = 14;
		attack_delay = 120;
		break;

	case evils::bomber:
		new_dims(100.0f, 152.0f);
		speed = 0.7f;
		lifes = 120;
		damage = 15;
		armor = 1;
		max_frames = 5;
		frame_delay = 14;
		attack_delay = 150;
		break;

	case evils::boss1:
		new_dims(300.0f, 300.0f);
		speed = 0.4f;
		lifes = 250;
		damage = 25;
		armor = 3;
		max_frames = 3;
		frame_delay = 20;
		attack_delay = 180;
		break;

	case evils::boss2:
		new_dims(300.0f, 215.0f);
		speed = 0.4f;
		lifes = 250;
		damage = 25;
		armor = 3;
		max_frames = 1;
		frame_delay = 60;
		attack_delay = 180;
		break;

	case evils::small_fighter:
		new_dims(42.0f, 80.0f);
		speed = 0.9f;
		lifes = 80;
		damage = 5;
		armor = 0;
		max_frames = 9;
		frame_delay = 14;
		attack_delay = 80;
		break;

	case evils::fighter:
		new_dims(76.0f, 90.0f);
		speed = 0.8f;
		lifes = 90;
		damage = 8;
		armor = 1;
		max_frames = 60;
		frame_delay = 1;
		attack_delay = 90;
		break;
	}

	max_attack_delay = attack_delay;
	max_frame_delay = frame_delay;
}

bool air::EVILS::move(float gear)
{
	float my_speed = speed + gear / 10.0f;

	start.x += move_next_x * my_speed;
	start.y += move_next_y * my_speed;
	set_edges();

	if (start.x < 0)
	{
		start.x = 0;
		set_edges();
	}
	if (end.x > scr_width)
	{
		end.x = scr_width;
		start.x = end.x - _width;
		set_edges();
	}
	if (start.y < sky)
	{
		start.y = sky;
		set_edges();
	}
	if (end.y > ground)
	{
		end.y = ground;
		start.y = end.y - _height;
		set_edges();
	}

	return true;
}

void air::EVILS::set_fly_angle(float sx, float sy)
{
	float oppos{ 0 };
	float adjanced{ ground - center.y };

	if (move_ex == center.x)
	{
		fly_angle = 0;
		return;
	}
	else if (move_ex < center.x)oppos = center.x - move_ex;
	else if (move_ex > center.x)oppos = move_ex - center.x;
	
	if (move_ey == center.y)
	{
		if (move_ex == center.x)fly_angle = 0;
		else if (move_ex < center.x)fly_angle = 90.0f;
		else if (move_ex > center.x)fly_angle = 270.0f;
		
		return;
	}

	fly_angle = std::atan2f(oppos, adjanced) + (180.0f / 3.1415);
}
float air::EVILS::get_fly_angle() const
{
	return fly_angle;
}

int air::EVILS::get_frame()
{
	--frame_delay;

	if (frame_delay <= 0)
	{
		frame_delay = max_frame_delay;
		++frame;
		if (frame > max_frames)frame = 0;
	}
	
	return frame;
}

int air::EVILS::attack()
{
	--attack_delay;
	if (attack_delay <= 0)
	{
		attack_delay = max_attack_delay;
		return damage;
	}
	return 0;
}

void air::EVILS::Release()
{
	delete this;
}

air::EVILS* air::EVILS::create(evils what, float sx, float sy)
{
	return new EVILS(what, sx, sy);
}

/////////////////////////////////////////