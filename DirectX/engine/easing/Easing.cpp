#include "Easing.h"
#include <cmath>

//“™‘¬’¼ü‰^“®
float Easing::Lerp(float _start, float _end, float _time)
{
	return _start * (1.0f - _time) + _end * _time;
}

//sin In
float Easing::InSine(float _start, float _end, float _time)
{
	float y = (1.0f - cosf((_time * 3.14159265359f) / 2.0f));
	return _start * (1.0f - y) + _end * y;
}

//sin Out
float Easing::OutSine(float _start, float _end, float _time)
{
	float y = sinf((_time * 3.14159265359f) / 2.0f);
	return _start * (1.0f - y) + _end * y;
}

//sin InOut
float Easing::InOutSine(float _start, float _end, float _time)
{
	float y = (-(cosf(3.14159265359f * _time) - 1.0f) / 2.0f);
	return _start * (1.0f - y) + _end * y;
}

//2æ In
float Easing::InQuad(float _start, float _end, float _time)
{
	float y = _time * _time;
	return _start * (1.0f - y) + _end * y;
}

//2æ Out
float Easing::OutQuad(float _start, float _end, float _time)
{
	float y = (1.0f - (1.0f - _time) * (1.0f - _time));
	return _start * (1.0f - y) + _end * y;
}

//2æ InOut
float Easing::InOutQuad(float _start, float _end, float _time)
{
	float y;
	if (_time < 0.5)
	{
		y = 2.0f * _time * _time;
	} else
	{
		y = (1.0f - powf(-2.0f * _time + 2.0f, 2) / 2.0f);
	}

	return _start * (1.0f - y) + _end * y;
}

//3æ In
float Easing::InCubic(float _start, float _end, float _time)
{
	float y = _time * _time * _time;
	return _start * (1.0f - y) + _end * y;
}

//3æ Out
float Easing::OutCubic(float _start, float _end, float _time)
{
	float y = (1.0f - powf(1.0f - _time, 3));
	return _start * (1.0f - y) + _end * y;
}

//3æ InOut
float Easing::InOutCubic(float _start, float _end, float _time)
{
	float y;
	if (_time < 0.5f)
	{
		y = 4.0f * _time * _time * _time;
	} else
	{
		y = (1.0f - powf(-2.0f * _time + 2.0f, 3) / 2.0f);
	}

	return _start * (1.0f - y) + _end * y;
}

//4æ In
float Easing::InQuart(float _start, float _end, float _time)
{
	float y = _time * _time * _time * _time;
	return _start * (1.0f - y) + _end * y;
}

//4æ Out
float Easing::OutQuart(float _start, float _end, float _time)
{
	float y = (1.0f - powf(1.0f - _time, 4));
	return _start * (1.0f - y) + _end * y;
}

//4æ InOut
float Easing::InOutQuart(float _start, float _end, float _time)
{
	float y;
	if (_time < 0.5f)
	{
		y = 8.0f * _time * _time * _time * _time;
	} else
	{
		y = (1.0f - powf(-2.0f * _time + 2.0f, 4) / 2.0f);
	}

	return _start * (1.0f - y) + _end * y;
}

//5æ In
float Easing::InQuint(float _start, float _end, float _time)
{
	float y = _time * _time * _time * _time * _time;
	return _start * (1.0f - y) + _end * y;
}

//5æ Out
float Easing::OutQuint(float _start, float _end, float _time)
{
	float y = (1.0f - powf(1.0f - _time, 5));
	return _start * (1.0f - y) + _end * y;
}

//5æ InOut
float Easing::InOutQuint(float _start, float _end, float _time)
{
	float y;
	if (_time < 0.5f)
	{
		y = 16.0f * _time * _time * _time * _time * _time;
	} else
	{
		y = (1.0f - powf(-2.0f * _time + 2.0f, 5) / 2.0f);
	}

	return _start * (1.0f - y) + _end * y;
}

//Expo In
float Easing::InExpo(float _start, float _end, float _time)
{
	float y;
	if (_time == 0)
	{
		y = 0.0f;
	} else
	{
		y = powf(2.0f, 10.0f * _time - 10);
	}

	return _start * (1.0f - y) + _end * y;
}

//Expo Out
float Easing::OutExpo(float _start, float _end, float _time)
{
	float y;
	if (_time == 1.0f)
	{
		y = 1.0f;
	} else
	{
		y = (1.0f - powf(2.0f, -10 * _time));
	}

	return _start * (1.0f - y) + _end * y;
}

//Expo InOut
float Easing::InOutExpo(float _start, float _end, float _time)
{
	float y;
	if (_time == 0.0f)
	{
		y = 0;
	} else if (_time == 1.0f)
	{
		y = 1;
	} else if (_time < 0.5f)
	{
		y = powf(2.0f, 20 * _time - 10) / 2.0f;
	} else
	{
		y = (2.0f - powf(2.0f, -20 * _time + 10)) / 2.0f;
	}

	return _start * (1.0f - y) + _end * y;
}

//Circ In
float Easing::InCirc(float _start, float _end, float _time)
{
	float y = 1.0f - sqrtf(1.0f - powf(_time, 2));
	return _start * (1.0f - y) + _end * y;
}

//Circ Out
float Easing::OutCirc(float _start, float _end, float _time)
{
	float y = sqrtf(1.0f - powf(_time - 1.0f, 2));
	return _start * (1.0f - y) + _end * y;
}

//Circ InOut
float Easing::InOutCirc(float _start, float _end, float _time)
{
	float y;
	if (_time < 0.5f)
	{
		y = (1.0f - sqrtf(1.0f - powf(2.0f * _time, 2))) / 2.0f;
	} else
	{
		y = (sqrtf(1.0f - powf(-2.0f * _time + 2.0f, 2)) + 1) / 2.0f;
	}

	return _start * (1.0f - y) + _end * y;
}

//Back In
float Easing::InBack(float _start, float _end, float _time)
{
	float y = 2.70158f * _time * _time * _time - 1.70158f * _time * _time;
	return _start * (1.0f - y) + _end * y;
}

//Back Out
float Easing::OutBack(float _start, float _end, float _time)
{
	float y = 1.0f + 2.70158f * powf(_time - 1.0f, 3) + 1.70158f * powf(_time - 1.0f, 2);
	return _start * (1.0f - y) + _end * y;
}

//Back InOut
float Easing::InOutBack(float _start, float _end, float _time)
{
	float y;
	if (_time < 0.5f)
	{
		y = (powf(2.0f * _time, 2) * ((1.70158f * 1.525f + 1) * 2.0f * _time - 1.70158f * 1.525f)) / 2.0f;
	} else
	{
		y = (powf(2.0f * _time - 2.0f, 2) * ((1.70158f * 1.525f + 1.0f) * (_time * 2.0f - 2.0f) + 1.70158f * 1.525f) + 2.0f) / 2.0f;
	}

	return _start * (1.0f - y) + _end * y;
}

//Elastic In
float Easing::InElastic(float _start, float _end, float _time)
{
	float y;
	if (_time == 0.0f)
	{
		y = 0.0f;
	} else if (_time == 1.0f)
	{
		y = 1.0f;
	} else
	{
		y = -powf(2.0f, 10 * _time - 10) * sinf((_time * 10.0f - 10.75f) * (2.0f * 3.14159265359f) / 3.0f);
	}

	return _start * (1.0f - y) + _end * y;
}

//Elastic Out
float Easing::OutElastic(float _start, float _end, float _time)
{
	float y;
	if (_time == 0.0f)
	{
		y = 0.0f;
	} else if (_time == 1.0f)
	{
		y = 1.0f;
	} else
	{
		y = powf(2.0f, -10 * _time) * sinf((_time * 10.0f - 0.75f) * (2.0f * 3.14159265359f) / 3.0f) + 1.0f;
	}

	return _start * (1.0f - y) + _end * y;
}

//Elastic InOut
float Easing::InOutElastic(float _start, float _end, float _time)
{
	float y;
	if (_time == 0.0f)
	{
		y = 0.0f;
	} else if (_time == 1.0f)
	{
		y = 1.0f;
	} else if (_time < 0.5f)
	{
		y = -(powf(2.0f, 20 * _time - 10) * sinf((20.0f * _time - 11.125f) * (2.0f * 3.14159265359f) / 4.5f)) / 2.0f;
	} else
	{
		y = (powf(2.0f, -20 * _time + 10) * sinf((20.0f * _time - 11.125f) * (2.0f * 3.14159265359f) / 4.5f)) / 2.0f + 1.0f;
	}

	return _start * (1.0f - y) + _end * y;
}

//Bounce In
float Easing::InBounce(float _start, float _end, float _time)
{
	float time = 1.0f - _time;
	float y = 1.0f - OutBounceCalculate(time);
	return _start * (1.0f - y) + _end * y;
}

//Bounce Out
float Easing::OutBounce(float _start, float _end, float _time)
{
	float time = _time;
	float y = OutBounceCalculate(time);
	return _start * (1.0f - y) + _end * y;
}

//Bounce InOut
float Easing::InOutBounce(float _start, float _end, float _time)
{
	float y;
	float time;
	if (_time < 0.5f)
	{
		time = 1.0f - 2.0f * _time;
		y = (1.0f - OutBounceCalculate(time)) / 2.0f;
	} else
	{
		time = 2.0f * _time - 1.0f;
		y = (1.0f + OutBounceCalculate(time)) / 2.0f;
	}

	return _start * (1.0f - y) + _end * y;
}

//BounceŒvZ—p
float Easing::OutBounceCalculate(float& _time)
{
	float y;
	if (_time < 1.0f / 2.75f)
	{
		y = 7.5625f * _time * _time;
	} else if (_time < 2.0f / 2.75)
	{
		y = 7.5625f * (_time -= 1.5f / 2.75f) * _time + 0.75f;
	} else if (_time < 2.5 / 2.75)
	{
		y = 7.5625f * (_time -= 2.25f / 2.75f) * _time + 0.9375f;
	} else
	{
		y = 7.5625f * (_time -= 2.625f / 2.75f) * _time + 0.984375f;
	}

	return y;
}