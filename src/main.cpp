#include <algorithm>
#include <bits/stdc++.h>
#include <chrono>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <unordered_map>
#include <vector>

#include "csv.h"

static constexpr long ID_ROW_SIZE = 5e6;
using UMapVec = std::unordered_map<uint, std::vector<ushort>>;

int main(int argc, char **argv) {
  auto startTimepoint = std::chrono::high_resolution_clock::now();
  std::cout << "reading the ids.csv file..." << std::endl;
  io::CSVReader<1> in("example/ids.csv");
  in.read_header(io::ignore_extra_column, "id");

  std::vector<uint> ids;
  ids.reserve(ID_ROW_SIZE);

  UMapVec umapvec;
  umapvec.reserve(ID_ROW_SIZE);
  {
    unsigned long long counter = 0;
        std::cout << "loading the ids.csv data to the specific container... " << std::endl;
    unsigned int id = 0;
    while (in.read_row(id)) {
      umapvec[id].reserve(200);
      ids.push_back(id);
      ++counter;
    }
        std::cout << "[" << counter << "] the ids container loaded (size : " << umapvec.size() << " elements)" << std::endl;
  }

  std::cout << "sort ids... " << std::endl;
  std::thread worker([&ids]() { std::sort(ids.begin(), ids.end()); });

  std::cout << "generate file path..." << std::endl;
  std::vector<std::string> paths;
  paths.reserve(10);
  paths.emplace_back("example/input1.csv");
  paths.emplace_back("example/input2.csv");
  paths.emplace_back("example/input3.csv");
  paths.emplace_back("example/input4.csv");
  paths.emplace_back("example/input5.csv");
  paths.emplace_back("example/input6.csv");
  paths.emplace_back("example/input7.csv");
  paths.emplace_back("example/input8.csv");
  paths.emplace_back("example/input9.csv");
  paths.emplace_back("example/input10.csv");
  std::cout << paths.size() << " file path generated." << std::endl;

  std::cout << "start to read all inputs files...\n\n" << std::endl;

  for (const auto &path : paths) {
    std::cout << "open file (" << path << ")" << std::endl;
    int fd = open(path.c_str(), O_RDONLY);
    off_t len = lseek(fd, 0, SEEK_END);
    char *buff = (char *)mmap(NULL, len, PROT_READ, MAP_PRIVATE, fd, 0);

    UMapVec::iterator it = umapvec.end();

    unsigned int number = 0;
    char *p = buff;
    off_t bytes = len;

    // drop header (id, duration)
    while (bytes > 0 && *p != '\n') {
      ++p;
      --bytes;
    }

    std::cout << "reading data..." << std::endl;

    while (bytes > 1) {
      if (*p == ',') {
        // duration block :
        ++p;
        --bytes;

        if (it == umapvec.end()) {
          // don't care about duration from this non id iteration
          while (*p != '\n') {
            ++p;
            --bytes;
          }

          // continue to check byte > 0
          continue;
        }

        while (*p != '\n') {
          if ('0' <= *p && *p <= '9') {
            number *= 10;
            number += *p - '0';
          } else
            goto OUTER;
          ++p;
          --bytes;
        }
        // duration = num
        it->second.push_back(number);
        number = 0;
      } else if (*p == '\n') {
        // id block :
        ++p;
        --bytes;

        // read 8 byte id data ( between 10e6~20e6 ) :
        for (int i = 0; i < 8; ++i) {
          if ('0' <= *p && *p <= '9') {
            number *= 10;
            number += *p - '0';
          } else
            goto OUTER;
          ++p;
          --bytes;
        }

        // find id iteration if exist :
        it = umapvec.find(number);
        number = 0;
      } else {
      OUTER:
        std::cerr << "can't handle." << std::endl;
        exit(1);
      }
    }
    if (*p != '\n') {
      std::cerr << "[eof] can't handle." << std::endl;
      exit(1);
    }

    std::cout << "close file (" << path << ")" << std::endl;
    close(fd);
    munmap(buff, len);
    std::cout << "---------------------------" << std::endl;
  }

  if (umapvec.empty()) {
    auto endTimerPoint = std::chrono::high_resolution_clock::now();
        auto start = std::chrono::time_point_cast<std::chrono::seconds>(startTimepoint).time_since_epoch().count();
        auto end = std::chrono::time_point_cast<std::chrono::seconds>(endTimerPoint).time_since_epoch().count();
    auto duration = end - start;
    double minutes = duration / 60;
    double seconds = duration % 60;
        std::cout << "\n\nempty. (duration : " << minutes << " minutes and " << seconds << " seconds)" << std::endl;
  }

  // wait for sort thread done.
  worker.join();
  std::cout << "\n\nids sorted." << std::endl;

  std::cout << "create the out.csv file..." << std::endl;
  std::ofstream file("out.csv");
  if (!file.is_open()) {
    std::cerr << "the file out.csv is not open." << std::endl;
    exit(1);
  } else {
    std::cout << "writing data... (to the out.csv file)" << std::endl;
    // print header column name to the file :
        file << "id,last-durations,longest-durations" << "\n";
    // print data to the file :
    for (const auto &id : ids) {
      // find itearation
      UMapVec::iterator it = umapvec.find(id);

      // check if duration size less than 29 step
      size_t count = it->second.size();
      size_t step = (count > 29) ? 29 : count;

      // id
      file << id << ",";

      // last duration
            for (auto rit = it->second.crbegin(); rit != (it->second.crbegin() + step); ++rit)
        file << *rit << "|";
      if (30 <= count)
        file << *(it->second.crbegin() + 30) << ",";
      else
        file << ",";

      // sort durations descending order :
      std::sort(it->second.rbegin(), it->second.rend());

      // long duration
            for (auto jit = it->second.cbegin(); jit != (it->second.cbegin() + step); ++jit)
        file << *jit << "|";
      if (30 <= count)
        file << *(it->second.cbegin() + 30) << "\n";
      else
        file << "\n";

      // umapvec.erase(it);
    }

    std::cout << "close the out.csv file." << std::endl;
    file.close();
  }

  auto endTimerPoint = std::chrono::high_resolution_clock::now();
    auto start = std::chrono::time_point_cast<std::chrono::seconds>(startTimepoint).time_since_epoch().count();
    auto end = std::chrono::time_point_cast<std::chrono::seconds>(endTimerPoint).time_since_epoch().count();
  auto duration = end - start;
  double minutes = duration / 60;
  double seconds = duration % 60;
    std::cout << "\n\ndone. (duration : " << minutes << " minutes and " << seconds << " seconds)" << std::endl;
  return 0;
}