#pragma once
#include <list>
#include "Visuals.MessageDetail.h"
namespace visuals
{
	struct Message
	{
		std::string caption;
		std::list<MessageDetail> details;
	};
}
