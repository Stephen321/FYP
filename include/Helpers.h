#pragma once
#include <vector>
#include "SFML\Graphics.hpp"
#include "ObjectType.h"

#define _USE_MATH_DEFINES
#include <math.h>

namespace Helpers
{
	inline int clamp(int value, int min, int max)
	{
		if (value < 0)
		{
			return max - 1;
		}
		if (value >= max)
		{
			return 0;
		}

		return value;
	}

	inline float getAngle(const sf::Vector2f& v)
	{
		return atan2(v.y, v.x) * (180.f / M_PI);
	}

	inline float getAngle(const sf::Vector2f& v1, const sf::Vector2f& v2)
	{
		sf::Vector2f v = v2 - v1; 
		return atan2(v.y, v.x) * (180.f / M_PI);
	}

	inline float getLength(const sf::Vector2f & v)
	{
		float length = sqrt(v.x * v.x + v.y * v.y);
		return length;
	}

	inline std::string objectTypeToString(ObjectType type)
	{
		std::string name;
		switch (type)
		{
		case ObjectType::None:
			name = "None";
			break;
		case ObjectType::Unit:
			name = "Unit";
			break;
		case ObjectType::Wall:
			name = "Wall";
			break;
		case ObjectType::Bullet:
			name = "Bullet";
			break;
		case ObjectType::Ammo:
			name = "Ammo";
			break;
		}
		return name;
	}


	inline sf::Vector2f normaliseCopy(const sf::Vector2f & v)
	{
		sf::Vector2f n;
		float length = getLength(v);
		if (length != 0)
		{
			n.x = v.x / length;
			n.y = v.y / length;
		}
		return n;
	}

	inline bool valueInRange(int value, int min, int max)
	{
		return (value >= min) && (value <= max);
	}

	inline float lerp(float start, float end, float t)
	{
		if (t > 1.f)
			t = 1.f;
		return start*(1 - t) + end*t;
	}

	inline void normalise(sf::Vector2f & v)
	{
		v = normaliseCopy(v);
	}
	
	inline float randomNumberF(float min, float max)
	{
		return ((float(rand()) / float(RAND_MAX)) * (max - min)) + min;
	}

	inline int randomNumber(int min, int max)
	{
		return rand() % (max - min + 1) + min;
	}


	inline void limit(sf::Vector2f& v, float max)
	{
		if (Helpers::getLength(v) > max)
		{
			v = Helpers::normaliseCopy(v) * max;
		}
	}
	template<typename T, typename Comparer1, typename Comparer2 = Comparer1>
	inline int binarySearch(const std::vector<T>& v, const T& target, Comparer1 equals, Comparer2 lessThanEquals)
	{
		int low = 0;
		int high = v.size();
		while (low <= high)
		{
			int mid = low + (high - low) / 2;
			if (mid == low)
				return (mid < low) ? mid : low;
			if (equals(v[mid], target))
				return mid;
			else if (lessThanEquals(v[mid], target)) // less than
				low = mid + 1;
			else
				high = mid - 1;
		}
		return -1; //not found
	}
}