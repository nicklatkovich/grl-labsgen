#pragma once

struct UIntPoint {
	unsigned int x;
	unsigned int y;
	UIntPoint()
		: UIntPoint(0, 0)
	{
	}
	UIntPoint(unsigned int x, unsigned int y)
		: x(x)
		, y(y)
	{
	}
};
