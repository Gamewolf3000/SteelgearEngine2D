#pragma once

namespace SG
{
	struct SGMatrix4x4
	{
		float elements[16];
	};

	struct SGMatrix3x3
	{
		float elements[9];
	};

	struct SGVec3
	{
		float elements[3];
	};

	struct SGVec4
	{
		float elements[4];
	};

	struct SGPoint
	{
		float xPos;
		float yPos;
	};

	struct SGRect
	{
		float leftMost;
		float topMost;
		float width;
		float height;

		bool Overlaps(const SGRect& other)
		{
			if (this->leftMost <= other.leftMost + other.width && this->leftMost + this->width >= other.leftMost)
				if (this->topMost <= other.topMost + other.height && this->topMost + this->height >= other.topMost)
					return true;

			return false;
		}

		bool Overlaps(const SGPoint& point)
		{
			if (this->leftMost <= point.xPos && this->leftMost + this->width >= point.xPos)
				if (this->topMost <= point.yPos && this->topMost + this->height >= point.yPos)
					return true;

			return false;
		}
	};
}