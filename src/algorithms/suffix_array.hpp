#ifndef _HAVE_SUFFIXARR_H

#define _HAVE_SUFFIXARR_H
#include <bits/stdc++.h>
#include <iostream>

void skew_sa(int* text, int* SA, int text_size, int alpha_size);
void sort_sa(int* text, int* SA, int text_size, int alpha_size);
int sa_search(char *text, int text_size,char *patt, int patt_size, int* SA, int* &occ, bool only_count);

#endif