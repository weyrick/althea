/*

	project althea
	original code: weyrick
	
	copyright (c) 2000 Roadsend, Inc.


	DESCRIPTION:
		implementation for config reader class
		
		syntax errors in config files generate warning errors

	CHANGE HISTORY:
	


*/

#include "configreader.h"
#include "log.h"

// parse a config file into sections
// curBlock is the current block we are parsing, recursively. (NULL if first iteration)
void ConfigReader::parseBlocks(configBlock * curBlock) { 


	char ch;
	bool inQuote=false;			// are we in a quoted string at the moment?
	String buf, splits[2];
	configBlock * newBlock;

	// start off searching for mode bracket
	while (infile) {

		// get a character
		infile.get(ch);

		// handle character in switch
		switch (ch) {
		
		case '"':
			(inQuote) ? inQuote = false : inQuote = true;
			continue;

		// eat up white space
		case '\t':
		case '\r':
		case '\n':
			continue;

		case ' ':
			if (!inQuote)
				continue;
			else
				buf += ch;
			continue;

		// zip to the end of the line on a comment
		case '#':
			while ((infile)&&(ch != '\n'))
				infile.get(ch);
			continue;

		// we're starting a new mode
		case '{':

			// buf should contain the new block header, switch to block mode.
			newBlock = new configBlock(buf);

			// if we have a current block, add this block to it
			if (curBlock)
				curBlock->addBlock(buf, newBlock);
			// otherwise add it to our main block list
			else {
				if (cBlocks.addItem(newBlock, buf))
					Log.logError("ConfigReader::parseBlocks: couldn't add config block to local list");
			}

			// parse this new block
			parseBlocks(newBlock);

			buf.clear();
			continue;

		// we're ending a block. if curBlock is NULL (ie, first call) then keep parsing, else return to caller
		case '}':
			if (curBlock)
				return;
			break;

		// fall through
		default:

			// if this is a semi colon, we've reached the end of the line and can parse
			if (ch == ';') {
				split(buf, splits, 2, '=');
				// splits[0] contains key
				// splits[1] contains value
				if (curBlock)
					curBlock->addPair(splits[0], splits[1]);
				else {
					// if curBlock is NULL, then we're not inside of a block currently
					Log.logWarning("ConfigReader::parseBlocks: parse error, found semi colon outside of block");
					continue;
				}
				buf.clear();
			}
			else {
				// this is part of a command and should be added to buf
				buf += ch;
			}
			break;

		} // end switch

	} // end file load loop

	return;

}
