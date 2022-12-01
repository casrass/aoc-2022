#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "curl/curl.h"

#pragma comment(lib, "libcurl.lib")

static const char* cookie = "session=deadbeef;";

auto main() -> __int32
{
    static std::string data;

    static const auto callback = []
    (
        char* contents, 
        unsigned __int64 size, 
        unsigned __int64 nmemb, 
        void* userp
    ) -> unsigned __int64
    {
        *(std::string*)userp += std::string(contents, size * nmemb);

        return size * nmemb;
    };

    for (auto curl = curl_easy_init(); curl_easy_perform(curl) != 0; Sleep(1000))
    {   
        curl_easy_setopt(curl, CURLOPT_URL, "https://adventofcode.com/2022/day/1/input");

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, 
            (unsigned __int64(__stdcall*)(char*, unsigned __int64, unsigned __int64, void*))callback);

        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);

        curl_easy_setopt(curl, CURLOPT_COOKIE, cookie);
    }

    static const auto split = [](
        std::string data,
        const char* delim
    ) -> std::vector<std::string>
    {
        std::vector<std::string> list;

        unsigned __int64 x = 0, y = data.find(delim);

        for (; y != std::string::npos; x = y + strlen(delim), y = data.find(delim, x))
            list.emplace_back(data.substr(x, y - x));

        list.emplace_back(data.substr(x, y));

        return list;
    };

    std::vector<std::pair<unsigned __int64, unsigned __int64>> sum_list;

    auto elf_list = split(data, "\n\n");

    for (unsigned __int64 x = 0; x < elf_list.size(); ++x)
    {
        auto item_list = split(elf_list[x], "\n");

        unsigned __int64 sum = 0;

        for (unsigned __int64 y = 0; y < item_list.size(); ++y)
        {
            if (item_list[y].find_first_of("0123456789") == std::string::npos)
                continue;

            sum += std::stoi(item_list[y]);
        }

        sum_list.emplace_back(std::pair<unsigned __int64, unsigned __int64>{ x, sum });
    }

    std::sort(sum_list.begin(), sum_list.end(), [&](
        std::pair<unsigned __int64, unsigned __int64>& item1, 
        std::pair<unsigned __int64, unsigned __int64>& item2
    ) -> bool
    {
        return item1.second > item2.second;
    });

    std::cout << "elf with the largest calorie count is - #" << 
        sum_list[0].first << " with " << sum_list[0].second << " calories" << std::endl;

    unsigned __int64 top_sum = 0;

    for (unsigned __int64 x = 0; x < 3; ++x)
        top_sum += sum_list[x].second;

    std::cout << "sum of the top 3 elves' calories is " << top_sum << std::endl;

    return std::cin.get() != 0xffffff;
}