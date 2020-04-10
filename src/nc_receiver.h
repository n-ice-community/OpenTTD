#ifndef NC_RECEIVER_H
#define NC_RECEIVER_H

#include <string>

class Receiver
{
public:
  virtual ~Receiver() {}
  virtual void DataReceived(std::string) = 0;
  virtual void Failed() = 0;
};

#endif
