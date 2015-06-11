/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Copyright David Trotz david.trotz@gmail.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include "stockfishremote.h"
#include "bitboard.h"
#include "evaluate.h"
#include "position.h"
#include "search.h"
#include "thread.h"
#include "tt.h"
#include "uci.h"
#include "stockfishguitoenginestream.h"
#include "stockfishenginetoguistream.h"
#include "stockfishenginetoguiinterface.h"

#include <iostream>
#include <pthread.h>
#include <sstream>

StockfishRemote* StockfishRemote::_instance = NULL;

void *_loop_kickoff(void *context) {
    pthread_setname_np("com.stockfish.loop");
    StockfishRemote* instance = reinterpret_cast<StockfishRemote*>(context);
    instance->_loop();
    fprintf(stdout, "Exiting loop thread");
    return NULL;
}

StockfishRemote::StockfishRemote()
{
    _guiToEngineStream = NULL;
    _cinBuff = NULL;
    _ready = false;
    _loop_kickoff_thread = NULL;
    pthread_mutex_init(&_ready_mutex, NULL);
    pthread_cond_init(&_ready_condition, NULL);

}

StockfishRemote::~StockfishRemote()
{
    std::cin.rdbuf(_cinBuff);
    delete _guiToEngineStream;
    delete _engineToGUIStream;
    pthread_cond_destroy(&_ready_condition);
    pthread_mutex_destroy(&_ready_mutex);
}

void StockfishRemote::InitEngine()
{
    if (_loop_kickoff_thread == NULL) {
        std::cout << engine_info() << std::endl;
        UCI::init(Options);
        Bitboards::init();
        Position::init();
        Bitbases::init();
        Search::init();
        Eval::init();
        Pawns::init();
        Threads.init();
        TT.resize(Options["Hash"]);
        
        _guiToEngineStream = new StockfishGUIToEngineStream();
        _cinBuff = std::cin.rdbuf();
        std::cin.rdbuf(_guiToEngineStream);
        
        _engineToGUIStream = new StockfishEngineToGUIStream(std::cout);
        
        pthread_create(&_loop_kickoff_thread, NULL, _loop_kickoff, this);
    } else {
        fprintf(stderr, "ERROR: Engine already initialized");
    }
}

StockfishRemote* StockfishRemote::Instance()
{
    if (StockfishRemote::_instance == NULL) {
        StockfishRemote::_instance = new StockfishRemote();
    }
    return StockfishRemote::_instance;
}

void StockfishRemote::_loop()
{
    pthread_mutex_lock(&_ready_mutex);
    _ready = true;
    pthread_cond_signal(&_ready_condition);
    pthread_mutex_unlock(&_ready_mutex);
    UCI::loop(1, NULL);
}

void StockfishRemote::SendCommand(const char* command)
{
    pthread_mutex_lock(&_ready_mutex);
    while (!_ready)
        pthread_cond_wait(&_ready_condition, &_ready_mutex);
    pthread_mutex_unlock(&_ready_mutex);
    
    size_t commandLen = strlen(command);
    if (command[commandLen - 1] != '\n') {
        char* tmp = (char*)calloc(commandLen + 2, sizeof(char));
        strcpy(tmp, command);
        tmp[commandLen] = '\n';
        tmp[commandLen + 1] = 0;
        
        _guiToEngineStream->add(tmp);
        free(tmp);
    } else {
        _guiToEngineStream->add(command);
    }
}

void StockfishRemote::SetEngineToGUIDelegate(StockfishEngineToGUIInterface* engineToGUIDelegate)
{
    _engineToGUIStream->SetEngineToGUIDelegate(engineToGUIDelegate);
}