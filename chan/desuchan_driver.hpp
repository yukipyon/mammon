#ifndef DESUCHAN_DRIVER_HPP
#define DESUCHAN_DRIVER_HPP

#include "chan_driver.hpp"

struct desuchan_driver : public chan_driver {

	desuchan_driver(std::vector<std::string>&&);
	std::string gen_thread_url(const chan_post&) const;
	~desuchan_driver();
};
#endif
