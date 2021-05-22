#include "room_type.h"

const char* GetRoomTypeDescriptionById(RoomType roomType)
{
	switch (roomType)
	{
	case NORMAL_OFF:
		return "Normal (off)";
	case GENERIC:
		return "Generic";
	case METAL_SMALL:
		return "Metal Small";
	case METAL_MEDIUM:
		return "Metal Medium";
	case METAL_LARGE:
		return "Metal Large";
	case TUNNEL_SMALL:
		return "Tunnel Small";
	case TUNNEL_MEDIUM:
		return "Tunnel Medium";
	case TUNNEL_LARGE:
		return "Tunnel Large";
	case CHAMBER_SMALL:
		return "Chamber Small";
	case CHAMBER_MEDIUM:
		return "Chamber Medium";
	case CHAMBER_LARGE:
		return "Chamber Large";
	case BRIGHT_SMALL:
		return "Bright Small";
	case BRIGHT_MEDIUM:
		return "Bright Medium";
	case BRIGHT_LARGE:
		return "Bright Large";
	case WATER_1:
		return "Water 1";
	case WATER_2:
		return "Water 2";
	case WATER_3:
		return "Water 3";
	case CONCRETE_SMALL:
		return "Concrete Small";
	case CONCRETE_MEDIUM:
		return "Concrete Medium";
	case CONCRETE_LARGE:
		return "Concrete Large";
	case BIG_1:
		return "Big 1";
	case BIG_2:
		return "Big 2";
	case BIG_3:
		return "Big 3";
	case CAVERN_SMALL:
		return "Cavern Small";
	case CAVERN_MEDIUM:
		return "Cavern Medium";
	case CAVERN_LARGE:
		return "Cavern Large";
	case PIPE_MEDIUM:
		return "Pipe Medium";
	case PIPE_SMALL:
		return "Pipe Small";
	case PIPE_BIG:
		return "Pipe Big?";
	default:
		return "????"; // Unknown
	}
}
