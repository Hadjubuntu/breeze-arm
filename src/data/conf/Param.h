/*
 * Param.h
 *
 *  Created on: Nov 11, 2015
 *      Author: adrien-i3
 */

#ifndef DATA_CONF_PARAM_H_
#define DATA_CONF_PARAM_H_

#include <string>

template <class T>
class Param {
private:
	std::string _name;
	T _value;
public:
	Param()
{

}
	void setParam(std::string pName, T pValue);

	virtual ~Param() {

	}
};

#endif /* DATA_CONF_PARAM_H_ */
