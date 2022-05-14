#ifndef _HAVE_LZ77_H

#define _HAVE_LZ77_H

#include <bits/stdc++.h>

std::vector<char> encode(char *txt, unsigned long sz);
std::vector<char> decode(char *encoded, int sz);

#endif