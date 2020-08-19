#include "KeyEvent.h"


namespace River {
	KeyEvent::KeyEvent(const Key key, const Action action) :
		key(key),
		action(action)
	{}
}