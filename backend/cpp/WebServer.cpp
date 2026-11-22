#include "Graph.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <map>
#include <mutex>
#include <queue>
#include <sstream>
#include <string>
#include <thread>
#include <vector>
#ifdef _WIN32
  #define NOMINMAX
  #include <winsock2.h>
  #include <ws2tcpip.h>
  using socket_t = SOCKET;
  const socket_t BAD_SOCKET = INVALID_SOCKET;
  void close_socket(socket_t s) { closesocket(s); }
#else
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <unistd.h>
  using socket_t = int;
  const socket_t BAD_SOCKET = -1;
  void close_socket(socket_t s) { close(s); }
#endif
using namespace std;

static const int PORT = 8000;
static mutex dataMutex;

string readFile(const string& path) {
    ifstream f(path.c_str(), ios::binary);
    if (!f) return "";
    ostringstream ss; ss << f.rdbuf(); return ss.str();
}
string jsonEscape(const string& s) {
    string o;
    for (unsigned char c : s) {
        if (c == '"' || c == '\\') { o += '\\'; o += (char)c; }
        else if (c == '\n') o += "\\n";
        else if (c == '\r') o += "\\r";
        else if (c == '\t') o += "\\t";
        else if (c >= 32) o += (char)c;
    }
    return o;
}
string getJsonString(const string& body, const string& key) {
    string needle = "\"" + key + "\"";
    size_t p = body.find(needle);
    if (p == string::npos) return "";
    p = body.find(':', p + needle.size());
    if (p == string::npos) return "";
    p = body.find('"', p + 1);
    if (p == string::npos) return "";
    string out; bool esc = false;
    for (++p; p < body.size(); ++p) {
        char c = body[p];
        if (esc) {
            if (c == 'n') out += '\n'; else if (c == 'r') out += '\r';
            else if (c == 't') out += '\t'; else out += c;
            esc = false;
        } else if (c == '\\') esc = true;
        else if (c == '"') break;
        else out += c;
    }
    return out;
}
int getJsonInt(const string& body, const string& key, int fallback=-1) {
    string needle = "\"" + key + "\"";
    size_t p = body.find(needle);
    if (p == string::npos) return fallback;
    p = body.find(':', p + needle.size());
    if (p == string::npos) return fallback;
    ++p; while (p < body.size() && isspace((unsigned char)body[p])) ++p;
    try {
        if (p < body.size() && body[p] == '"') ++p;
        return stoi(body.substr(p));
    } catch (...) { return fallback; }
}
string response(const string& body, int status=200, const string& type="application/json; charset=utf-8") {
    string label = status == 200 ? "OK" : status == 400 ? "Bad Request" :
                   status == 401 ? "Unauthorized" : status == 404 ? "Not Found" :
                   status == 409 ? "Conflict" : "Internal Server Error";
    ostringstream out;
    out << "HTTP/1.1 " << status << " " << label << "\r\n"
        << "Content-Type: " << type << "\r\n"
        << "Content-Length: " << body.size() << "\r\n"
        << "Cache-Control: no-store\r\n"
        << "Access-Control-Allow-Origin: *\r\n"
        << "Access-Control-Allow-Headers: Content-Type\r\n"
        << "Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n"
        << "Connection: close\r\n\r\n" << body;
    return out.str();
}
string errJson(const string& msg) { return "{\"ok\":false,\"msg\":\"" + jsonEscape(msg) + "\"}"; }

bool validUser(const string& u) {
    if (u.empty() || u.size() > 80) return false;
    for (unsigned char c : u) if (isspace(c) || c == '\t') return false;
    return true;
}
map<string,string> loadUsers() {
    map<string,string> users;
    ifstream f("backend/data/users.txt");
    string u,p;
    while (f >> u >> p) users[u] = p;
    return users;
}
bool authenticate(const string& u, const string& p) {
    map<string,string> users = loadUsers();
    map<string,string>::const_iterator it = users.find(u);
    return it != users.end() && it->second == p;
}
struct Contact { string user, name, phone; };
vector<Contact> loadContacts() {
    vector<Contact> v; ifstream f("backend/data/contacts.txt");
    string line;
    while (getline(f,line)) {
        istringstream ss(line); Contact c;
        if (getline(ss,c.user,'\t') && getline(ss,c.name,'\t') && getline(ss,c.phone)) v.push_back(c);
    }
    return v;
}
void saveContacts(const vector<Contact>& v) {
    ofstream f("backend/data/contacts.txt", ios::trunc);
    for (const auto& c : v) f << c.user << '\t' << c.name << '\t' << c.phone << '\n';
}
struct Alert { string user, location, time; };
vector<Alert> loadAlerts() {
    vector<Alert> v; ifstream f("backend/data/sos.txt"); string line;
    while (getline(f,line)) {
        istringstream ss(line); Alert a;
        if (getline(ss,a.user,'\t') && getline(ss,a.location,'\t') && getline(ss,a.time)) v.push_back(a);
    }
    return v;
}
void saveAlerts(const vector<Alert>& v) {
    ofstream f("backend/data/sos.txt", ios::trunc);
    for (const auto& a : v) f << a.user << '\t' << a.location << '\t' << a.time << '\n';
}
string contactsJson(const vector<Contact>& all, const string& user) {
    string out = "[";
    bool first = true;
    for (const auto& c : all) if (c.user == user) {
        if (!first) out += ",";
        first = false;
        out += "{\"name\":\"" + jsonEscape(c.name) + "\",\"phone\":\"" + jsonEscape(c.phone) + "\"}";
    }
    return out + "]";
}
string alertsJson(const vector<Alert>& all) {
    string out = "[";
    for (size_t i=0;i<all.size();++i) {
        if (i) out += ",";
        out += "{\"user\":\"" + jsonEscape(all[i].user) + "\",\"location\":\"" +
               jsonEscape(all[i].location) + "\",\"time\":\"" + jsonEscape(all[i].time) + "\"}";
    }
    return out + "]";
}
string nowTime() {
    time_t t = time(nullptr); tm* x = localtime(&t); char b[16] = {};
    if (x) strftime(b,sizeof(b),"%H:%M:%S",x);
    return b;
}

void buildGraph(Graph& g) {
    string n[7]={"College Gate","Main Market","City Mall","Bus Stand","Railway Station","Hospital","Safe Shelter"};
    for(int i=0;i<7;i++) g.addLocation(i,n[i]);
    g.addRoad(0,1,2,8); g.addRoad(0,2,5,2); g.addRoad(1,2,2,7);
    g.addRoad(1,3,3,8); g.addRoad(2,3,3,2); g.addRoad(2,5,4,1);
    g.addRoad(3,4,2,9); g.addRoad(4,5,3,7); g.addRoad(5,6,2,1); g.addRoad(3,6,5,3);
}
struct RouteResult { bool found; vector<int> path; int risk, distance; };
RouteResult dijkstra(Graph& g,int src,int dst,bool safe) {
    vector<vector<Edge>>& adj=g.getGraph(); int n=(int)adj.size(), INF=1000000000;
    typedef pair<int,int> Cost;
    vector<Cost> dist(n,Cost(INF,INF)); vector<int> parent(n,-1);
    priority_queue<pair<Cost,int>,vector<pair<Cost,int>>,greater<pair<Cost,int>>> pq;
    dist[src]=Cost(0,0); pq.push(make_pair(Cost(0,0),src));
    while(!pq.empty()) {
        pair<Cost,int> cur=pq.top(); pq.pop();
        if(cur.first>dist[cur.second]) continue;
        int u=cur.second; if(u==dst) break;
        for(const Edge& e:adj[u]) {
            Cost nc=safe?Cost(cur.first.first+e.risk,cur.first.second+e.distance):
                         Cost(cur.first.first+e.distance,cur.first.second+e.risk);
            if(nc<dist[e.destination]) { dist[e.destination]=nc; parent[e.destination]=u; pq.push(make_pair(nc,e.destination)); }
        }
    }
    RouteResult r; r.found=dist[dst].first<INF; r.risk=0; r.distance=0;
    if(!r.found) return r;
    for(int x=dst;x!=-1;x=parent[x]) r.path.push_back(x);
    reverse(r.path.begin(),r.path.end());
    r.risk=safe?dist[dst].first:dist[dst].second;
    r.distance=safe?dist[dst].second:dist[dst].first;
    return r;
}
string routePart(const RouteResult& r) {
    ostringstream o; o << "{\"found\":" << (r.found?"true":"false") << ",\"path\":[";
    for(size_t i=0;i<r.path.size();i++) o << (i?",":"") << r.path[i];
    o << "],\"risk\":" << r.risk << ",\"distance\":" << r.distance << "}";
    return o.str();
}
string graphJson(Graph& g) {
    ostringstream o; o << "{\"nodes\":[";
    vector<string>& names=g.getLocationNames();
    for(int i=0;i<7;i++) o << (i?",":"") << "{\"id\":" << i << ",\"name\":\"" << jsonEscape(names[i]) << "\"}";
    o << "],\"edges\":["; bool first=true;
    vector<vector<Edge>>& adj=g.getGraph();
    for(int u=0;u<7;u++) for(const Edge& e:adj[u]) if(u<e.destination) {
        if(!first)o<<","; first=false;
        o << "{\"from\":"<<u<<",\"to\":"<<e.destination<<",\"distance\":"<<e.distance<<",\"risk\":"<<e.risk<<"}";
    }
    o << "]}"; return o.str();
}

string handleApi(const string& method,const string& path,const string& body,Graph& graph) {
    if(method=="GET" && path=="/api/health") return "{\"ok\":true,\"message\":\"C++ backend connected\"}";
    if(method=="GET" && path=="/api/graph") return graphJson(graph);
    if(method=="GET" && path.find("/api/contacts?user=")==0) {
        string u=path.substr(path.find('=')+1);
        size_t amp=u.find('&'); if(amp!=string::npos)u=u.substr(0,amp);
        lock_guard<mutex> lock(dataMutex); return contactsJson(loadContacts(),u);
    }
    if(method=="GET" && path=="/api/sos") {
        lock_guard<mutex> lock(dataMutex); return alertsJson(loadAlerts());
    }
    string user=getJsonString(body,"user");
    if(path=="/api/signup" || path=="/api/login") {
        string pw=getJsonString(body,"password");
        if(!validUser(user)||pw.empty()||pw.find_first_of(" \t\r\n")!=string::npos)
            return errJson("User ID / password is required (spaces are not allowed).");
        lock_guard<mutex> lock(dataMutex);
        map<string,string> users=loadUsers();
        if(path=="/api/signup") {
            if(users.find(user)!=users.end()) return errJson("User ID already registered.");
            ofstream f("backend/data/users.txt",ios::app); if(!f)return errJson("Could not open user data file.");
            f<<user<<" "<<pw<<"\n";
            return "{\"ok\":true,\"msg\":\"Account created. Please log in.\"}";
        }
        bool ok=authenticate(user,pw);
        return string("{\"ok\":")+(ok?"true":"false")+",\"msg\":\""+(ok?"Login successful.":"Incorrect User ID or password.")+"\"}";
    }
    if(path=="/api/route") {
        int s=getJsonInt(body,"src"),d=getJsonInt(body,"dst");
        if(s<0||s>6||d<0||d>6)return errJson("Select valid source and destination locations.");
        RouteResult a=dijkstra(graph,s,d,true),b=dijkstra(graph,s,d,false);
        return "{\"safest\":"+routePart(a)+",\"shortest\":"+routePart(b)+"}";
    }
    if(user.empty()) return errJson("Please log in first.");
    lock_guard<mutex> lock(dataMutex);
    if(path=="/api/contacts/add") {
        string name=getJsonString(body,"name"),phone=getJsonString(body,"phone");
        if(name.empty()||phone.empty())return errJson("Enter both name and phone number.");
        vector<Contact> cs=loadContacts();
        for(const auto& c:cs) if(c.user==user && c.name==name) return errJson("A contact with this name already exists.");
        cs.push_back({user,name,phone}); saveContacts(cs);
        return "{\"ok\":true,\"contacts\":"+contactsJson(cs,user)+"}";
    }
    if(path=="/api/contacts/remove") {
        string name=getJsonString(body,"name"); vector<Contact> cs=loadContacts();
        cs.erase(remove_if(cs.begin(),cs.end(),[&](const Contact& c){return c.user==user&&c.name==name;}),cs.end());
        saveContacts(cs); return "{\"ok\":true,\"contacts\":"+contactsJson(cs,user)+"}";
    }
    if(path=="/api/sos") {
        vector<Alert> as=loadAlerts(); as.push_back({user,getJsonString(body,"location").empty()?"Unknown":getJsonString(body,"location"),nowTime()});
        saveAlerts(as); vector<Contact> cs=loadContacts();
        return "{\"ok\":true,\"contacts\":"+contactsJson(cs,user)+",\"sos\":"+alertsJson(as)+"}";
    }
    if(path=="/api/sos/process") {
        vector<Alert> as=loadAlerts(); if(!as.empty())as.erase(as.begin()); saveAlerts(as);
        return "{\"ok\":true,\"sos\":"+alertsJson(as)+"}";
    }
    return errJson("Not found");
}

string readRequest(socket_t client) {
    string req; char buf[8192]; size_t headerEnd=string::npos; size_t contentLength=0;
    while((headerEnd=req.find("\r\n\r\n"))==string::npos && req.size()<65536) {
#ifdef _WIN32
        int n=recv(client,buf,sizeof(buf),0);
#else
        ssize_t n=recv(client,buf,sizeof(buf),0);
#endif
        if(n<=0)return ""; req.append(buf,(size_t)n);
    }
    if(headerEnd==string::npos)return "";
    string headers=req.substr(0,headerEnd);
    size_t p=headers.find("\r\n");
    string first=headers.substr(0,p);
    istringstream line(first); string method,target,version; line>>method>>target>>version;
    size_t cl=headers.find("Content-Length:");
    if(cl==string::npos)cl=headers.find("content-length:");
    if(cl!=string::npos) { cl=headers.find(':',cl)+1; while(cl<headers.size()&&isspace((unsigned char)headers[cl]))cl++; try{contentLength=stoul(headers.substr(cl));}catch(...){} }
    size_t have=req.size()-(headerEnd+4);
    while(have<contentLength) {
#ifdef _WIN32
        int n=recv(client,buf,sizeof(buf),0);
#else
        ssize_t n=recv(client,buf,sizeof(buf),0);
#endif
        if(n<=0)break; req.append(buf,(size_t)n); have+=(size_t)n;
    }
    string body=req.substr(headerEnd+4,contentLength);
    string path=target.substr(0,target.find('?'));
    if(method=="OPTIONS")return response("",200);
    if(method=="GET" && (path=="/" || path=="/index.html")) {
        string html=readFile("frontend/index.html");
        return response(html.empty()?"Frontend file not found.":html,html.empty()?404:200,"text/html; charset=utf-8");
    }
    // Future local assets can be served from web/ without exposing parent paths.
    if(method=="GET" && path.find("/frontend/")==0 && path.find("..")==string::npos) {
        string asset=readFile(path.substr(1));
        string type="text/plain; charset=utf-8";
        if(path.size()>=5 && path.substr(path.size()-5)==".html")type="text/html; charset=utf-8";
        else if(path.size()>=4 && path.substr(path.size()-4)==".htm")type="text/html; charset=utf-8";
        else if(path.size()>=4 && path.substr(path.size()-4)==".css")type="text/css; charset=utf-8";
        else if(path.size()>=3 && path.substr(path.size()-3)==".js")type="application/javascript; charset=utf-8";
        else if(path.size()>=4 && path.substr(path.size()-4)==".png")type="image/png";
        else if(path.size()>=4 && path.substr(path.size()-4)==".jpg")type="image/jpeg";
        else if(path.size()>=5 && path.substr(path.size()-5)==".jpeg")type="image/jpeg";
        else if(path.size()>=4 && path.substr(path.size()-4)==".svg")type="image/svg+xml";
        return response(asset,asset.empty()?404:200,type);
    }
    Graph graph(7); buildGraph(graph);
    if(path.find("/api/")==0) {
        string result=handleApi(method,target,body,graph);
        int status=result.find("\"ok\":false")!=string::npos?400:200;
        if(path=="/api/login" && result.find("Incorrect User ID")!=string::npos)status=401;
        if(path=="/api/signup" && result.find("already registered")!=string::npos)status=409;
        if(result=="{\"error\":\"Not found\"}")status=404;
        return response(result,status);
    }
    return response("{\"error\":\"Not found\"}",404);
}
void serveClient(socket_t client) {
    string out=readRequest(client);
    if(!out.empty()) {
        size_t sent=0;
        while(sent<out.size()) {
#ifdef _WIN32
            int n=send(client,out.data()+sent,(int)(out.size()-sent),0);
#else
            ssize_t n=send(client,out.data()+sent,out.size()-sent,0);
#endif
            if(n<=0)break; sent+=(size_t)n;
        }
    }
    close_socket(client);
}
int main() {
#ifdef _WIN32
    WSADATA wsa; if(WSAStartup(MAKEWORD(2,2),&wsa)!=0){cerr<<"Winsock startup failed\n";return 1;}
#endif
    socket_t server=socket(AF_INET,SOCK_STREAM,0);
    if(server==BAD_SOCKET){cerr<<"Could not create server socket\n";return 1;}
#ifndef _WIN32
    int yes=1; setsockopt(server,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(yes));
#endif
    sockaddr_in addr; addr.sin_family=AF_INET; addr.sin_port=htons(PORT); addr.sin_addr.s_addr=htonl(INADDR_LOOPBACK);
    if(::bind(server,(sockaddr*)&addr,sizeof(addr))<0){cerr<<"Port 8000 is busy or unavailable\n";close_socket(server);return 1;}
    if(listen(server,20)<0){cerr<<"Could not listen on port 8000\n";close_socket(server);return 1;}
    cout<<"SheShield C++ backend running: http://localhost:8000\nPress Ctrl+C to stop.\n";
    while(true) {
        sockaddr_in peer;
#ifdef _WIN32
        int len=sizeof(peer);
#else
        socklen_t len=sizeof(peer);
#endif
        socket_t client=accept(server,(sockaddr*)&peer,&len);
        if(client==BAD_SOCKET)continue;
        thread(serveClient,client).detach();
    }
    close_socket(server);
#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}
