#include "ObjetoPortable.h"

ObjetoPortable::ObjetoPortable(Game* mGame) : GameObject(mGame), isPicked_(false)
{
}

void ObjetoPortable::pickObject()
{
	isPicked_ = true;
	onObjectPicked();
}

void ObjetoPortable::dropObject()
{
	isPicked_ = false;
	onObjectDropped();
}

void ObjetoPortable::setPicked(bool b)
{
	isPicked_ = b;
}

bool ObjetoPortable::isPicked()
{
	return isPicked_;
}
