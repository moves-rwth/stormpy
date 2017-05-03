/*
 * parser.h
 *
 *  Created on: 14 Apr 2016
 *      Author: hbruintjes
 */

#pragma once

#include "src/common.h"
#include <string>

py::handle parseString(const std::string& input);
