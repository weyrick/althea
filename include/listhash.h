/*

	project althea
	original code: weyrick
	
	copyright (c) 2000 Roadsend, Inc.


	DESCRIPTION:
	  template for a doubly linked list that uses a hash table for lookups.
 
        NOTE: The TYPE *data must be CREATED (with the 'new' operator) and passed to the addItem function, but it
        should not be explicitly DELETED (with the 'delete' operator) outside of these functions, as removeItem 
        will handle this for you. this was done so we can do a freeList() and free all memory the list is currently
        taking, for rehashes, reloads, etc.


	CHANGE HISTORY:
	
*/

#ifndef LISTHASH_H
#define LISTHASH_H

/* LOCAL INCLUDES */
#include "String.h"
#include <ctype.h>

// how big should the hash table be? 
#define HASHTABLESIZE 512

// debugging? 
#undef HASH_DEBUG

// error return values
#define HASH_STATUS_OK          0
#define HASH_ERROR_NOMEMORY		1
#define HASH_ERROR_NOITEMS      2
#define HASH_ERROR_NOTFOUND     3
#define HASH_ERROR_NULLNODE	    4
#define HASH_ERROR_DUPLICATEKEY	5
	   
typedef unsigned short int HashIndexType;

// single node in our hash list 
template<class TYPE>
struct lh_node
{
	
	TYPE* data;         // pointer to the data                       
	String key;          // key by which we will recognize this node
	
	lh_node* nextNode;  // next in linked list
	lh_node* prevNode;  // prev in linked list
	
	lh_node* nextHash;  // next in hash list
	lh_node* prevHash;  // prev in hash list
	
};

template<class TYPE>
class Hash
{
	
public:

	// default constructor
	Hash(void) { 

		// clear and setup our variables for a clean list
		firstNode = NULL;
		lastNode = NULL;
		lastFind = NULL;
		currNode = NULL;
		numItems = 0;
		caseSensitive = true;

		// reset hash table
		for (int i=0; i < HASHTABLESIZE; i++)
			hashTable[i] = NULL;
	}
	
	// free memory upon deletion of object
	~Hash(void) { freeList(); }

	// item lookup by key
	TYPE * operator [] (String searchID) { return findItem(searchID); }

	// change whether we should use case sensitive keys or not
	void setCaseSensitivity(bool c) { caseSensitive = c; }

	// add an item to the hash table. data should be created (with "new") then passed to this function
	int addItem(TYPE* newNode, String hashID) {

		// sanity check 
		if (!newNode)
			return HASH_ERROR_NULLNODE;

		// make sure we haven't already used this key
		if (findItem(hashID))
			return HASH_ERROR_DUPLICATEKEY;

		// create a new local node with the data they've passed 
		lh_node<TYPE>* tempItem;
		lh_node<TYPE>* newLink = new lh_node<TYPE>;

		if (!newLink)
			return HASH_ERROR_NOMEMORY;

		numItems++;

		newLink->prevHash = NULL;
		newLink->nextHash = NULL;

		newLink->nextNode = NULL;
		newLink->prevNode = NULL;

		newLink->data = newNode;

		if (!caseSensitive)
			hashID.toupper();

		newLink->key = hashID;

		// add to hash table 
		HashIndexType bucket = hash(hashID);
		tempItem = hashTable[bucket];
		hashTable[bucket] = newLink;
		newLink->nextHash = tempItem;
		if (tempItem)
			tempItem->prevHash = newLink;
		///*****************

		// ie, first in list 
		if (!firstNode) {
			firstNode = newLink;
			lastNode = newLink;
			currNode = firstNode;
			return HASH_STATUS_OK;
		}

		// ie, not first in list.. 
		lastNode->nextNode = newLink;
		newLink->prevNode = lastNode;
		lastNode = newLink;	

		return HASH_STATUS_OK;


	}

	// retrieve an item from the hash by it's key
	TYPE* findItem(String searchID) {

		// check the last node we searched, we might already have it
		if ((lastFind)&&(lastFind->key == searchID))
			return lastFind->data;	// bingo, no lookups

		HashIndexType bucket = hash(searchID);
		
		lh_node<TYPE>* current = hashTable[bucket];
		
		if (!current)
			return NULL;
		
		if (!caseSensitive)
			searchID.toupper();
		
		// hunt through list 
		while (current) {
			if (current->key == searchID) {
				lastFind = current;
				return current->data;
			}
			current = current->nextHash;
		}
		
		// if we get here, we didn't find it.. ;\ 
		return NULL;

	}

	// remove an item from the hash table by it's key
	int removeItem(String hashID, bool saveData = false) {

		HashIndexType bucket = hash(hashID);
		
		lh_node<TYPE>* current = hashTable[bucket];
		
		if (!current) {
			// no items in list 
			return HASH_ERROR_NOITEMS;
		}
		
		if (!caseSensitive)
			hashID.toupper();
		
		// hunt through list 
		while (current) {
	
			if (current->key == hashID) {
				// found the one we want to delete 
				
				// remove from hash 
				if (hashTable[bucket] == current)
					hashTable[bucket] = current->nextHash;
				if (current->prevHash)
					current->prevHash->nextHash = current->nextHash;
				if (current->nextHash)
					current->nextHash->prevHash = current->prevHash;
				
				
				// remove from list 
				if (!current->nextNode)
					// at the end of the list, so update lastNode 
					lastNode = current->prevNode;
				
				if (!current->prevNode) {
					if (!current->nextNode) {
						// only node in list, so update firstNode 
						firstNode = NULL;
					}
					else {
						// was first in list, so update firstNode 
						firstNode = current->nextNode;
					}
				}
				
				if (current->prevNode)
					current->prevNode->nextNode = current->nextNode;
				if (current->nextNode)
					current->nextNode->prevNode = current->prevNode;
				
				if (!saveData)
					// delete the TYPE* data 
					delete current->data;
				
				// nuke it 
				delete current;
				
				numItems--;
				
				// status OK 
				return HASH_STATUS_OK;
			}
			current = current->nextHash;
		}
		
		// item not found 
		return HASH_ERROR_NOTFOUND;

	}

	// rehash an item with a new key
	int reHashItem(String oldID, String newID) {


		TYPE * saveData = findItem(oldID);

		if (!saveData)
			return HASH_ERROR_NOTFOUND;

		// remove item (w/o deleteing object) 
		removeItem(oldID, true);
		addItem(saveData, newID);

		return HASH_STATUS_OK;

	}
	
	// is list empty? returns true/false
	bool isListEmpty(void) { return (firstNode) ? false : true; }
	
	// return the object at the "current" location of the internal node pointer
	TYPE* current(void) { 
		if (currNode)
			return (currNode->data);
		else
			 return NULL; 
	}
	
	// advance the internal node pointer, and return the next object
	TYPE* next(void) { 
		if (!currNode) {
			return NULL;
		}
		currNode = currNode->nextNode;
		if (currNode)
			return currNode->data;
		else
			return NULL;
	}
	
	// reset the internal node pointer to the first node in the list
	void resetCounter(void) { currNode = firstNode; }
	
	// delete all NODES and optionally data (freeing memory) in list
	int freeList(bool saveData=false);
		
	// stuff the passed array with a list of itemID's, up to max
	int getKeys(String itemList[], int max);

#ifdef HASH_DEBUG
	// debug output dump of the hash table
	void display();
#endif
	
	// retrieve the number of items in the list
	unsigned int getNumItems(void) { return numItems; }
	
private:

    // return a hash value based on a key
	HashIndexType hash(String hId) {

		HashIndexType h;
		unsigned char h1, h2;
		
		if (!caseSensitive)
			hId.toupper();
		
		const char *str = hId.getCstr();
		
		if (*str == 0) return 0;
		h1 = toupper(*str); h2 = toupper(*str) + 1;
		str++;
		while (*str) {
			h1 = h1 ^ toupper(*str);
			h2 = h2 ^ toupper(*str);
			str++;
		}
		
		// h is in range 0..65535 
		h = ((HashIndexType)h1 << 8)|(HashIndexType)h2;
		// use division method to scale 
		HashIndexType retVal = h % HASHTABLESIZE;
	
		#ifdef HASH_DEBUG
		cout << "hash bucket ["<< hId << "] = " << retVal << endl;
		#endif
	
		return retVal;

	}	  			
	
	/* DATA */

	unsigned int numItems;						// number of items in the list

	bool caseSensitive;							// should the key's be case sensitive?

	lh_node<TYPE>* firstNode;					// first node in list
	lh_node<TYPE>* lastNode;					// last node in list
	
	lh_node<TYPE>* lastFind;  					// the last node we did a findItem() for
	lh_node<TYPE>* currNode;  					// the current node pointer for next()
	
	lh_node<TYPE>* hashTable[HASHTABLESIZE];  	// the hash table
	
};


// implementation 

// stuff the passed array with a list of itemID's, up to max 
// will return the number of keys actually returned 
template <class TYPE>
int Hash<TYPE>::getKeys(String itemList[], int max) {

	lh_node<TYPE>* current = firstNode;
	
	if (!current) {
		return 0;
	}
	
	int x = 0;
	
	// hunt through list 
	while ((current) && (x < max)) {
		itemList[x++] = current->key;
		current = current->nextNode;
	}
	
	return x;
}

// deletes ALL nodes and optionally data in the list! 
template <class TYPE>
int Hash<TYPE>::freeList(bool saveData=false)
{
	
	lh_node<TYPE>* current = firstNode;
	lh_node<TYPE>* save;
	
	if (!current)
		return HASH_ERROR_NOITEMS;
	
	while (current) {
		if (!saveData)
			delete current->data;
		save = current->nextNode;
		delete current;
		current = save;
	}
	
	firstNode = NULL;
	lastNode = NULL;
	lastFind = NULL;
	currNode = NULL;
	numItems = 0;

	// reset hash table 
	for (int i=0; i < HASHTABLESIZE; i++)
		hashTable[i] = NULL;
	
	return HASH_STATUS_OK;
	
}

#ifdef HASH_DEBUG
// debug dump to stdout 
template <class TYPE>
void Hash<TYPE>::display(void)
{
	
	lh_node<TYPE>* current = firstNode;
	
	cout << "---listhash dump---\n";
	
	while (current) {
		current->data->display();
		current = current->nextNode;
	}
	
	cout << "--------------------\n";
	
}
#endif

#endif
