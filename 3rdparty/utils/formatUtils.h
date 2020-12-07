#pragma once

#ifndef _FORMAT_UTILS_H_
#define _FORMAT_UTILS_H_

#include <iostream>
#include <string.h>
#include <stdio.h>

using namespace std;

typedef std::uint64_t hash_t ;

constexpr hash_t prime = 0x100000001B3ull;
constexpr hash_t basis = 0xCBF29CE484222325ull;



string formatNumByDigit(int num);

hash_t hash_(char const* str);

constexpr hash_t hash_compile_time(char const* str, hash_t last_value = basis);

constexpr unsigned long long operator "" _hash(char const* p, size_t);



#endif
