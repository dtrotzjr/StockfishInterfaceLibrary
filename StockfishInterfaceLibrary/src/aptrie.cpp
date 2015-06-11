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

#include "aptrie.h"
#include "assert.h"

APTrie::APTrie()
{
    _root = new APTrieNode();
}

APTrie::~APTrie()
{
    delete _root;
}

int APTrie::IdForToken(const char* token)
{
    return _matchesId(_root->_children, token);
}

void APTrie::AddToken(const char* token, int tokenId)
{
    assert(token[0] != 0 && tokenId != 0);
    char c = token[0];
    if (_root->_children == NULL) {
        // First Entry
        _addChild(_root, token, tokenId);
    } else {
        APTrieNode* node = _nodeForChar(_root->_children, c);
        if (node->_key == c) {
            _addChild(node, &token[1], tokenId);
        } else {
            _addNext(node, token, tokenId);
        }
    }
}

APTrie::APTrieNode* APTrie::_nodeForChar(APTrie::APTrieNode* level, const char c)
{
    assert(level != NULL);
    if (level->_key != c && level->_next != NULL) {
        return _nodeForChar(level->_next, c);
    }
    return level;
}

void APTrie::_addChild(APTrie::APTrieNode* root, const char* token, int tokenId)
{
    assert(root != NULL);
    if (root->_children == NULL) {
        root->_children = new APTrieNode();
        root->_children->_key = token[0];
        if (root->_children->_key == 0) {
            root->_children->_value = tokenId;
        } else {
            _addChild(root->_children, &token[1], tokenId);
        }
    } else if (root->_children->_key == token[0]) {
        _addChild(root->_children, &token[1], tokenId);
    } else {
        APTrieNode* node = _nodeForChar(root->_children, token[0]);
        if (node->_key == token[0]) {
            _addChild(node, &token[1], tokenId);
        } else {
            _addNext(node, token, tokenId);
        }
    }
}

void APTrie::_addNext(APTrie::APTrieNode* root, const char* token, int tokenId)
{
    assert(root != NULL && root->_next == NULL);
    root->_next = new APTrieNode();
    root->_next->_key = token[0];
    if (root->_next->_key == 0) {
        root->_next->_value = tokenId;
    } else {
        _addChild(root->_next, &token[1], tokenId);
    }
}

int APTrie::_matchesId(APTrie::APTrieNode* root, const char* token)
{
    if (root != NULL) {
        if (root->_key == token[0]) {
            if (token[0] == 0) {
                return root->_value;
            } else {
                return _matchesId(root->_children, &token[1]);
            }
        } else {
            return _matchesId(root->_next, token);
        }
    }
    return -1;
}

#pragma mark - APTrieNode
APTrie::APTrieNode::APTrieNode()
{
    _key = -1;
    _value = -1;
    _next = NULL;
    _children = NULL;
}

APTrie::APTrieNode::~APTrieNode()
{
    if (_next)
        delete _next;
    if (_children)
        delete _children;
}