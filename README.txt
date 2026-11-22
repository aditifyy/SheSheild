SHESHIELD - SIMPLE C++ HTTP SERVER + FILE HANDLING

This version keeps the existing frontend and C++ project files.
The original DSA/source files have not been rewritten.

HOW THE PROJECT WORKS
1. Open this pbl folder in VS Code.
2. Run the launcher for your operating system.
3. Open http://localhost:8000 in your browser.
4. The C++ HTTP server receives requests from the website.
5. C++ processes the request and reads/writes the data files.
6. The server sends the result back to the website.

SIMPLE ARCHITECTURE
Frontend (HTML/CSS/JavaScript)
        |
        | HTTP request / response
        v
backend/cpp/WebServer.cpp
        |
        +--> Graph.cpp and route calculation
        |
        +--> File handling
                 users.txt
                 contacts.txt
                 sos.txt

FILE HANDLING USED
- users.txt: user ID and password, one account per line.
- contacts.txt: user ID, contact name and phone number separated by tabs.
- sos.txt: user ID, location and time separated by tabs.

In WebServer.cpp:
- ifstream is used to read files.
- ofstream is used to write or append data.
- users.txt is read to check login and signup.
- contacts.txt is read and rewritten when contacts are added/removed.
- sos.txt is read and rewritten when an SOS alert is saved/processed.

IMPORTANT TERMS
- HTTP server: receives browser requests and returns responses.
- Endpoint: a URL path handled by the server, for example /api/login.
- JSON: the format currently used to send structured data between JavaScript and C++.
- File handling: permanent local storage of users, contacts and SOS records.
These are different jobs; file handling does not replace the communication between the browser and C++.

RUN ON MAC / LINUX
1. Open this pbl folder in VS Code.
2. Open Terminal in this folder.
3. Run: chmod +x run_mac_linux.sh
4. Run: ./run_mac_linux.sh
5. Open http://localhost:8000

RUN ON WINDOWS
1. Install a C++ compiler (g++/MinGW) and make sure g++ is available in PATH.
2. Open this pbl folder in VS Code.
3. Run run_windows.bat.
4. Open http://localhost:8000

TO STOP
Press Ctrl+C in the terminal running the server.

FEATURES
- Login and signup
- Safe route and shortest route comparison
- Trusted contact management
- Browser GPS location and map links
- SOS alert saved in the local project file
- Emergency call link to 112 and nearby police search

LIMITATIONS
- This is a local academic prototype, not a real emergency dispatch service.
- Browser GPS requires user permission; localhost is supported.
- SMS links open the messaging app. The user must review and press Send.
- SOS records are stored locally; police/emergency services are not automatically notified.
- The current website/backend communication still uses HTTP endpoints and JSON.
