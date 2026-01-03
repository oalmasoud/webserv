# Webserv

Scaffolded project structure and runtime workflow.

Project layout

Webserv/
├─ src/
│   ├─ main.cpp                     # نقطة البداية: تنشئ Server أو MultiServer إذا تعدد البورت
│   ├─ server/
│   │   ├─ Server.hpp                # إدارة sockets + bind + listen + poll + multi-port
│   │   ├─ Server.cpp
│   │   ├─ PollManager.hpp           # إدارة جميع fd + poll events + epoll-ready
│   │   ├─ PollManager.cpp
│   │   ├─ Client.hpp                # fd + buffer + state + keep-alive + request queue
│   │   └─ Client.cpp
│   ├─ http/
│   │   ├─ HttpRequest.hpp           # parsing request, method, headers, body
│   │   ├─ HttpRequest.cpp
│   │   ├─ HttpResponse.hpp          # build response, headers, body, chunked
│   │   ├─ HttpResponse.cpp
│   │   ├─ Router.hpp                # route request → Controllers
│   │   └─ Router.cpp
│   ├─ controllers/
│   │   ├─ BaseController.hpp        # interface: handleRequest(HttpRequest)
│   │   ├─ BaseController.cpp
│   │   ├─ StaticController.hpp      # serve static files
│   │   ├─ StaticController.cpp
│   │   ├─ ErrorController.hpp       # 404 / 500
│   │   ├─ ErrorController.cpp
│   │   ├─ ApiController.hpp         # dynamic JSON output
│   │   ├─ ApiController.cpp
│   │   └─ UploadController.hpp      # file uploads, multipart/form-data
│   │       UploadController.cpp
│   └─ utils/
│       ├─ Logger.hpp                # log connections, errors, requests
│       ├─ Logger.cpp
│       ├─ FileManager.hpp           # read/write files safely
│       └─ FileManager.cpp
├─ include/                           # common headers
├─ tests/                             # unit / integration tests
├─ CMakeLists.txt
└─ README.md

Runtime workflow

1. Server Start
	- socket() → create server_fd
	- setsockopt(SO_REUSEADDR)
	- bind()
	- fcntl(O_NONBLOCK)
	- listen()

2. Poll Loop (PollManager)
	- poll() or epoll_wait()
	- check all fd

3. Accept New Clients
	- if server_fd ready → accept()
	- create Client object
	- add to PollManager
	- initialize buffers & state
	- set fd non-blocking

4. Read Requests
	- poll detects client_fd ready for reading
	- read into buffer
	- if bytes_read == 0 → client disconnected → close & remove
	- parse HttpRequest
	- store in Client request queue (for keep-alive)

5. Route Request
	- Router chooses controller:
		 - StaticController → files
		 - ApiController → dynamic JSON
		 - UploadController → handle multipart/form-data
		 - ErrorController → 404 / 500
	- Controller returns HttpResponse

6. Write Response
	- put response in Client.outputBuffer
	- poll monitors fd for write (POLLOUT)
	- write() response fully
	- if keep-alive → reset Client state → wait for next request
	- if close → close fd & remove from PollManager

7. Logging & Utilities
	- Logger logs each request/response/error
	- FileManager ensures safe file read/write

8. Repeat
	- Poll loop continues indefinitely
	- supports multiple ports if Server holds multiple server_fd
	- supports hundreds of clients simultaneously


