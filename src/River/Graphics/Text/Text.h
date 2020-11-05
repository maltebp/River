#pragma once

#include "River/Vendor/RiverECS/ECS.h"
#include "Font.h"
#include "River/Primitives/Alignment.h"

namespace River::ECS {

	struct Text : public Component {
		Font* font = nullptr;
		unsigned int size = 12;
		std::string text;
		Color color = Colors::WHITE;
		Alignment alignment = Alignments::CENTER;
	};

}
