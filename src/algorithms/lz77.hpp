#ifndef _HAVE_LZ77_H

#define _HAVE_LZ77_H

#include <bits/stdc++.h>

std::vector<char> encode(char *txt, int sz);
std::vector<char> decode(std::vector<char> &encoded, int sz);

#endif