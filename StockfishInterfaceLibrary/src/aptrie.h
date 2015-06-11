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

#ifndef __StockfishAnalysis__aptrie__
#define __StockfishAnalysis__aptrie__

#include <stdio.h>

class APTrie {
public:
    APTrie();
    virtual ~APTrie();
    
    int IdForToken(const char* token);
    void AddToken(const char* token, int tokenId);
private:
    APTrie(const APTrie& other);
    APTrie& operator=(const APTrie& other);
    
    class APTrieNode {
    public:
        APTrieNode();
        ~APTrieNode();
        char _key;
        int _value;
        APTrieNode* _next;
        APTrieNode* _children;
    };
    
    APTrie::APTrieNode* _nodeForChar(APTrie::APTrieNode* level, const char c);
    void _addChild(APTrie::APTrieNode* root, const char* token, int tokenId);
    void _addNext(APTrie::APTrieNode* root, const char* token, int tokenId);
    int _matchesId(APTrie::APTrieNode* root, const char* token);
    
    APTrieNode* _root;
    
};

#endif /* defined(__StockfishAnalysis__aptrie__) */
