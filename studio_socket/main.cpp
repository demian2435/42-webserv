#include "Server.hpp"
#include "Request.hpp"
#include "Config.hpp"
#include <vector>

int main(void)
{
	std::string str = "POST /upload?upload_progress_id=12344 HTTP/1.1\nHost: localhost:3000\nContent-Length: 1325\nOrigin: http://localhost:3000\n... other headers ...\nContent-Type: multipart/form-data; boundary=----WebKitFormBoundaryePkpFF7tjBAqx29L\n\n------WebKitFormBoundaryePkpFF7tjBAqx29L\nContent-Disposition: form-data; name=\"MAX_FILE_SIZE\"\n100000\n------WebKitFormBoundaryePkpFF7tjBAqx29L\nContent-Disposition: form-data; name=\"uploadedfile\"; filename=\"hello.o\"\nContent-Type: application/x-object\n... contents of file goes here ...\n------WebKitFormBoundaryePkpFF7tjBAqx29L--";

	char *r = &str[0];
	Request req(r);
	if (req.content_type.compare("") && !req.method.compare("POST"))
		req.print_request();
}
