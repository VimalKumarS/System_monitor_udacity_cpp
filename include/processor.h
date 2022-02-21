#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  long  prevuser , prevnice , prevsystem , previrq , prevsoftirq , prevsteal, previdle , previowait;
};

#endif