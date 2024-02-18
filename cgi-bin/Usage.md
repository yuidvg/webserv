### helloWorld.cgi
- [GET]
curl -X GET http://localhost:8080/cgi-bin/helloWorld.cgi

- [POST]
curl -X POST http://localhost:8080/cgi-bin/helloWorld.cgi -d "name=sample"


### localRedirect.cgi
curl -X GET http://localhost:8080/cgi-bin/localRedirect.cgi


### remoteRedirect.cgi
curl -X GET http://localhost:8080/cgi-bin/remoteRedirect.cgi


### redirAfterFiveMinutes.cgi
curl -X GET http://localhost:8080/cgi-bin/redirAfterFiveMinutes.cgi


### submit.cgi
curl -X POST http://localhost:8080/cgi-bin/submit.cgi -d "food=sample"


### infiniteLoop.cgi
curl -X GET http://localhost:8080/cgi-bin/infiniteLoop.cgi