//
// Created by Pfifer, Justin on 3/7/17.
//

#ifndef AMAZON_KINESIS_PRODUCER_INTERNAL_NATIVETHREAD_H
#define AMAZON_KINESIS_PRODUCER_INTERNAL_NATIVETHREAD_H

#include <pthread.h>

class NativeThread {
private:
  pthread_t thread_id;

};


#endif //AMAZON_KINESIS_PRODUCER_INTERNAL_NATIVETHREAD_H
