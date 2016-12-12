/*
 * Config.h
 *
 *  Created on: Jan 19, 2016
 *      Author: qi
 */

#ifndef MODEL_CONFIG_H_
#define MODEL_CONFIG_H_

namespace rec {

class Config {
public:
	int _dim;
	enum POWER_TRANSFORM_FUNC {
		EXP,
		SIGMOID
	};
	POWER_TRANSFORM_FUNC _powerTFunc;
	double _sigBound;
	double _initLearnRate;
	double _eta;
	static Config CONFIG_INST;
public:
	Config();
	virtual ~Config();
};

const Config& getConfig();

} /* namespace model */

#endif /* MODEL_CONFIG_H_ */
