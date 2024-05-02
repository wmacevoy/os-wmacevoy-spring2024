#include <mutex>
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <functional>
#include <vector>

//
// This one is a little less clever with shared pointers, it just uses them
// to manage forks and philosophers, not to obtain lock gaurds...
//

//
// All threads share console logging, so Logger managages one ostream...
//
class Logger {
private:
  std::mutex mutex;
  std::ostream &out;
public:
  Logger(std::ostream &_out) : out(_out) {}

  // The print(out) of notes happens one at a time
  void note(std::function < void (std::ostream &out) > print) {
    std::lock_guard < std::mutex > gaurd(mutex);
    print(out); out << std::endl;
  }
};

Logger outLogger(std::cout);

// Macro to use logger for formatting messages synchronously, the print(out)
// is a lambda which is executed only after obtaining the logger murtex.
#define NOTE(MESSAGE) outLogger.note([&](auto &out) { out << MESSAGE; })

// forward declaration of these classes.
class Fork;
class Philosopher;

//
// Fork is a shared resource - two philosophers will want to access each fork
//
class Fork {
public:
  using Guard = std::lock_guard < std::mutex >;
  const int id;
private:
  std::mutex mutex;
public:
Fork(int _id)
    : id(_id) {}
  // so Philosopher has access to Fork's mutex
  friend Philosopher;
};

//
// #size philosophers are in a room and they think (independently) or eat,
// to eat they sit at their spot at a round table and pick up the forks
// on the left and right, then eat, put them down, and go back to thinking.
//
class Philosopher {
public:
  const int id;
  volatile bool alive;
private:
  Fork &leftFork;
  Fork &rightFork;
  std::thread *thread;
public:
  Philosopher(int _id, Fork &_leftFork, Fork &_rightFork)
    : id(_id), alive(false), leftFork(_leftFork), rightFork(_rightFork), thread(nullptr) {}

  void start() {
    alive = true;
    thread = new std::thread(&Philosopher::live, this);
  }

  void stop() {
    alive = false;
  }
  
  void join() {
    alive = false;
    if (thread != nullptr) {
      thread->join();
      delete thread;
      thread = nullptr;
    }
  }

private:
  void live() {
    NOTE("Philosopher " << id << " is alive.");
    while (alive) {
      think();
      eat();
    }
    NOTE("Philosopher " << id << " is dead.");
  }
  
  void think() {
    NOTE("Philosopher " << id << " is thinking.");
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  
  void eat() {
    NOTE("Philosopher " << id << " is hungry.");
    Fork::Guard leftForkGaurd(leftFork.mutex);
    NOTE("Philosopher " << id << " got left fork " << leftFork.id << ".");
    Fork::Guard rightForkGaurd(rightFork.mutex);
    NOTE("Philosopher " << id << " got right fork " << rightFork.id << ".");

    NOTE("Philosopher " << id << " is eating.");
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
};

class Room {
public:
  const size_t size;
  std::vector < std::shared_ptr < Fork > > forks;
  std::vector < std::shared_ptr < Philosopher > > philosophers;
  Room(size_t _size) : size(_size), forks(size), philosophers(size) {
    for (size_t id = 0; id<size; ++id) {
      forks[id]=std::make_shared < Fork > (id);
    }

    for (size_t id = 0; id<size; ++id) {
      Fork &left = *forks[id];
      Fork &right = *forks[(id+1) % size];
      philosophers[id]=std::make_shared < Philosopher > (id, left, right);
    }
  }

  void start() {
    for (size_t id = 0; id != size; ++id) {
      philosophers[id]->start();
    }
  }

  void stop() {
    for (size_t id = 0; id != size; ++id) {
      philosophers[id]->stop();
    }
  }

  void join() {
    for (size_t id = 0; id<size; ++id) {
      philosophers[id]->join();
    }
  }
};

//
// helper to process command line options
// if arg starts with pfx, call match with the rest of the argument, and return true
// otherwise return false.
//
bool option(const std::string &arg,
	const std::string &pfx,
	std::function<void (const char *rest)> match) {
  if (arg.compare(0,pfx.size(),pfx) == 0) {
    const char *rest=arg.c_str()+pfx.size();
    match(rest);
    return true;
  }
  return false;
}

int main(int argc, const char *argv[]) {
  size_t size = 4;
  int time = 10;
  
  for (int i=1; i<argc; ++i) {
    std::string arg=argv[i];
    if (option(arg,"--size=",[&](auto _size) { size = atoi(_size); })) continue;
    if (option(arg,"--time=",[&](auto _time) { time = atoi(_time); })) continue;
    std::cout << "unknown argument " << arg << std::endl;
  }

  Room room(size);

  room.start();
  std::this_thread::sleep_for(std::chrono::seconds(time));
  room.stop();
  room.join();

  return 0;

}

