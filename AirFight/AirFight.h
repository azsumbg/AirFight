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

constexpr float nature_speed{ 0.5f };

constexpr D2D1_RECT_F FULL_SCREEN{ 0.0f, 0.0f, scr_width, scr_height };
constexpr D2D1_RECT_F GAME_SCREEN{ 0.0f, sky, scr_width, ground };

constexpr int ERR_PTR{ 6001 };
constexpr int ERR_PARAM{ 6002 };
constexpr int ERR_INDEX{ 6003 };
constexpr int ERR_UNKNOWN{ 6004 };
constexpr int ERR_ITERATOR{ 6005 };

constexpr int EXPLOSION_ENDED{ -1 };

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

		void insert(T element, size_t position)
		{
			if (!mPtr)throw EXCEPTION(ERR_PTR);
			else
			{
				if (position == 0)push_front(element);
				else if (position >= next_pos)push_back(element);
				else
				{
					if (next_pos + 1 <= max_size)
					{
						for (size_t i = next_pos; i > position; --i)mPtr[i] = mPtr[i - 1];

						mPtr[position] = element;

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
								for (size_t i = next_pos; i > position; --i)mPtr[i] = mPtr[i - 1];

								mPtr[position] = element;

								++next_pos;
							}
						}
					}
				}
			}
		}
		void insert(T* element, size_t position)
		{
			if (!mPtr)throw EXCEPTION(ERR_PTR);
			else
			{
				if (position == 0)push_front(element);
				else if (position >= next_pos)push_back(element);
				else
				{
					if (next_pos + 1 <= max_size)
					{
						for (size_t i = next_pos; i > position; --i)mPtr[i] = mPtr[i - 1];

						mPtr[position] = *element;

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
								for (size_t i = next_pos; i > position; --i)mPtr[i] = mPtr[i - 1];

								mPtr[position] = *element;

								++next_pos;
							}
						}
					}
				}
			}
		}

		class iterator
		{
		private:
			T* it_ptr{ nullptr };
			T* container_ptr{ nullptr };
			size_t container_size{ 1 };

		public:
			friend class BAG<T>;

			using iterator_category = std::contiguous_iterator_tag;
			using value_type = T;
			using difference_type = ptrdiff_t;
			using pointer = T*;
			using reference = T&;

			iterator(T* init_ptr, BAG<T>& container)
			{
				it_ptr = init_ptr;
				container_ptr = container.mPtr;
				container_size = container.next_pos;
			}

			pointer operator-> () const
			{
				return it_ptr;
			}
			reference operator* () const
			{
				return *it_ptr;
			}

			friend operator== (const iterator& first, const iterator& second)
			{
				return(first.it_ptr == second.it_ptr);
			}
			friend operator>= (const iterator& first, const iterator& second)
			{
				return(first.it_ptr >= second.it_ptr);
			}
			friend operator<= (const iterator& first, const iterator& second)
			{
				return(first.it_ptr <= second.it_ptr);
			}
			friend operator> (const iterator& first, const iterator& second)
			{
				return(first.it_ptr > second.it_ptr);
			}
			friend operator< (const iterator& first, const iterator& second)
			{
				return(first.it_ptr < second.it_ptr);
			}

			iterator& operator++ ()
			{
				if (it_ptr >= container_ptr + container_size)throw EXCEPTION(ERR_ITERATOR);

				++it_ptr;

				return (*this);
			}
			iterator operator++ (int)
			{
				iterator temp{ (*this) };

				if (it_ptr >= container_ptr + container_size)throw EXCEPTION(ERR_ITERATOR);

				++(*this);

				return temp;
			}

			iterator& operator-- ()
			{
				if (it_ptr < container_ptr)throw EXCEPTION(ERR_ITERATOR);

				--it_ptr;

				return (*this);
			}
			iterator operator-- (int)
			{
				iterator temp{ (*this) };

				if (it_ptr < container_ptr)throw EXCEPTION(ERR_ITERATOR);

				--(*this);

				return temp;
			}

			iterator& operator+ (size_t step)
			{
				if (it_ptr >= container_ptr + container_size)throw EXCEPTION(ERR_ITERATOR);

				it_ptr += step;

				return (*this);
			}
			iterator& operator+= (size_t step)
			{
				if (it_ptr >= container_ptr + container_size)throw EXCEPTION(ERR_ITERATOR);

				it_ptr += step;

				return (*this);
			}

			iterator& operator- (size_t step)
			{
				if (it_ptr < container_ptr)throw EXCEPTION(ERR_ITERATOR);

				it_ptr -= step;

				return (*this);
			}
			iterator& operator-= (size_t step)
			{
				if (it_ptr < container_ptr)throw EXCEPTION(ERR_ITERATOR);

				it_ptr -= step;

				return (*this);
			}
		};

		iterator begin()
		{
			return iterator(mPtr, (*this));
		}
		iterator end()
		{
			return iterator(mPtr + next_pos, (*this));
		}

		void erase(iterator which_element)
		{
			for (size_t i = 0; i < next_pos; ++i)
			{
				if (mPtr + i == which_element.it_ptr)
				{
					erase(i);
					return;
				}
			}

			throw EXCEPTION(ERR_ITERATOR);
		}
	};

	class AIRFIGHT_API PROTON
	{
	protected:
		float _width{ 1.0f };
		float _height{ 1.0f };

		float move_sx{ 0 };
		float move_ex{ 0 };

		float move_sy{ 0 };
		float move_ey{ 0 };

		float move_next_x{ 1.0f };
		float move_next_y{ 1.0f };

		float speed{ nature_speed };

	public:
		D2D1_POINT_2F start{ 0 };
		D2D1_POINT_2F end{ 0 };
		D2D1_POINT_2F center{ 0 };

		D2D1_RECT_F rect{};

		float x_rad{ 0 };
		float y_rad{ 0 };

		PROTON(float sx = 0, float sy = 0);
		PROTON(float sx, float sy, float first_width, float first_height);
		PROTON(D2D1_POINT_2F start_point, float first_width, float first_height);

		virtual ~PROTON() {};

		void set_edges();
		void set_width(float new_width);
		void set_height(float new_height);
		void new_dims(float new_width, float new_height);

		void set_path(float ex, float ey);

		float get_width()const;
		float get_height()const;

		float get_move_ex()const;
		float get_move_ey()const;
	};

	class AIRFIGHT_API FIELD :public PROTON
	{
	private:
		fields type{ fields::intro };

		int frame{ 0 };
		int frame_delay{ 0 };
		int max_frames{ 0 };
		int max_frame_delay{ 0 };

		FIELD(fields _what, float _sx, float _sy);

	public:

		fields get_type() const;

		int get_frame();

		bool move(dirs to_where, float gear);

		void Release();

		static FIELD* create(fields what, float sx, float sy);
	};

	class AIRFIGHT_API SHOT :public PROTON
	{
	private:
		ammo type{ ammo::bullet };

		float altitude{ 1.0f };

		int altitude_delay{ 40 };
		int max_altitude_delay{ 40 };

		SHOT(ammo _what, float _sx, float _sy, float _ex, float _ey, int _damage);

	public:
		int damage{ 0 };
		bool explode = false;

		bool move(float gear);

		ammo get_type() const;

		void Release();

		static SHOT* create(ammo what, float sx, float sy, float ex, float ey, int damage_init);
	};

	class AIRFIGHT_API EXPLOSION: public PROTON
	{
	private:
		int frame{ 0 };
		int frame_delay{ 3 };
		int max_frame_delay{ 3 };

		EXPLOSION(float _sx, float _sy);

	public:
		int get_frame();

		void Release();

		static EXPLOSION* create(float sx, float sy);
	};

	class AIRFIGHT_API HERO :public PROTON
	{
	private:
		int frame{ 0 };
		int max_frames{ 5 };
		int frame_delay{ 12 };

		float fly_angle{ 0 };

	public:
		int armor{ 1 };
		int damage{ 5 };
		int lifes{ 100 };

		HERO(float sx, float sy);

		void set_angle(dirs rotation_dir);

		float get_angle() const;

		void move(float gear);
	};

	class AIRFIGHT_API EVILS: public PROTON
	{
	private:
		int frame{ 0 };
		int max_frames{ 0 };
		int frame_delay{ 0 };
		int max_frame_delay{ 0 };

		int attack_delay{ 0 };
		int max_attack_delay{ 0 };

		float fly_angle{ 0 };

		EVILS(evils _what, float _sx, float _sy);

	public:
		evils type{ evils::amphibia };
		int lifes{ 0 };
		int damage{ 0 };
		int armor{ 0 };

		bool move(float gear);

		void set_fly_angle(float sx, float sy);
		float get_fly_angle() const;

		int get_frame();

		int attack();

		void Release();

		static EVILS* create(evils what, float sx, float sy);
	};



}