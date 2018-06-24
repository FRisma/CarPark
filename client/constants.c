const char *HTTP_PUT = "PUT %s HTTP/1.1\r\n"\
						"Content-Type: %s\r\n"\
						"Content-Length: %li\r\n\r\n"\
						"%s";

const char *HTTP_POST = "POST %s HTTP/1.1\r\n"\
						"Content-Type: %s\r\n"\
						"Content-Length: %li\r\n\r\n"\
						"%s";

const char *HTTP_GET = "GET %s HTTP/1.1\r\n\r\n";

const char *HTTP_DELETE = "DELETE %s HTTP/1.1\r\n\r\n";

const char *CT_JSON = "application/json";

const char *CT_TEXT = "text/plain";

/*
const unsigned int MAX_IP_LENGTH = 16;
const unsigned int MIN_IP_LENGTH = 7;
const unsigned int MAX_PORT_LENGTH = 6;
const unsigned int MIN_PORT_LENGTH = 2;
*/
