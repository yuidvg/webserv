#include "../autoindex/all.hpp"
#include "../config/path.hpp"
#include "build.hpp"

HttpResponse conductPost(const HttpRequest &request, const Location &location)
{
    const std::string targetResourcePath = comply(request.target, location);
    if (utils::isDirectory(targetResourcePath))
        return HttpResponse::build(Http::Status::FORBIDDEN, request.version);
}
