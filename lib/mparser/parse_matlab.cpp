/* parse_matlab.cpp
 *
 * Copyright(C) 2013 Yauhen Yakimovich
 *
 * This file is part of the mparser package, which is licensed under
 * the MIT license.  See the file COPYING for details.
 */

#include <iostream>
#include "mparser.hpp"
using namespace std;

extern "C" {
    const char *parse_matlab(const char* mfilename)
    {
        cout << "parse_matlab: \n";
        const char *res = "hello world\n";
        cout << res;
        return res;
    }
}
