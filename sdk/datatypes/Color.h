struct Color255 {
	unsigned char red, green, blue;

	constexpr Color255() : red(0), green(0), blue(0) { }

	constexpr Color255(unsigned char r, unsigned char g, unsigned char b)
		: red{ r }, green{ g }, blue{ b } { }
};

struct Color {
	unsigned char red, green, blue, alpha;

	constexpr Color() : red(0), green(0), blue(0), alpha(255) { }

	constexpr Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255)
		: red{ r }, green{ g }, blue{ b }, alpha{ a } { }

	bool operator == (const Color &color) const {
		return (*((int *)this) == *((int *)&color));
	}

	bool operator != (const Color &color) const {
		return !(operator==(color));
	}

	constexpr Color& operator *=(const float coeff) {
		this->red = static_cast<int>(this->red * coeff);
		this->green = static_cast<int>(this->green * coeff);
		this->blue = static_cast<int>(this->blue * coeff);
		return *this;
	}

	constexpr Color operator ()(const int a) const {
		return Color(red, green, blue, a);
	}

	constexpr Color& operator /=(const float div) {
		const auto div_ = 1.f / div;
		*this *= div_;
		return *this;
	}

	constexpr Color& operator *(const float coeff) const {
		auto color = *this;
		return color *= coeff;
	}

	unsigned long HEXtoRGBA(int r, int g, int b, int a) {
		return ((r & 0xff) << 24) + ((g & 0xff) << 16) + ((b & 0xff) << 8)
			+ (a & 0xff);
	}

	Color(Color color, int alpha)
	{
		SetColor((int)color.red, (int)color.green, (int)color.blue, alpha);
	}

	void SetColor(int r, int g, int b, int a = 255)
	{
		red = (unsigned char)r;
		green = (unsigned char)g;
		blue = (unsigned char)b;
		alpha = (unsigned char)a;
	}

	void SetRawColor(int color32) {
		*((int*)this) = color32;
	}

	int GetRawColor() const {
		return *((int*)this);
	}

	inline int r() const { return red; }
	inline int g() const { return green; }
	inline int b() const { return blue; }
	inline int a() const { return alpha; }

	constexpr Color& FromHSV(float h, float s, float v) {
		float out[3]{ };
		if (s == 0.0f) {
			red = green = blue = static_cast<int>(v * 255);
			return *this;
		}

		h = fmodf(h, 1.0f) / (60.0f / 360.0f);
		int   i = static_cast<int>(h);
		float f = h - static_cast<float>(i);
		float p = v * (1.0f - s);
		float q = v * (1.0f - s * f);
		float t = v * (1.0f - s * (1.0f - f));

		switch (i) {
		case 0:
			out[0] = v;
			out[1] = t;
			out[2] = p;
			break;
		case 1:
			out[0] = q;
			out[1] = v;
			out[2] = p;
			break;
		case 2:
			out[0] = p;
			out[1] = v;
			out[2] = t;
			break;
		case 3:
			out[0] = p;
			out[1] = q;
			out[2] = v;
			break;
		case 4:
			out[0] = t;
			out[1] = p;
			out[2] = v;
			break;
		case 5:
		default:
			out[0] = v;
			out[1] = p;
			out[2] = q;
			break;
		}

		red = static_cast<int>(out[0] * 255);
		green = static_cast<int>(out[1] * 255);
		blue = static_cast<int>(out[2] * 255);
		return *this;
	}

	static Color FromHSB(float hue, float saturation, float brightness) {
		float h = hue == 1.0f ? 0 : hue * 6.0f;
		float f = h - (int)h;
		float p = brightness * (1.0f - saturation);
		float q = brightness * (1.0f - saturation * f);
		float t = brightness * (1.0f - (saturation * (1.0f - f)));

		if (h < 1) {
			return Color(
				(unsigned char)(brightness * 255),
				(unsigned char)(t * 255),
				(unsigned char)(p * 255)
			);
		}
		else if (h < 2) {
			return Color(
				(unsigned char)(q * 255),
				(unsigned char)(brightness * 255),
				(unsigned char)(p * 255)
			);
		}
		else if (h < 3) {
			return Color(
				(unsigned char)(p * 255),
				(unsigned char)(brightness * 255),
				(unsigned char)(t * 255)
			);
		}
		else if (h < 4) {
			return Color(
				(unsigned char)(p * 255),
				(unsigned char)(q * 255),
				(unsigned char)(brightness * 255)
			);
		}
		else if (h < 5) {
			return Color(
				(unsigned char)(t * 255),
				(unsigned char)(p * 255),
				(unsigned char)(brightness * 255)
			);
		}
		else {
			return Color(
				(unsigned char)(brightness * 255),
				(unsigned char)(p * 255),
				(unsigned char)(q * 255)
			);
		}
	}

	static constexpr Color Red() { return Color(255, 0, 0); }
	static constexpr Color Green() { return Color(0, 255, 0); }
	static constexpr Color Blue() { return Color(0, 0, 255); }
	static constexpr Color LightBlue() { return Color(100, 100, 255); }
	static constexpr Color Grey() { return Color(128, 128, 128); }
	static constexpr Color DarkGrey() { return Color(45, 45, 45); }
	static constexpr Color Black() { return Color(0, 0, 0); }
	static constexpr Color White() { return Color(255, 255, 255); }
	static constexpr Color Purple() { return Color(220, 0, 220); }
	static constexpr Color Pink() { return Color(255, 105, 180); }
};