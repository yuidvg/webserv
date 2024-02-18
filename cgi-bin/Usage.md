### helloWorld.cgi
- [GET]
`curl -X GET http://localhost:8080/cgi-bin/helloWorld.cgi -v`

- [POST]
`curl -X POST http://localhost:8080/cgi-bin/helloWorld.cgi -d "name=sample" -v`


### localRedirect.cgi
`curl -X GET http://localhost:8080/cgi-bin/localRedirect.cgi -v -L`


### remoteRedirect.cgi
`curl -X GET http://localhost:8080/cgi-bin/remoteRedirect.cgi -v -L`


### redirAfterFiveMinutes.cgi
`curl -X GET http://localhost:8080/cgi-bin/redirAfterFiveMinutes.cgi -v -L`


### submit.cgi
`curl -X POST http://localhost:8080/cgi-bin/submit.cgi -d "food=sample" -v`


### infiniteLoop.cgi
`curl -X GET http://localhost:8080/cgi-bin/infiniteLoop.cgi -v`