#pragma once

namespace River {

	struct Alignment {
		enum class H { LEFT, CENTER, RIGHT };
		enum class V { TOP, CENTER, BOTTOM };

		const H h;
		const V v;

		Alignment(V vAlign, H hAlign) : h(hAlign), v(vAlign) { }

		bool isTop() { return v == V::TOP; }
		bool isVCenter() { return v == V::CENTER; }
		bool isBottom() { return v == V::BOTTOM; }

		bool isLeft() { return h == H::LEFT; }
		bool isHCenter() { return h == H::CENTER; }
		bool isRight() { return h == H::RIGHT; }

		bool isCenter() { return v == V::CENTER && h == H::CENTER; }
	};


	namespace Align {

		const Alignment TOP_LEFT		(Alignment::V::TOP,		Alignment::H::LEFT);
		const Alignment TOP_CENTER		(Alignment::V::TOP,		Alignment::H::CENTER);
		const Alignment TOP_RIGHT		(Alignment::V::TOP,		Alignment::H::RIGHT);
		const Alignment BOTTOM_LEFT		(Alignment::V::BOTTOM,	Alignment::H::LEFT);
		const Alignment BOTTOM_CENTER	(Alignment::V::BOTTOM,	Alignment::H::CENTER);
		const Alignment BOTTOM_RIGHT	(Alignment::V::BOTTOM,	Alignment::H::RIGHT);
		const Alignment CENTER_LEFT		(Alignment::V::CENTER,	Alignment::H::LEFT);
		const Alignment CENTER_RIGHT	(Alignment::V::CENTER,	Alignment::H::RIGHT);
		const Alignment CENTER			(Alignment::V::CENTER,	Alignment::H::CENTER); 
	}
}


