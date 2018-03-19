#include <nan.h>
#include <string>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <thread>
#include <windows.h>
#include <conio.h>
#include <assert.h>
#include <winbase.h>
#include "streaming-worker.h"
#include "include\TcAdsDef.h"
#include "include\TcAdsAPI.h"

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
        // for (int i = 0; i < value; i++ ) {
        //     Message tosend("integer", std::to_string(i));
        //     writeToNode(progress, tosend);
        //     std::this_thread::sleep_for(chrono::milliseconds(50));
        // }
        /*************************************
            Status request from the PLC
        **************************************/
        long          nErr, nPort;
        AmsAddr       Addr;
        PAmsAddr      pAddr = &Addr;
        unsigned long lHdlVar;
        int           nIndex;
        short         Data[255];
        char          szVar[] = { "GVL_HOIST.stHoistPlcToOpc" };

        cout << "open communication !4";
        // Open communication port on the ADS router
        nPort = AdsPortOpen();
        nErr = AdsGetLocalAddress(pAddr);
        if (nErr) cerr << "Error: AdsGetLocalAddress: " << nErr << '\n';

        // Select Port: TwinCAT 3 PLC1 = 851
        pAddr->port = 851;

        // Fetch handle for the PLC variable 
        cout << "sync read write request fired\n";
        nErr = AdsSyncReadWriteReq(pAddr, ADSIGRP_SYM_HNDBYNAME, 0x0, sizeof(lHdlVar), &lHdlVar, sizeof(szVar), szVar);
        if (nErr) cerr << "Error: AdsSyncReadWriteReq: " << nErr << '\n';
        // Read values of the PLC variables (by handle)
        cout << "sync read request fired\n";
        nErr = AdsSyncReadReq(pAddr, ADSIGRP_SYM_VALBYHND, lHdlVar, sizeof(Data), &Data[0]);
        if (nErr)
            cerr << "Error: AdsSyncReadReq: " << nErr << '\n';
        else
        {   
            cout << "jump into the loop\n";
            for (nIndex = 0; nIndex < 255; nIndex++)
                //cout << "Data[" << nIndex << "]: " << Data[nIndex] << '\n';
                Message toSend("index", std::to_string(nIndex));
                writeToNode(progress, toSend);
        }
        cout.flush();
        //_getch();
        // Close communication port
        nErr = AdsPortClose();
        if (nErr) cerr << "Error: AdsPortClose: " << nErr << '\n';
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