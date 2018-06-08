/*
class Color
{
public:
	unsigned int Key;
	constexpr Color() 
	{
		Key();
	}
	constexpr Color(const Color& color) 
	{
		Key(color.Key);
	}
	constexpr Color(unsigned int dw) 
	{
		Key(dw)
	}
	constexpr Color(unsigned char a, unsigned char r, unsigned char g, unsigned char b) 
	{
		Key((a << 24u) | (r << 16u) | (g << 8u) | b);
	}
	constexpr Color(unsigned char r, unsigned char g, unsigned char b)
	{
		Key((r << 16u) | (g << 8u) | b);
	}
	constexpr Color(Color col, unsigned char x)
	{
		Color((x << 24u) | col.Key);
	}
	constexpr unsigned char GetAlpha() const
	{
		return Key >> 24u;
	}
	constexpr unsigned char GetRed() const
	{
		return (Key >> 16u) & 0xFFu;
	}
	constexpr unsigned char GetGreen() const
	{
		return (Key >> 8u) & 0xFFu;
	}
	constexpr unsigned char GetBlue() const
	{
		return Key & 0xFFu;
	}
	void SetAlpha(unsigned char a)
	{
		Key = (Key & 0xFFFFFFu) | (a << 24u);
	}
	void SetRed(unsigned char r)
	{
		Key = (Key & 0xFF00FFFFu) | (r << 16u);
	}
	void SetGreen(unsigned char g)
	{
		Key = (Key & 0xFFFF00FFu) | (g << 8u);
	}
	void SetBlue(unsigned char b)
	{
		Key = (Key & 0xFFFFFF00u) | b;
	}
	Color& operator = (Color color)
	{
		Key = color.Key;
		return *this;
	}
	static constexpr Color MakeColor(unsigned char r, unsigned char g, unsigned char b)
	{
		return (r << 16) | (g << 8) | b;
	}
	static constexpr Color White = MakeColor(255u, 255u, 255u);
	static constexpr Color Black = MakeColor(0u, 0u, 0u);
	static constexpr Color Gray = MakeColor(0x80u, 0x80u, 0x80u);
	static constexpr Color LightGray = MakeColor(0xD3u, 0xD3u, 0xD3u);
	static constexpr Color Red = MakeColor(255u, 0u, 0u);
	static constexpr Color Green = MakeColor(0u, 255u, 0u);
	static constexpr Color Blue = MakeColor(0u, 0u, 255u);
	static constexpr Color Yellow = MakeColor(255u, 255u, 0u);
	static constexpr Color Cyan = MakeColor(0u, 255u, 255u);
	static constexpr Color Magenta = MakeColor(255u, 0u, 255u);
};
*/

struct color
{
	int red;
	int green;
	int blue;
	int alpha;
};

class Canvas
{
public:
	Canvas(int, int);
	~Canvas();
	int pitch, width, BPP = 4, pitch = pitch * BPP;
	color* screen = new color[pitch*width];

private:
	void Put_pixel(int, int, color);
	void Line(int, int, int, int, color);
	void rectangle(int, int, int, int, color);
	void circle(int, int, int, color);
	void ellipse(int, int, int, color);
	void BitBlit(Canvas*, int x, int y);
	void BitBlit(Canvas*, int x, int y, int yinix, int iniy, int endx, int endy);
	color getpixel(int, int);
};

Canvas::Canvas(int W, int H)
{
	pitch = W;
	width = H;
}

void Canvas::Put_pixel(int x, int y, color color)
{
	screen[(x * 4) + (y*pitch)] = color;
}

color Canvas::getpixel(int X, int Y)
{
	return screen[(X)+(Y*pitch)];
}


void Canvas::Line(int x1, int y1, int x2, int y2, color color)
{
	int pendiente;
	int dx, dy, incE, incNE, d, x, y;
	if (x1 > x2)
	{
		Line(x2, y2, x1, y1, color);
		return;
	}
	dx = x2 - x1;
	dy = y2 - y1;

	if (dy < 0)
	{
		pendiente = -1;
		dy = -dy;
	}
	else
	{
		pendiente = 1;
	}

	incE = 2 * dy;
	incNE = 2 * dy - 2 * dx;
	d = 2 * dy - dx;
	y = y1;
	for (x = x1; x <= x2; x++)
	{
		Put_pixel(x, y, color);
		if (d <= 0)
		{
			d += incE;
		}
		else
		{
			d += incNE;
			y += pendiente;
		}
	}
}

void Canvas::rectangle(int x, int y, int pitch, int width, color color)
{
	Line(x, y, x + pitch, y, color);
	Line(x + pitch, y, x + pitch, y + width, color);
	Line(x + pitch, y + width, x, y + width, color);
	Line(x, y + width, x, y, color);
}


void Canvas::circle(int X, int Y, int radio, color color)
{
	int f = 1 - radio;
	int ddF_x = 1;
	int ddF_y = -2 * radio;
	int x = 0;
	int y = radio;
	Put_pixel(X, Y + radio, color);
	Put_pixel(X, Y - radio, color);
	Put_pixel(X + radio, Y, color);
	Put_pixel(X - radio, Y, color);
	while (x < y)
	{
		if (f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;
		Put_pixel(X + x, Y + y, color);
		Put_pixel(X - x, Y + y, color);
		Put_pixel(X + x, Y - y, color);
		Put_pixel(X - x, Y - y, color);
		Put_pixel(X + y, Y + x, color);
		Put_pixel(X - y, Y + x, color);
		Put_pixel(X + y, Y - x, color);
		Put_pixel(X - y, Y - x, color);
	}
}

void Canvas::ellipse(int x0, int y0, int radio, color color)
{
	int
		x = -radio, y = 0, err = 2 - 2 * radio;
	do
	{
		Put_pixel(x0 - x, y0 + y, color);
		Put_pixel(x0 - y, y0 - x, color);
		Put_pixel(x0 + x, y0 - y, color);
		Put_pixel(x0 + y, y0 + x, color);
		radio = err;
		if (radio > x)
		{
			err += x++ * 2 + 1;
		}
		if (radio <= y)
		{
			err += y++ * 2 + 1;
		}
	} while (x < 0);
}

void Canvas::BitBlit(Canvas* image, int x, int y)
{
	for (size_t j = 0; j <= image->width; j++)
	{
		for (size_t i = 0; i <= image->pitch; i++)
		{
			if (x + i > 0 || this->pitch > x + i)
			{
				if (y + j > 0 || this->width > y + i)
				{
					this->screen[(x + i) + ((y + j)*pitch)] = image->screen[(x)+(y*pitch)];
				}
			}
		}
	}

}

void Canvas::BitBlit(Canvas* image, int x, int y, int inix, int iniy, int endx, int endy)
{
	int tam_x = endx - inix, tam_y = endy - iniy;
	for (size_t j = 0; j <= tam_y; j++)
	{
		for (size_t i = 0; i <= tam_x; i++)
		{
			if (x + i > 0 || this->pitch > x + i)
			{
				if (y + j > 0 || this->width > y + i)
				{
					this->screen[(x + i) + ((y + j)*pitch)] = image->screen[(x + inix) + ((y + iniy)*pitch)];
				}
			}
		}
	}



}


Canvas::~Canvas()
{
}