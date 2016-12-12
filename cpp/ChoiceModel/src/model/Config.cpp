/*
 * Config.cpp
 *
 *  Created on: Jan 19, 2016
 *      Author: qi
 */

#include "Config.h"

namespace rec {

Config::Config():_dim(10),_powerTFunc(SIGMOID),_sigBound(50),_initLearnRate(1e-3),_eta(0.01) {
	// TODO Auto-generated constructor stub

}

Config::~Config() {
	// TODO Auto-generated destructor stub
}

/// global instance
Config Config::CONFIG_INST;

const Config& getConfig() {
	static Config _CONFIG_INST;
	return _CONFIG_INST;
}

} /* namespace model */
