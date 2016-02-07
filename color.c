#include "7w.h"

int gettypecolor(int type) {
	switch (type) {
	case RESOURCE:
		return COLORRESOURCE;
	case INDUSTRY:
		return COLORINDUSTRY;
	case STRUCTURE:
		return COLORSTRUCTURE;
	case COMMERCIAL:
		return COLORCOMMERCIAL;
	case MILITARY:
		return COLORMILITARY;
	case SCIENTIFIC:
		return COLORSCIENTIFIC;
	case GUILD:
		return COLORGUILD;
	default:
		return 0;
	}
}
