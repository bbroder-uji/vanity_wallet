
#include <sodium.h>
#include <string>
#include <vector>
#include <cassert>
#include <iostream>
#include <map>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <random>
#include <cstdlib>

#include <thread>
#include <atomic>
#include <mutex>

#include "base.h"
#include "mnemonic.h"
#include "words.h"
#include "address.h"

std::atomic<int> atomic_count{0};
int last_count{0};
std::chrono::time_point<std::chrono::system_clock> last_time;
std::mutex cout_mutex;

int main (int argc, char** argv)
{

  // Hunt for a wallet that matches one of the target words.
  auto work = [] (int seeder)
  { 
    unsigned char ed25519_pk[crypto_sign_ed25519_PUBLICKEYBYTES];
    unsigned char ed25519_sk[crypto_sign_ed25519_SECRETKEYBYTES];
    const int ed25519_pk_size = sizeof(ed25519_pk);
    const std::vector<std::pair<bytes,std::pair<int,int>>> & matches = english_5letter_matches;

    bytes seed = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    randombytes_buf(seed.data(), sizeof seed);
    std::mt19937 mt(std::time(nullptr) ^ seed[0]);
  
    while (true)
    {
      auto m = mt();
      if  (!m) m++;
      seed[m%32]++;

      crypto_sign_ed25519_seed_keypair (ed25519_pk, ed25519_sk, seed.data());
      atomic_count++;

      for (auto & matchpair : matches)
      {
	  // if the first bytes match
          if ((std::equal (matchpair.first.begin(), matchpair.first.end() - 1, ed25519_pk)))
	  { 
	    // then check the final mask
	    auto & match = matchpair.first;
            auto & mask = matchpair.second;
	    if ((ed25519_pk[mask.second] & mask.first) == match[mask.second])
	    {
	      // lock for logging and encoding
	      std::lock_guard<std::mutex> m(cout_mutex);
              Address a(bytes{ed25519_pk, &ed25519_pk[ed25519_pk_size]});
              std::cout << std::time(nullptr) << " : " << a.as_string << "\n"; 
              std::cout << std::time(nullptr) << " : " << mnemonic_from_seed(seed)<< "\n" << std::flush;
              randombytes_buf(seed.data(), sizeof seed);
	    }
	  }
      }
    }
  };

  // log some speed stats occasionally if they change
  auto speed = [] ()
  {
      double last_per = 1;
      while (true)
      {
        std::this_thread::sleep_for(std::chrono::milliseconds(10000));
        int count = atomic_count;
        {
  	  std::lock_guard<std::mutex> m(cout_mutex);
          auto this_time = std::chrono::system_clock::now();
          std::chrono::duration<double> diff = this_time - last_time;
          double per = (double)((count - last_count)) / (double)(diff.count());
	  if ((diff.count() != 0) && ( (abs(last_per - per) / per) > 0.1))
	  {
              double per = (double)((count - last_count)) / (double)(diff.count());

	      std::cout << std::time(nullptr) << " : " << count << ", " << (per/1000.0) << " k hashes per second\n" << std::flush;
	      last_time = this_time;
	      last_count = count;
	      last_per = per;
          }
	}
      }
  };

  int sodium_init_return = sodium_init();
  last_time = std::chrono::system_clock::now();
  std::srand(std::time(nullptr));

  // launch the threads and play around with affinity.
  std::vector<std::thread> threads;
  threads.push_back (std::thread(speed));
  for (auto i = 0; i != 32; i++) {
    threads.push_back (std::thread(work, i*100));
    cpu_set_t cpuset;
    CPU_ZERO (&cpuset);
    CPU_SET(i, &cpuset);
    pthread_setaffinity_np (threads[i+1].native_handle(), sizeof(cpu_set_t), &cpuset);
  }
  threads[0].join();

  std::cout << "end of program" << "\n";
  return 0;
}
