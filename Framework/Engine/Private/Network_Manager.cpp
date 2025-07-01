//#include "NetWork_Manager.h"
//
//CNetwork_Manager::CNetwork_Manager()
//{
//}
//
//HRESULT CNetwork_Manager::Initialize()
//{
//
//    return S_OK;
//}
//
//TEST* CNetwork_Manager::Ping()
//{
//    std::string host = "localhost";
//    std::string port = "8080";
//    std::string target = "/ping";
//
//    json j = SendHttpGetJson(host, port, target);
//
//    // JSON 객체처럼 사용 가능
//    TEST* test = new TEST();
//    test->title = j.at("title").get<string>();
//    test->desc = j.at("desc").get<string>();
//
//    return test;
//}
//
//list<USER*> CNetwork_Manager::Get_AllUsers()
//{
//    std::string host = "localhost";
//    std::string port = "8080";
//    std::string target = "/users";
//
//    json j = SendHttpGetJson(host, port, target);
//    list<USER*> user_list;
//    // JSON 객체처럼 사용 가능
//    for (auto it = j["users"].begin(); it != j["users"].end(); ++it)
//    {
//        string key = it.key();
//        json value = it.value();
//
//        USER* user = new USER();
//        user->iId = stoi(key);
//        user->sName = value.at("name").get<string>();
//        user->sNickname = value.at("nickname").get<string>();
//        user->sPassword = value.at("password").get<string>();
//        user_list.push_back(user);
//    };
//
//    return user_list;
//}
//
//json CNetwork_Manager::SendHttpGetJson(const string& host, const string& port, const string& target)
//{
//    try {
//        net::io_context ioc;
//        tcp::resolver resolver(ioc);
//        beast::tcp_stream stream(ioc);
//
//        auto const results = resolver.resolve(host, port);
//        stream.connect(results);
//
//        http::request<http::string_body> req{ http::verb::get, target, 11 };
//        req.set(http::field::host, host);
//        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
//
//        http::write(stream, req);
//
//        beast::flat_buffer buffer;
//        http::response<http::dynamic_body> res;
//        http::read(stream, buffer, res);
//
//        stream.socket().shutdown(tcp::socket::shutdown_both);
//
//        std::string body = beast::buffers_to_string(res.body().data());
//        return json::parse(body);
//    }
//    catch (const std::exception& e) {
//        std::cerr << "에러: " << e.what() << std::endl;
//        return {};
//    }
//}
//
//CNetwork_Manager* CNetwork_Manager::Create()
//{
//    CNetwork_Manager* pInstance = new CNetwork_Manager();
//
//    if (FAILED(pInstance->Initialize()))
//    {
//        MSG_BOX(TEXT("Failed to Created : CNetwork_Manager"));
//        Safe_Release(pInstance);
//    }
//
//    return pInstance;
//}
//
//void CNetwork_Manager::Free()
//{
//    __super::Free();
//
//}
