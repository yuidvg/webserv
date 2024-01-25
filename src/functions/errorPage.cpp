// #include "../webserv.hpp"

// static HttpResponse createHttpDefaultErrorPage(const unsigned int statusCode)
// {
//     if (statusCode <= 400 && statusCode < 500)
//         return BAD_REQUEST_RESPONSE;
//     else
//         return SERVER_ERROR_RESPONSE;
// }

// HttpResponse createHttpErrorPage(const HttpResponse &httpresponse, const Servers &servers, const Socket &sd)
// {
//     const unsigned int statusCode = httpresponse.statusCode;
//     if (host == NULL)
//         return createHttpDefaultErrorPage(statusCode);
//     const MatchedServerResult serverResult = matchedServer(httpresponse.host, servers, sd);
//     if (!serverResult.success)
//         return createHttpDefaultErrorPage(statusCode);

//     std::map<int, std::string>::const_iterator it = serverResult.value.errorPages.find(statusCode);

//     if (it != server.errorPages.end())
//     {
//         std::ifstream file(it->second);
//         if (file)
//         {
//             std::string content;
//             std::string line;
//             while (std::getline(file, line))
//             {
//                 content += line;
//             }
//             file.close();
//             return HttpResponse(statusCode, Headers("text/html", utils::toString(content.length())), content);
//         }
//         else
//         {
//             return createHttpDefaultErrorPage(statusCode);
//         }
//     }
//     else
//     {
//         return createHttpDefaultErrorPage(statusCode);
//     }
// }
