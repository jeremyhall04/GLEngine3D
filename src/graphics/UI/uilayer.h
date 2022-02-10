#ifndef UI_LAYER_H
#define UI_LAYER_H

#include "../renderable2D.h"

class UILayer
{
private:
	Quad2D* crosshair;
public:
	UILayer();
	~UILayer();
};

#endif // !UI_LAYER_H

