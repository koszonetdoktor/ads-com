#include <nan.h>
#include <string>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <thread>
#include "streaming-worker.h"

using namespace std;

class AdsCom : public StreamingWorker {
    public: 
    AdsCom(Callback *data, Callback *complete, Callback *error_callback, v8::Local<v8::Object> & options)
        : StreamingWorker(data, complete, error_callback) {

            // vairable can be added here eg:
            //sum = 0;
            if(options->IsObject()) {
                // do soemting with te options if I wish
            }  
        }

    void Execute (const AsyncProgressWorker::ExecutionProgress& progress) {
        // DO te job here. 
        int value;
        Message m = fromNode.read();
        value = stoi(m.data);
        Message toSend("name", m.name);
        writeToNode(progress, toSend);
        for (int i = 0; i < value; i++ ) {
            Message tosend("integer", std::to_string(i));
            writeToNode(progress, tosend);
            std::this_thread::sleep_for(chrono::milliseconds(50));
        }
    }
    //if varibale was added that private declaration must be added also here, eg: 
    //private: 
    //  int sum;
};
// Adddon setup code
StreamingWorker * create_worker(Callback *data 
    , Callback *complete
    , Callback *error_callback, v8::Local<v8::Object> & options) {
    return new AdsCom(data, complete, error_callback, options);
}

NODE_MODULE(ads_com, StreamWorkerWrapper::Init)