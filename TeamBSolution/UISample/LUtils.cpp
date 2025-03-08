#include "LUtils.h"

bool LBox::InPoint(LVector& p)
{
	if (m_Min.x <= p.x && m_Max.x >= p.x
		&&
		m_Min.y <= p.y && m_Max.y >= p.y
		&&
		m_Min.z <= p.z && m_Max.z >= p.z)
	{
		return true;
	}

	return false;
}

bool LBox::RectInPoint(LVector& p)
{
	if (m_Min.x <= p.x && m_Max.x >= p.x
		&&
		m_Min.y <= p.y && m_Max.y >= p.y)
	{
		return true;
	}

	return false;
}

bool LBox::InBox(LBox& box)
{
	if (m_Min.x <= box.m_Min.x && m_Max.x >= box.m_Max.x
		&&
		m_Min.y <= box.m_Min.y && m_Max.y >= box.m_Max.y
		&&
		m_Min.z <= box.m_Min.z && m_Max.z >= box.m_Max.z)
	{
		return true;
	}

	return false;
}

bool LBox::CollisionRect(LBox& rect)
{
	if (*this == rect) return false;

	LVector dir = m_Center - rect.m_Center;

	if (fabs(dir.x) <= (m_Half.x + rect.m_Half.x))
	{
		if (fabs(dir.y) <= (m_Half.y + rect.m_Half.y))
		{
			rect.isCollision = true;
			return true;
		}
	}

	return false;
}

bool LBox::CollisionBox(LBox& box)
{
	if (*this == box) return false;

	LVector dir = m_Center - box.m_Center;

	if (fabs(dir.x) <= (m_Half.x + box.m_Half.x))
	{
		if (fabs(dir.y) <= (m_Half.y + box.m_Half.y))
		{
			if (fabs(dir.z) <= (m_Half.z + box.m_Half.z))
			{
				box.isCollision = true;
				return true;
			}
		}
	}

	return false;
}

void LBox::Set(float fx, float fy, float fz, float fw, float fh, float fd)
{
	m_Center.x = fx;
	m_Center.y = fy;
	m_Center.z = fz;
	m_fWidth = fw;
	m_fHeight = fh;
	m_fDepth = fd;
	m_Half = { m_fWidth * 0.5f, m_fHeight * 0.5f, m_fDepth * 0.5f };

	m_Point[0] = { m_Center.x - m_Half.x, m_Center.y + m_Half.y, m_Center.z - m_Half.z };
	m_Point[1] = { m_Center.x + m_Half.x, m_Center.y + m_Half.y, m_Center.z - m_Half.z };
	m_Point[2] = { m_Center.x + m_Half.x, m_Center.y - m_Half.y, m_Center.z - m_Half.z };
	m_Point[3] = { m_Center.x - m_Half.x, m_Center.y - m_Half.y, m_Center.z - m_Half.z };

	m_Point[4] = { m_Center.x - m_Half.x, m_Center.y + m_Half.y, m_Center.z + m_Half.z };
	m_Point[5] = { m_Center.x + m_Half.x, m_Center.y + m_Half.y, m_Center.z + m_Half.z };
	m_Point[6] = { m_Center.x + m_Half.x, m_Center.y - m_Half.y, m_Center.z + m_Half.z };
	m_Point[7] = { m_Center.x - m_Half.x, m_Center.y - m_Half.y, m_Center.z + m_Half.z };

	m_Min = m_Point[3];
	m_Max = m_Point[5];
}

void LBox::Set(LVector p, float fw, float fh, float fd)
{
	Set(p.x, p.y, p.z, fw, fh, fd);
}

void LBox::Set(float fx, float fy, float fz)
{
	Set(fx, fy, fz, m_fWidth, m_fHeight, m_fDepth);
}

void LBox::Set(LVector p)
{
	Set(p.x, p.y, p.z, m_fWidth, m_fHeight, m_fDepth);
}

bool LBox::operator == (LBox& box)
{
	if (fabs(m_Center.x - box.m_Center.x) < L_EPSILON)
	{
		if (fabs(m_Center.y - box.m_Center.y) < L_EPSILON)
		{
			if (fabs(m_Center.z - box.m_Center.z) < L_EPSILON)
			{
				if (fabs(m_fWidth - box.m_fWidth) < L_EPSILON)
				{
					if (fabs(m_fHeight - box.m_fHeight) < L_EPSILON)
					{
						if (fabs(m_fDepth - box.m_fDepth) < L_EPSILON)
						{
							return true;
						}
					}
				}
				return false;
			}
		}
	}

	return false;
}

bool LBox::operator != (LBox& box)
{
	return (*this == box);
}

LBox::LBox() {}
LBox::LBox(float fx, float fy, float fz, float fw, float fh, float fd)
{
	Set(fx, fy, fz, fw, fh, fd);
}
