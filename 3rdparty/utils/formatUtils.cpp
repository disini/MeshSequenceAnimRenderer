#include "formatUtils.h"

using namespace std;

//    const char* formatNumByDigit(int num)
string formatNumByDigit(int num)
{
    char outputStr[20] = "00000000000";
    sprintf(outputStr, "%05d", num);
    cout << "formatNumByDigit() : num == " << num << ", outputStr == " << outputStr << endl;
    return outputStr;
}


hash_t hash_(const char *str)
{
    hash_t ret{basis};

    while (*str) {
        ret ^= *str;
        ret *= prime;
        str++;
    }

    return ret;
}

constexpr hash_t hash_compile_time(const char *str, hash_t last_value)
{
    return *str ? hash_compile_time(str+1, (*str ^ last_value) * prime) : last_value;
}

// https://blog.csdn.net/YoziDream/article/details/22789147?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromBaidu-1.control&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromBaidu-1.control
constexpr unsigned long long operator "" _hash(char const* p, size_t size)
{
    return hash_compile_time(p);
}
