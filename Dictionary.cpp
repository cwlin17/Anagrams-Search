// Dictionary.cpp

#include "Dictionary.h"
#include <string>
#include <list>
#include <cctype>
#include <utility>  // for swap
#include <functional>
#include <algorithm>
using namespace std;

const int NUM_BUCKETS = 50000;
struct Node {
	string key;
	list<string> values;
};
typedef list<Node> Bucket;

void removeNonLetters(string& s);

// This class does the real work of the implementation.

class HashTable {
public:
	HashTable() {}
	~HashTable() {}
	void insert(string word);
	void lookup(string letters, void callback(string)) const;
private:
	Bucket m_bucket[NUM_BUCKETS];
	// Hash function for strings
	unsigned int HashFunction(const string &HashMe) const {
		hash<string> str_hash;
		unsigned int hashValue = str_hash(HashMe);
		unsigned int bucketNum = hashValue % NUM_BUCKETS;
		return bucketNum;
	}
};

// Inserts a word into the hash table based off of a key. 
// The key is the sorted version of the word. If the key exists
// in the list of nodes at a certain bucket, then push the word into the node's list.
// If the key doesn't already exist, create a new node, push the word into 
// the node's list, and then push the new node into the list of nodes at the bucket.
void HashTable::insert(string word) {
	// Copy of the word is made, sorted, and hashed
	string key = word;
	sort(key.begin(), key.end());
	int bucketNum = HashFunction(key);
	// Checking if the key already exists in the list of nodes at a given bucket
	for (Bucket::iterator itr = m_bucket[bucketNum].begin(); itr != m_bucket[bucketNum].end(); itr++) {
		if (itr->key == key) {
			itr->values.push_back(word);
			return;
		}
	}
	// Creating a new node that contains the word and
	// adding it to the list of nodes at the given bucket
	Node m_node;
	m_node.key = key;
	m_node.values.push_back(word);
	m_bucket[bucketNum].push_back(m_node);
}

// Looks up all anagrams of the given string in the hash table
// and prints them out if they're found.
void HashTable::lookup(string letters, void callback(string)) const {
	// Sort the string and hash it to determine the correct bucket
	sort(letters.begin(), letters.end());
	int bucketNum = HashFunction(letters);
	// Looking through the list of nodes at the bucket. If the sorted string is equal 
	// to the key of a node, loop through the list of values at that node and print
	// each value out.
	for (Bucket::const_iterator itr = m_bucket[bucketNum].begin(); itr != m_bucket[bucketNum].end(); itr++) {
		if (itr->key == letters) {
			for (list<string>::const_iterator itr2 = itr->values.begin(); itr2 != itr->values.end(); itr2++) {
				callback(*itr2);
			}
			return;
		}
	}
}

class DictionaryImpl
{
public:
	DictionaryImpl() {}
	~DictionaryImpl() {}
	void insert(string word);
	void lookup(string letters, void callback(string)) const;
private:
	HashTable m_words;
};

// Inserts the word into the dictionary
void DictionaryImpl::insert(string word)
{
	removeNonLetters(word);
	if (!word.empty())
		m_words.insert(word);
}

// Looks up all anagrams of the string and prints them out if found
void DictionaryImpl::lookup(string letters, void callback(string)) const
{
	if (callback == nullptr)
		return;

	removeNonLetters(letters);
	if (letters.empty())
		return;

	m_words.lookup(letters, callback);
}

void removeNonLetters(string& s)
{
	string::iterator to = s.begin();
	for (string::const_iterator from = s.begin(); from != s.end(); from++)
	{
		if (isalpha(*from))
		{
			*to = tolower(*from);
			to++;
		}
	}
	s.erase(to, s.end());  // chop everything off from "to" to end.
}

//******************** Dictionary functions ********************************

// These functions simply delegate to DictionaryImpl's functions.
// You probably don't want to change any of this code

Dictionary::Dictionary()
{
	m_impl = new DictionaryImpl;
}

Dictionary::~Dictionary()
{
	delete m_impl;
}

void Dictionary::insert(string word)
{
	m_impl->insert(word);
}

void Dictionary::lookup(string letters, void callback(string)) const
{
	m_impl->lookup(letters, callback);
}
