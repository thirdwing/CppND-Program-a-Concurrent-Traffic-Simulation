#include <iostream>
#include <random>
#include <future>
#include "TrafficLight.h"

using MS = std::chrono::milliseconds;

/* Implementation of class "MessageQueue" */

template <typename T>
T MessageQueue<T>::receive()
{
    // FP.5a : The method receive should use std::unique_lock<std::mutex> and _condition.wait() 
    // to wait for and receive new messages and pull them from the queue using move semantics. 
    // The received object should then be returned by the receive function. 
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex> 
    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.
}

/* Implementation of class "TrafficLight" */

TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}

void TrafficLight::waitForGreen()
{
    // FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop 
    // runs and repeatedly calls the receive function on the message queue. 
    // Once it receives TrafficLightPhase::green, the method returns.
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::simulate()
{
  threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
  std::random_device rd;
  std::mt19937 eng(rd());
  std::uniform_int_distribution<> dist(4, 6);

  int cycle_duration = dist(eng);

  auto last_update = std::chrono::system_clock::now();

  while (true) {
    int elapsed = std::chrono::duration_cast<std::chrono::seconds>(
                      std::chrono::system_clock::now() - last_update)
                      .count();
    std::this_thread::sleep_for(MS(1));
    if (elapsed >= cycle_duration) {
      if (_currentPhase == TrafficLightPhase::red) {
        _currentPhase = TrafficLightPhase::green;
      } else {
        _currentPhase = TrafficLightPhase::red;
      }
      TrafficLightPhase msg = _currentPhase;
      std::future<void> fut =
          std::async(std::launch::async, &MessageQueue<TrafficLightPhase>::send,
                     _queue, std::move(msg));
      fut.wait();

      last_update = std::chrono::system_clock::now();

      cycle_duration = dist(eng);
    }
  }
}
