#include <stdint.h>


class JMXProtocol
{
public:

	/**
	  * Constructor for the JMXParser()
	  *
	  * The JMXParser is a lightweight json parser that has a single byte look behind, and a 21 byte static RAM footprint.
	  * During processing, the parser allocates a struct of whatever size is indicated by the programmer from the actionStore.
	  *
	  * After a packet has been processed, the function specified in the actionStore will be invoked with a void pointer to where
	  * your struct has been allocated.
	  */
	JMXProtocol();

	/**
	  *	Accepts a single character and updates the state of this instance of JMX parser.
	  *
	  *	@param c the character to process
	  *
	  * @return PARSER_OK if the parser has moved to a valid state, PARSER_ERROR if the parser has moved to an invalid state
	  *			and PARSER_SUCCESS if the JSON packet has been processed successfully.
	  *
	  * @note On PARSER_ERROR, all state for the parser will be reset, thus you will have to parse from the beginning of the next packet.
	  */
	int parse(char c);

	int send(const char* identifier, void* data);

	int sendAndReceive(const char* identifier, void* send_buffer, void* rec_buffer);
};
