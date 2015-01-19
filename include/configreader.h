/*

	project althea
	original code: weyrick
	
	copyright (c) 2000 Roadsend, Inc.


	DESCRIPTION:
		a class to read in and parse c-style config files

	CHANGE HISTORY:
	


*/
 

#ifndef ACONFIGREADER_H
#define ACONFIGREADER_H

#include "String.h"
#include "listhash.h"
#include "log.h"
#include "fstream.h"

// a config block is a block in a config file surrounded by braces with a header that
// contains key value pairs or other blocks
class configBlock {
public:
	// constructor
	configBlock(String h) {
		header = h;
	}

	// destructor
	~configBlock(void) {
		dataPairs.freeList();
		subBlocks.freeList();
	}

	// add a new key/value pair to this block
	void addPair(String key, String value) { 
		String * newData = new String(value);
		if (dataPairs.addItem(newData, key))
			Log.logError("configBlock::addPair: unable to add data pair");
	}

	// add a new block to this block
	void addBlock(String key, configBlock * newBlock) { 
		if (subBlocks.addItem(newBlock, key))
			Log.logError("configBlock::addBlock: unable to add sub block");
	}

	// get a key/value pair from the dataPairs list
	String getPair(String key) { 
		String * match = dataPairs.findItem(key);
		if (match)
			return *match;
		else {
			Log.logWarning("configBlock::getPair: didn't find requested data/pair with key: "+key);
			return "";
		}
	}

	// get a key/block pair from the subBlocks list
	configBlock * getSubBlock(String key) { 
		configBlock * b = subBlocks.findItem(key);
		if (!b)
			Log.logWarning("configBlock::getSubBlock: didn't find requested subblock with key: "+key);
		return b;
	}


	// get a hash of sub blocks in this block
	Hash<configBlock>& getSubBlockList(void) { return subBlocks; }

	// get the number of pairs in this block
	unsigned int getNumPairs(void) { return dataPairs.getNumItems(); }

	// get the number of sub blocks in this block
	unsigned int getNumSubBlocks(void) { return subBlocks.getNumItems(); }

private:
	String header;						// header of the block
	Hash<String> dataPairs;				// list of data pairs
	Hash<configBlock> subBlocks;		// list of blocks within this block

};

class ConfigReader {

public:
	ConfigReader(String fn) {
		
		fileName = fn;
		infile.open(fn);

		if (!infile)
			Log.logError("ConfigReader::ConfigReader: couldn't open config file: " + fn);

		Log.logDebug("ConfigReader: loading new config file ["+fileName+"]");

	}

	// cleanup
	~ConfigReader(void) { 
		infile.close(); 
		cBlocks.freeList();
	}

	// initiate parse procedure
	unsigned int parse(void) { 
		parseBlocks(NULL);
		return cBlocks.getNumItems();
	}

	// return the config blocks we found
	Hash<configBlock>& getConfigBlocks(void) { return cBlocks; }


	// return a config block by key
	configBlock * getBlock(String key) { 
		configBlock * b = cBlocks.findItem(key);
		if (!b)
			Log.logWarning("ConfigReader::getBlock: couldn't find block with key: "+key);
		return b; 
	}

private:	
	// parse the config file into sections. called recursively
	void parseBlocks(configBlock * curBlock);
	
	String fileName;			// file name of config file we will parse
	Hash<configBlock> cBlocks;	// our list of config blocks
	ifstream infile;			// the file stream we will be working with

};

#endif
