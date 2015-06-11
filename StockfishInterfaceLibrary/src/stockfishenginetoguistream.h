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

#ifndef __StockfishAnalysis__standardinputcapturestream__
#define __StockfishAnalysis__standardinputcapturestream__

#include <sstream>
#include <vector>
#include <pthread.h>

class APTrie;
class StockfishEngineToGUIInterface;

typedef enum {
    StockfishResponseTokenId = 0x7AAAAA01,
    StockfishResponseTokenUCIOK,
    StockfishResponseTokenREADYOK,
    StockfishResponseTokenBESTMOVE,
    StockfishResponseTokenCOPYPROTECTION,
    StockfishResponseTokenREGISTRATION,
    StockfishResponseTokenINFO,
    StockfishResponseTokenOPTION
}StockfishResponseTokenType;

typedef enum {
    StockfishInfoToken_DEPTH = 0x7BBBBB01,
    StockfishInfoToken_SELDEPTH,
    StockfishInfoToken_TIME,
    StockfishInfoToken_NODES,
    StockfishInfoToken_PV,
    StockfishInfoToken_MULTIPV,
    StockfishInfoToken_SCORE,
    StockfishInfoToken_CURRMOVE,
    StockfishInfoToken_CURRMOVENUMBER,
    StockfishInfoToken_NPS,
}StockfishInfoTokenType;

typedef enum {
    StockfishInfoScore_CP = 0x7CCCCC01,
    StockfishInfoScore_MATE,
    StockfishInfoScore_LOWERBOUND,
    StockfishInfoScore_UPPERBOUND
}StockfishInfoScoreTokenType;

#define TOTAL_RESPONSE_STARTER_TOKENS 8
#define TOTAL_INFO_TOKENS 10
#define TOTAL_INFO_SCORE_TOKENS 4

class StockfishEngineToGUIStream : public std::streambuf {
public:
    explicit StockfishEngineToGUIStream(std::ostream &sink);
    virtual ~StockfishEngineToGUIStream();
    
    void SetEngineToGUIDelegate(StockfishEngineToGUIInterface* engineToGUIDelegate);
private:
    int_type overflow(int_type ch);
    int sync();
    void parseCurrentLineAndSendGUIResponse();
    // copy ctor and assignment not implemented;
    // copying not allowed
    StockfishEngineToGUIStream(const StockfishEngineToGUIStream &);
    StockfishEngineToGUIStream &operator= (const StockfishEngineToGUIStream &);
    
    std::ostream &_sink;
    std::streambuf* _coutBuff;
    bool _firstWord;
    std::string _curLine;
    int _capturingResponse;
    
    APTrie* _keywordTrie;
    StockfishEngineToGUIInterface* _engineToGUIDelegate;
    
    static const char* _responseStarterTokens[TOTAL_RESPONSE_STARTER_TOKENS];
    static const char* _infoTokens[TOTAL_INFO_TOKENS];
    static const char* _infoScoreTokens[TOTAL_INFO_SCORE_TOKENS];
};

#endif /* defined(__standardinputcapturestream__) */
